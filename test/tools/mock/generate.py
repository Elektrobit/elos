#!/usr/bin/env python
""" Usage: call with <filename>
"""

import os
import sys

import ast_parser

def append_include_list(mock, source_folder, func_dict):
    """Append list of includes to mock source or header"""

    abs_source_folder = os.path.abspath(source_folder)

    # Create a list of includes
    include_list = set()
    for _, fdict  in func_dict.items():
        for location in fdict["locations"]:
            if location.endswith(".h"):
                location = location.replace(abs_source_folder, '')
                location = location.replace('/interface/', '')
                location = location.replace('/public/', '')
                include_list.add(location)

    for include in sorted(include_list):
        mock.append(f"#include \"{include}\"")
    mock.append("")

def create_mock_source(source_folder, func_dict):
    """Create a mock source from the function dict"""

    cname = os.path.basename(source_folder).lower()

    mock = []

    mock.append("// SPDX-License-Identifier: MIT")
    mock.append("")
    mock.append(f"#include \"mock_{cname}.h\"")
    mock.append("")

    for _, fdict in func_dict.items():
        name = fdict['name']
        # Add intro
        pnames = ""
        intro = f"MOCK_FUNC_BODY({name}, {fdict['returns']}, "
        for param in fdict['param']:
            pname = param['name']
            ptype = param['type']
            if ptype.endswith("*"):
                intro += f"{ptype}{pname}, "
            else:
                intro += f"{ptype} {pname}, "
            pnames += f"{pname}, "
        intro = intro[:-2]
        pnames = pnames[:-2]
        intro += ") {"
        mock.append(intro)

        # Add body
        mock.append(f"    {fdict['returns']} result;")
        mock.append( "")
        mock.append( "    switch (MOCK_GET_TYPE(" + name + ")) {")
        mock.append( "        case CMOCKA_MOCK_ENABLED_WITH_FUNC:")
        mock.append(f"            result = MOCK_FUNC_WITH({name})({pnames});")
        mock.append( "            break;")
        mock.append( "        case CMOCKA_MOCK_ENABLED:")
        for param in fdict['param']:
            pname = param['name']
            ptype = param['type']
            if "*" in ptype:
                mock.append(f"            check_expected_ptr({pname});")
            else:
                mock.append(f"            check_expected({pname});")
        mock.append(f"            result = mock_type({fdict['returns']});")
        mock.append( "            break;")
        mock.append( "        default:")
        mock.append(f"            result = MOCK_FUNC_REAL({name})({pnames});")
        mock.append( "            break;")
        mock.append( "    }")
        mock.append( "")
        mock.append( "    return result;")
        mock.append( "}")
        mock.append( "")

    mock.append("")
    return mock

def create_mock_header(source_folder, func_dict):
    """Create mock header"""

    cname = os.path.basename(source_folder).upper()

    mock = []

    mock.append("// SPDX-License-Identifier: MIT")
    mock.append("#pragma once")
    mock.append("")
    mock.append("#include <cmocka_extensions/cmocka_extensions.h>")
    mock.append("#include <cmocka_extensions/mock_extensions.h>")
    mock.append(f"#ifdef {cname}_FUNC_LINKER_WRAPPING")
    mock.append("#include <cmocka_extensions/mock_func_wrap.h>")
    mock.append("#else")
    mock.append("#include <cmocka_extensions/mock_func_weak.h>")
    mock.append("#endif")
    mock.append("")

    append_include_list(mock, source_folder, func_dict)

    for _, fdict in func_dict.items():
        name = fdict['name']
        # Add intro
        intro = f"MOCK_FUNC_PROTOTYPE({name}, {fdict['returns']}, "
        for param in fdict['param']:
            pname = param['name']
            ptype = param['type']
            if ptype.endswith("*"):
                intro += f"{ptype}{pname}, "
            else:
                intro += f"{ptype} {pname}, "
        intro = intro[:-2]
        intro += ")"
        mock.append(intro)

    mock.append("")
    return mock



def main():
    """Main function"""

    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <component_folder> <mock_folder>")
        sys.exit(0)

    source_folder = sys.argv[1]
    mock_folder = sys.argv[2]

    if os.path.exists(source_folder) is False:
        print(f"ERROR: Source folder '{source_folder}' does not exist!")
        sys.exit(-1)

    # Get all functions from the specified folder (e.g. ./src/component/event)
    print("Generate function dictionary...")
    func_dict = ast_parser.generate_func_dict(source_folder)

    # Remove static functions from the list
    sfunc_list = [x for x in func_dict if x.startswith("_")]
    for sfunc in sfunc_list:
        del func_dict[sfunc]

    print("Generate mock sources...")
    mock_source = create_mock_source(source_folder, func_dict)
    mock_header = create_mock_header(source_folder, func_dict)


    print("Write mock sources...")
    os.makedirs(mock_folder, exist_ok=True)

    cname = os.path.basename(source_folder).lower()
    mock_file = os.path.join(mock_folder, f"mock_{cname}")

    with open(mock_file + ".h", mode="w+", encoding="utf-8") as fh:
        print(f"Write {fh.name} ...")
        fh.write("\n".join(mock_header))
        fh.close()

    with open(mock_file + ".c", mode="w+", encoding="utf-8") as fh:
        print(f"Write {fh.name} ...")
        fh.write("\n".join(mock_source))
        fh.close()

    sys.exit(0)

if __name__ == "__main__":
    main()

