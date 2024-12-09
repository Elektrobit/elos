import json
import hashlib
import argparse


def parse_robocop_output(robocop_file, output_file):
    with open(robocop_file, "r") as f:
        robocop_output = f.readlines()

    issues = []
    for line in robocop_output:
        try:
            parts = line.strip().split(" ")
            if len(parts) < 4:
                continue

            location = parts[0].strip().split(":")
            file_path = location[0].strip()
            line_number = int(location[1].strip())
            column_number = int(location[2].strip())
            severity = parts[1]
            rule_name = parts[2]
            description = " ".join(parts[3:]).strip()

            severity_map = {"I": "minor", "W": "major", "E": "critical"}
            gitlab_severity = severity_map.get(severity[1], "unknown")

            fingerprint = hashlib.md5(
                f"{file_path}:{line_number}:{description}".encode()
            ).hexdigest()

            issues.append({
                "description": f"{rule_name}: {description}",
                "fingerprint": fingerprint,
                "severity": gitlab_severity,
                "location": {
                    "path": file_path,
                    "lines": {
                        "begin": line_number,
                        "end": line_number,
                        "column": column_number
                    }
                }
            })
        except ValueError:
            print(f"Failed to parse line: {line}")
            continue

    with open(output_file, "w") as f:
        json.dump(issues, f, indent=4)


def arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input',
                        dest='input',
                        help="the robocop log file using the default format"
                        )
    parser.add_argument('-o', '--output',
                        dest='output',
                        help="the path to the out put file in json format ")
    return parser.parse_args()


if __name__ == '__main__':
    args = arguments()
    parse_robocop_output(args.input, args.output)
