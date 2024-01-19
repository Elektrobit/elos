#!/usr/bin/env python
""" Usage: call with <filename>
"""

import re
import os
import sys
import json
import clang.cindex

JCC_FILE = "build/Debug/cmake/compile_commands.json"

def parse_json_cc(json_file = JCC_FILE):
    """Parse the json compile commands file"""

    jcc_dict = None
    with open(json_file, encoding = "utf-8") as fh:
        jcc_dict = json.load(fh)
        fh.close()

    return jcc_dict

def parse_source_folder(source_folder, jcc_dict):
    """Parse .c and .h files within the given path"""

    source_entries = []
    abs_path = os.path.abspath(source_folder) + '/'

    for entry in jcc_dict:
        if entry["file"].startswith(abs_path):
            source_entries.append(entry)

    return source_entries

def jcc_entry_to_args(jcc_entry):
    """Convert an entry from the compile_commands.json into compiler args"""

    file = jcc_entry["file"]
    args = jcc_entry["command"]
    filename = os.path.basename(file)
    args = re.sub(f'-[oc] .+/{filename}\\S*', '', args)
    args = args.split(" ")[1:]
    to_remove = ["", "-Wno-analyzer-malloc-leak"]
    args = [x for x in args if x not in to_remove]

    return args

def traverse_source(func_dict, source, source_folder, root, node):
    """ Traverse the AST tree """

    file = str(node.location.file)

    # Add the node to function_declarations
    if node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        name = node.spelling

        if file == source:
            func_dict.setdefault(name, {})

            fdict = func_dict[name]
            fdict.setdefault("param", [])
            fdict.setdefault("locations", set())

            fdict["name"] = name
            fdict["returns"] = node.result_type.spelling
            fdict["locations"].add(file)

            for child in node.get_children():
                if child.kind == clang.cindex.CursorKind.PARM_DECL:
                    param_dict = {
                        "name" : child.spelling,
                        "type" : child.type.spelling,
                    }
                    fdict["param"].append(param_dict)

        elif file.startswith(source_folder):
            func_dict.setdefault(name, {})
            fdict = func_dict[name]
            fdict.setdefault("locations", set())
            fdict["locations"].add(file)

    # Recurse for children of this node
    for child in node.get_children():
        traverse_source(func_dict, source, source_folder, root, child)

    return

def traverse_source_entries(source_entries, source_folder):
    """ast traverse all given files"""

    func_dict = {}

    for source_entry in source_entries:
        source = source_entry["file"]
        args = jcc_entry_to_args(source_entry)
        cidx = clang.cindex.Index.create()
        tu = cidx.parse(source, args=args)
        root = tu.cursor
        traverse_source(func_dict, source, source_folder, root, root)

    return func_dict

def generate_func_dict(source_folder, jcc_file = JCC_FILE):
    """ast traverse all sources in given folder"""

    abs_source_folder = os.path.abspath(source_folder)
    jcc_dict = parse_json_cc(jcc_file)
    source_entries = parse_source_folder(abs_source_folder, jcc_dict)
    if len(source_entries) < 1:
        print(f"ERROR: Could not find any source files in '{source_folder}'")
        sys.exit(-1)

    func_dict = traverse_source_entries(source_entries, abs_source_folder)

    return func_dict
