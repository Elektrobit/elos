#!/usr/bin/env python3

import json
import os
import os.path as path
import subprocess
import multiprocessing

BASE_DIR = path.abspath(path.join(path.dirname(path.abspath(__file__)), '..'))
CHECKOUT_PATH = path.join(BASE_DIR, "build/deps/src")
INSTALL_PATH = path.join(BASE_DIR, "build/deps")


def dependency_sources(user_config="dependencies.json"):
    defaults = path.join(BASE_DIR, "ci/dependency_default.json")
    with open(defaults, "r") as d:
        dependency = json.load(d)
    user_config = path.join(BASE_DIR, user_config)
    if os.path.isfile(user_config):
        with open(user_config, "r") as u:
            dependency_user = json.load(u)
        for key, val in dependency.items():
            if key in dependency_user:
                dependency[key] = dependency[key] | dependency_user[key]

    return dependency

def run_cmd(cmd):
    print(*cmd)
    return subprocess.run(cmd)

def checkout(dependencies):
    for dep, conf in dependencies.items():
        reop_path = path.join(CHECKOUT_PATH, dep)
        print(f"## {dep}")
        if "path" not in conf:
            conf["path"] = reop_path
            if path.exists(reop_path):
                cmd = ["git", "-C", conf["path"], "fetch"]
                run_cmd(cmd)
                cmd = ["git", "-C", conf["path"], "checkout"]
                if "commit" in conf:
                    cmd.append(conf["commit"])
                elif "tag" in conf:
                    cmd.append(conf["tag"])
                elif "branch" in conf:
                    cmd.append(conf["branch"])
                run_cmd(cmd)
                continue
            cmd = ["git", "clone", conf["url"], conf["path"]]
            if "tag" in conf:
                cmd.append("-b")
                cmd.append(conf["tag"])
            elif "branch" in conf:
                cmd.append("-b")
                cmd.append(conf["branch"])
            run_cmd(cmd)
            if "commit" in conf:
                cmd = ["git", "-C", conf["path"], "checkout", conf["commit"]]
                run_cmd(cmd)
        else:
            print("nothing to do for local repository!")


def single_install(dependency, config):
    print(f"## {dependency}")
    config["build"] = path.join(config["path"], "build")
    cmake_opts = config["cmake_opts"] if "cmake_opts" in config else []
    cmd = ["cmake", "-B", config["build"], config["path"],
           "-DCMAKE_BUILD_TYPE=Release", "-G", "Ninja",
           f"-DCMAKE_INSTALL_PREFIX={INSTALL_PATH}", *cmake_opts]
    cp = run_cmd(cmd)
    if cp.returncode != 0:
        print(f"FAILED cmake for {dependency}")
        return False

    cmd = ["ninja", "-C", config["build"],
           f"-j{multiprocessing.cpu_count()}", "all", "install"]
    cp = run_cmd(cmd)
    if cp.returncode != 0:
        print(f"FAILED to build and install {dependency}")
        return False
    return True


def build_and_install(dependencies):
    for dependency in ["cmocka_extensions", "cmocka_mocks", "safu", "samconf"]:
        if not single_install(dependency, dependencies[dependency]):
            break




if __name__ == '__main__':
    deps = dependency_sources()
    print("# Checkout")
    checkout(deps)
    print("# build")
    build_and_install(deps)
