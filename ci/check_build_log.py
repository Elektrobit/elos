#!/usr/bin/env python3
"""
Checks build log for warnings and errors and displays a summary of them.
"""

import sys


print("#################################")

summary_lines = []
count_errors = 0
count_warnings = 0
f = open(sys.argv[1], "r")
lines = f.readlines()
f.close()
state_collect = False
for line in lines:
    if "warning:" in line:
        state_collect = True
        count_warnings += 1
        summary_lines.append(line)
    elif "error:" in line:
        state_collect = True
        count_errors += 1
        summary_lines.append(line)
    elif state_collect:
        if line[0] == " " or line[0] == "\t":
            summary_lines.append(line)
        else:
            state_collect = False

for line in summary_lines:
    print(line.strip())

print("Build finished with %i errors and %i warnings" % (count_errors, count_warnings))
