#!/usr/bin/env python3
"""
Checks or fixes sources with clang-format
"""

import os
import re
import sys
import glob
import json
import argparse
import itertools
import shutil
import subprocess as sp


def get_env(key, default=None):
    """
    Gets the environment variable or a default value
    """
    result = os.getenv(key, default)
    if result is None or result == "":
        result = default
    return result


def test(error_code):
    """
    Returns TEST SUCCESS or TEST FAILED based on the system code
    """
    result = "TEST FAILED"
    if error_code == os.EX_OK:
        result = "TEST SUCCESS"
    return result


def flatten_list(l):
    """
    Returns a flattened list
    """
    # https://stackoverflow.com/questions/952914/how-do-i-make-a-flat-list-out-of-a-list-of-lists
    return [item for sublist in l for item in sublist]


def log_line(log_fh, line):
    """
    Prints the given line to stdout and writes it to a file
    """
    print(line)
    if log_fh:
        log_fh.write(line + "\n")


def collect_sources_all(path_list):
    """
    Collects all available sources (c and h files)
    in the given directory lists
    """
    source_set = set()

    for path in path_list:
        for root, _, files in os.walk(path):
            for file in files:
                if file.endswith(".c") or file.endswith(".h"):
                    source_set.add(os.path.abspath(os.path.join(root, file)))

    return source_set


def prepare_compile_commands(cfg):
    """
    Copy and adapt an existing compile_copmmands.json in a way
    that allows clang-tidy to run without errors due to gcc specific
    flags which clang does not understand
    """
    print("Preparing compile_commands.json ..")
    ccj_src = os.path.join(cfg["build_dir"], 'compile_commands.json')
    ccj_dst = os.path.join(cfg["result_dir"], 'compile_commands.json')

    shutil.copyfile(ccj_src, ccj_dst)
    with open(ccj_dst, 'r+', encoding="utf-8") as json_fh:
        json_data = json.loads(json_fh.read())
        json_fh.seek(0)
        json_fh.truncate()
        for i, _ in enumerate(json_data):
            if "command" in json_data[i]:
                cmd = json_data[i]["command"]
                cmd = cmd.replace("-fanalyzer ", "")
                json_data[i]["command"] = cmd
        json.dump(json_data, json_fh, indent=2)

    cfg["tidy_dir"] = cfg["result_dir"]


def collect_sources_used(build_dir, json_file=None):
    """
    Collects all available sources (c and h files)
    in the given compile_commands.json file
    """
    source_set = set()
    json_dict_list = None

    if json_file is None:
        json_file = os.path.join(build_dir, 'compile_commands.json')

    with open(json_file, 'r', encoding="utf-8") as json_fh:
        json_dict_list = json.loads(json_fh.read())

    inc_def_re = re.compile('(?:-I[^ ]+)|(?:-isystem [^ ]+)|(?:-D[^ ]+)')

    for json_dict in json_dict_list:
        source = json_dict["file"]
        idparam = [x.group(0) for x in inc_def_re.finditer(json_dict["command"])]
        idparam = ' '.join(idparam).split(" ")
        cc = json_dict["command"].split(" ")[0]
        command = [cc, '-MM', source, *idparam]
        output = sp.run(command, stdout=sp.PIPE, check=True)
        results = output.stdout.decode("utf-8").split()
        results = [
            os.path.abspath(x) for x in results
            if x.endswith(".c") or x.endswith(".h")
        ]
        # Remove generated files like version.c from the list
        results = [
            x for x in results
            if os.path.relpath(x, build_dir).startswith("..")
        ]
        source_set.update(results)

    return source_set


def collect_sources_diff(base_dir, target_branch):
    """
    Selects all sources (c and h files) that were modified
    and/or added between this branch and the target
    """
    source_set = set()
    cwd = os.getcwd()

    os.chdir(base_dir)
    command = ["git", "rev-parse", "--show-toplevel"]
    output = sp.run(command, stdout=sp.PIPE, check=True)
    git_dir = output.stdout.decode("utf-8").strip()
    os.chdir(git_dir)
    command = ["git", "diff", "--name-status", target_branch]
    output = sp.run(command, stdout=sp.PIPE, check=True)
    if len(output.stdout) == 0:
        return source_set

    results = output.stdout.decode("utf-8").split("\n")
    results = [
        x[2:] for x in results
        if x.startswith("M") or x.startswith("A")
    ]
    results = [
        os.path.abspath(x) for x in results
        if x.endswith(".c") or x.endswith(".h")
    ]
    # Remove sources outside of base_dir
    results = [
        x for x in results
        if not os.path.relpath(x, base_dir).startswith("..")
    ]
    source_set.update(results)
    os.chdir(cwd)

    return source_set


def collect_sources_list_from_env(env_var, no_match_msg="Globs that don't match anything:"):
    """
    Collect a list of sources from a list of globs in an enviroment variable.
    And print all the globs not matching any files.
    """
    source_list = []
    not_necessary = []
    for x in get_env(env_var, "").split(" "):
        if len(x) <= 1:
            continue
        paths = glob.glob(x)
        if len(paths) == 0:
            not_necessary.append(x)
        else:
            source_list.append(paths)
    source_set = {
        os.path.abspath(x) for x in flatten_list(source_list)
    }
    if len(not_necessary) != 0:
        print(no_match_msg)
        for x in sorted(not_necessary):
            print(f"  {x}")
        print("")

    return source_set


def collect_sources_ignored(cfg):
    """
    Collects ignored sources
    """
    return collect_sources_list_from_env("IGNORE_SOURCES", "Not existing IGNORE_SOURCES globs:")


def collect_sources_intentional_unused(cfg):
    """
    Collect sources that are not used
    but shouldn't be ignored
    """
    return collect_sources_list_from_env("UNUSED_SOURCES", "UNUSED_SOURCES globs not matcing anything:")


def collect_sources(cfg):
    """
    Collects all sources necessary for testing
    """
    source_set = {}
    ignored_set = collect_sources_ignored(cfg)

    if "show_sources" not in cfg["steps"]:
        print("Linting with the following settings:")
        print(f"BASE_DIR        = {cfg['base_dir']}")
        print(f"BUILD_DIR       = {cfg['build_dir']}")
        print(f"LINT_RESULT_DIR = {cfg['result_dir']}")
        print("")

        if len(ignored_set) > 0:
            print("Ignored sources:")
            for source in sorted(ignored_set):
                print(f"  {source}")
            print("")

        print("Collecting sources...")

    step_unused = sum([True for x in cfg["steps"] if x.endswith("unused")])

    if cfg["collect_mode"] == "diff":
        branch = "integration"
        source_set["diff"] = collect_sources_diff(cfg["base_dir"], branch)
        source_set["diff"] -= ignored_set

    if cfg["collect_mode"] == "used" or step_unused:
        source_set["used"] = collect_sources_used(cfg['build_dir'])
        source_set["used"] -= ignored_set

    if cfg["collect_mode"] == "all" or step_unused:
        folders = [os.path.join(cfg['base_dir'], x) for x in ["src", "test"]]
        source_set["all"] = collect_sources_all(folders)
        source_set["all"] -= ignored_set

    source_set["intentionally_unused"] = collect_sources_intentional_unused(
        cfg)
    source_set["default"] = source_set[cfg["collect_mode"]]

    step_tidy = sum([True for x in cfg["steps"] if x.endswith("tidy")])
    if step_tidy:
        mock_path = os.path.join(cfg["base_dir"], "test", "utest", "mocks")
        source_set["tidy"] = [
            x for x in source_set["default"]
            if x.startswith(mock_path) is False]

    return source_set


def show_sources(cfg, source_set):
    """
    Simply prints a list of selectes sources
    """
    sources = sorted(source_set[cfg["collect_mode"]])
    for source in sources:
        print(source)
    return os.EX_OK


def check_unused(cfg, source_set):
    """
    Checks for unused files
    """
    result = os.EX_OK
    state = "SUCCESS"
    log_file = os.path.join(cfg["result_dir"], "results_check_unused.txt")

    with open(log_file, 'w', encoding='utf-8') as log_fh:
        log_line(log_fh, "check_unused: Searching for unused files...")
        source_set_unused = source_set["all"].difference(source_set["used"])
        source_set_known_unused = source_set_unused.intersection(
            source_set["intentionally_unused"])
        source_set_unused -= source_set["intentionally_unused"]
        unused_file_count = len(source_set_unused)
        should_be_unused = source_set["used"].intersection(
            source_set["intentionally_unused"])

        if unused_file_count > 0:
            log_line(log_fh, "Unused sources:")
            for file in sorted(list(source_set_unused)):
                log_line(log_fh, "        " + file)
            log_line(log_fh, "")
            result = os.EX_DATAERR
            state = "FAILED"
        if len(source_set_known_unused) > 0:
            log_line(log_fh, f"Known and expected unused sources:")
            for source in sorted(source_set_known_unused):
                log_line(log_fh, f"        {source}")
            log_line(log_fh, f"")
        if len(should_be_unused) > 0:
            log_line(log_fh, f"Specified as not used sources that are in use:")
            for source in sorted(should_be_unused):
                log_line(log_fh, f"        {source}")
            log_line(log_fh, f"")
            result = os.EX_DATAERR
            state = "FAILED"

        log_line(log_fh, f"check_unused: TEST {state}; " +
                 f"Found {unused_file_count} unused files!")

    return result


def check_format(cfg, source_set):
    """
    Checks the given set of sources with clang-format
    """
    result = os.EX_SOFTWARE
    log_file = os.path.join(cfg["result_dir"], "results_check_format.txt")
    dump_file = os.path.join(cfg["result_dir"], "output_clang_format.txt")
    sources = source_set[cfg["collect_mode"]]
    error_lines = 0

    with open(log_file, 'w', encoding='utf-8') as log_fh, \
            open(dump_file, 'bw') as dump_fh:

        log_line(log_fh, "check_format: Verify formatting with clang-format..")
        if len(sources) == 0:
            log_line(log_fh, "check_format: TEST SKIPPED; No sources found.")
            return os.EX_OK

        log_fh.write("---- clang-format output start ---\n")
        params = ["--dry-run", "-Werror"]
        command = ["clang-format", *params, *sources]
        with sp.Popen(command, stdout=sp.PIPE, stderr=sp.STDOUT) as proc:
            buffer = ""
            while True:
                byte = proc.stdout.read(1)
                if byte:
                    dump_fh.write(byte)
                    buffer += byte.decode("utf-8")
                    if buffer[-1] == '\n':
                        if buffer.find(": error:") >= 0:
                            error_lines += 1
                            result = os.EX_DATAERR
                        log_line(log_fh, buffer)
                        buffer = ""
                else:
                    break
            result = proc.wait()

        log_fh.write("---- clang-format output stop ---\n")
        log_line(log_fh, f"check_format: {test(result)}; " +
                 f"Found {error_lines} errors " +
                 f"in {len(sources)} scanned files.")

    return result


def check_tidy(cfg, source_set):
    """
    Format the given set of sources with clang-format
    """
    result = os.EX_OK
    log_file = os.path.join(cfg["result_dir"], "results_check_tidy.txt")
    dump_file = os.path.join(cfg["result_dir"], "output_clang_tidy.txt")
    sources = source_set["tidy"]
    error_lines = 0

    with open(log_file, "w", encoding="utf-8") as log_fh, \
            open(dump_file, "bw") as dump_fh:

        log_line(log_fh, "check_tidy: Verify formatting with clang-tidy..")
        if len(sources) == 0:
            log_line(log_fh, "check_tidy: TEST SKIPPED; No sources found.")
            return result

        log_fh.write("---- clang-tidy output start ---\n")
        command_params = [
            "--extra-arg-before=-Wno-error=unknown-warning-option",
            "-p", cfg["tidy_dir"]
        ]
        command = ["clang-tidy", *command_params, *sources]
        with sp.Popen(command, stdout=sp.PIPE, stderr=sp.STDOUT) as proc:
            buffer = ""
            while True:
                byte = proc.stdout.read(1)
                if byte:
                    dump_fh.write(byte)
                    buffer += byte.decode("utf-8")
                    if buffer[-1] == '\n':
                        if buffer.find("warnings generated.") >= 0:
                            pass
                        elif buffer.find(": error:") >= 0:
                            log_line(log_fh, buffer)
                            error_lines += 1
                            result = os.EX_DATAERR
                        else:
                            log_line(log_fh, buffer)
                        buffer = ""
                else:
                    break
            result = proc.wait()

        log_fh.write("---- clang-tidy output stop ---\n")
        log_line(log_fh, f"check_tidy: {test(result)}; " +
                 f"Found {error_lines} errors " +
                 f"in {len(sources)} scanned files.")

    return result


def fix_unused(cfg, source_set):
    """
    Checks for unused files
    """
    del cfg
    result = os.EX_OK
    source_set_unused = source_set["all"].difference(source_set["used"])
    unused_file_count = len(source_set_unused)

    if unused_file_count > 0:
        print("fix_unused: Removing files...")
        for file in sorted(list(source_set_unused)):
            print("    " + file)
            os.unlink(file)
        print()
        print(f"fix_unused: Removed {unused_file_count} files.")
    else:
        print("fix_unused: No unused files found to remove.")

    return result


def fix_format(cfg, source_set):
    """
    Format the given set of sources with clang-format
    """

    result = os.EX_SOFTWARE
    sources = source_set[cfg["collect_mode"]]
    command = ["clang-format", "-i", *sources]

    print("fix_format: Try to fix formatting in sources")
    if len(sources) == 0:
        print("fix_format: TEST SKIPPED; No sources found.")
        return os.EX_OK

    with sp.Popen(command, stdout=sp.PIPE, stderr=sp.STDOUT) as proc:
        while True:
            byte = proc.stdout.read(1)
            if byte:
                sys.stdout.buffer.write(byte)
                sys.stdout.flush()
            else:
                break
        result = proc.wait()

    print(f"fix_format: {len(sources)} files were formatted.")

    return result


def fix_tidy(cfg, source_set):
    """
    Format the given set of sources with clang-tidy
    """

    result = os.EX_SOFTWARE
    sources = source_set["tidy"]
    command_params = [
        "--fix-errors",
        "--extra-arg=-Wno-error=unknown-warning-option",
        "--warnings-as-errors=-*",
        "-p", cfg["tidy_dir"]
    ]
    command = ["clang-tidy", *command_params, *sources]

    print("fix_tidy: Try to fix formatting in sources")
    if len(sources) == 0:
        print("fix_tidy: TEST SKIPPED; No sources found.")
        return os.EX_OK

    with sp.Popen(command, stdout=sp.PIPE, stderr=sp.STDOUT) as proc:
        buffer = ""
        while True:
            byte = proc.stdout.read(1)
            if byte:
                buffer += byte.decode("utf-8")
                if buffer[-1] == '\n':
                    if buffer.find("warnings generated.") < 0:
                        print(buffer)
                    buffer = ""
            else:
                break
        result = proc.wait()

    print(f"fix_tidy: {len(sources)} files were formatted.")

    return result


def parameter_process_steps(args, cfg):
    """
    Sets the steps to be executed
    """

    cfg["steps"] = set()
    steps_def = [["format", "f"], ["tidy", "t"]]
    steps_all = [["unused", "u"], *steps_def]
    steps = [*steps_all, ["all", "a"]]

    if args.ci:
        cfg["steps"].update([f"check_{x[0]}" for x in steps_all])
        return cfg
    if args.show:
        cfg["steps"].add("show_sources")
        return cfg

    modes = {"check": args.check, "fix": args.fix}
    active_modes = [x[0] for x in modes.items() if x[1] is not None]
    for key in active_modes:
        if len(modes[key]) == 0:
            cfg["steps"].update([f"{key}_{s[0]}" for s in steps_def])
            continue
        for value in modes[key]:
            found = False
            for step in steps:
                if value in step:
                    cfg["steps"].add(f"{key}_{step[0]}")
                    found = True
                    break
            if not found:
                values = ", ".join(itertools.chain(*steps))
                print(f"{sys.argv[0]}: error: " +
                      f"Unknown value '{value}', valid values are: {values}")
                sys.exit(os.EX_USAGE)
        if f"{key}_all" in cfg["steps"]:
            cfg["steps"].remove(f"{key}_all")
            cfg["steps"].update([f"{key}_{s[0]}" for s in steps_all])

    return cfg


def parameter_process_directories(args, cfg):
    """
    Processes parameters and environment to set used directories
    """
    cfg["base_dir"] = args.base_dir
    if cfg["base_dir"] is None:
        default = os.path.join(os.path.dirname(sys.argv[0]), '..')
        cfg["base_dir"] = get_env("BASE_DIR", default)
    cfg["base_dir"] = os.path.abspath(cfg["base_dir"])
    if os.path.exists(cfg["base_dir"]) is False:
        print(f"{sys.argv[0]}: error: " +
              f"BASE_DIR path '{cfg['base_dir']}' does not exist!")
        sys.exit(os.EX_OSFILE)

    cfg["build_dir"] = args.build_dir
    if cfg["build_dir"] is None:
        default = os.path.join(cfg["base_dir"], 'build', 'Debug', 'cmake')
        if args.ci:
            default = os.path.join(
                cfg["base_dir"], 'build', 'Release', "cmake")
        cfg["build_dir"] = get_env("BUILD_DIR", default)
    cfg["build_dir"] = os.path.abspath(cfg["build_dir"])
    if os.path.exists(cfg["build_dir"]) is False:
        print(f"{sys.argv[0]}: error: " +
              f"BUILD_DIR path '{cfg['build_dir']}' does not exist!")
        sys.exit(os.EX_OSFILE)

    cfg["result_dir"] = args.result_dir
    if cfg["result_dir"] is None:
        default = os.path.join(cfg["build_dir"], 'lint_results')
        cfg["result_dir"] = get_env("LINT_RESULT_DIR", default)
    cfg["result_dir"] = os.path.abspath(cfg["result_dir"])
    cfg["tidy_dir"] = cfg["result_dir"]
    if os.path.exists(cfg["result_dir"]) is False:
        os.mkdir(cfg["result_dir"])

    return cfg


def parameter_process(args, cfg):
    """
    Sets the default values for unset parameters and a few other things
    """
    if args.ci:
        cfg["ci"] = True
        cfg["collect_mode"] = "all"
    else:
        cfg["ci"] = False
        mode = {0: "diff", 1: "all", 2: "diff", 4: "used"}
        cfg["collect_mode"] = mode[
            int(args.select_all) << 0 |
            int(args.select_diff) << 1 |
            int(args.select_used) << 2
        ]

    cfg = parameter_process_steps(args, cfg)
    cfg = parameter_process_directories(args, cfg)

    return cfg


def parameter_parse():
    """
    Parses command line arguments
    """
    parser = argparse.ArgumentParser(
        description="Tool for checking and fixing source formatting")

    grp_mode = parser.add_mutually_exclusive_group(required=True)
    grp_mode.add_argument(
        "-c", "--check", nargs="*",
        help="Checks sources (unused sources, " +
        "clang-format, clang-tidy) and shows errors.")
    grp_mode.add_argument(
        "-f", "--fix", nargs="*",
        help="Try to fix sources (remove unused files," +
        " format with clang-format and clang-tidy). " +
        " MAY DAMAGE FILES, be sure to commit everything beforehand.")
    grp_mode.add_argument(
        "-s", "--show", action="store_true",
        help="Prints a list of selected sources")
    grp_mode.add_argument(
        "--ci", action="store_true",
        help="Special mode intended for use in " +
        "continuous integration, uses all tests and " +
        "./build/Release as build-dir per default.")

    grp_select = parser.add_mutually_exclusive_group(required=False)
    grp_select.add_argument(
        "-d", "--select-diff", action="store_true",
        help="Selects sources based on git branch " +
        "differences compare to the 'integration' branch (default)")
    grp_select.add_argument(
        "-u", "--select-used", action="store_true",
        help="Selects sources based on compile_commands.json" +
        "(only uses compiled files, slow)")
    grp_select.add_argument(
        "-a", "--select-all", action="store_true",
        help="Selects all sources in src and test")

    parser.add_argument(
        "--base-dir", help="Base directory (e.g. ./) to use; " +
        "Can also be set with 'export BASE_DIR'")
    parser.add_argument(
        "--build-dir", help="Build directory (e.g. ./build) to use; " +
        "Can also be set with 'export BUILD_DIR'")
    parser.add_argument(
        "--result-dir", help="Result directory " +
        "(e.g. ./build/lint_result) to use; " +
        "Can also be set with 'export LINT_RESULT_DIR'")

    args = parser.parse_args()

    return args


def execute_steps(cfg, source_set):
    """
    Runs every specified step and returns an appropriate success/error code
    """
    result = os.EX_OK
    results = []
    function = dict(x for x in globals().items() if callable(x[1]))

    for step in cfg["steps"]:
        if step not in function:
            raise Exception(f"No function exists for step '{step}'!")
        result = function[step](cfg, source_set)
        results.append(result)

    info = None
    failures = len([x for x in results if x != os.EX_OK])
    if failures > 0:
        result = os.EX_DATAERR
        info = f"FAILED; {failures} of {len(results)} steps failed!"
    else:
        info = f"SUCCESS; {len(results) - failures} " +\
               f"of {len(results)} steps executed successfully."

    if "show_sources" not in cfg["steps"]:
        print(info)

    return result


def main():
    """
    Main function, called during command line operation
    """
    args = parameter_parse()
    cfg = parameter_process(args, {})

    source_set = collect_sources(cfg)
    if len(source_set) == 0:
        print("No sources found." +
              " Perhaps try --select-json or --select-all? (see --help)")
        sys.exit(os.EX_NOINPUT)

    if "check_tidy" in cfg["steps"] or "fix_tidy" in cfg["steps"]:
        prepare_compile_commands(cfg)

    result = execute_steps(cfg, source_set)
    sys.exit(result)


if __name__ == "__main__":
    main()
