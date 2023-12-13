#!/bin/python3

import os
import re


def find_todo_patterns(files):
    for file_name in files:
        if (
            "lstodos.py" in file_name or "strencode.py" in file_name
        ):  # Idk why strencode.py fails (probably cuz it has Russian chars)
            continue
        with open(file_name, "r") as file:
            lines = file.readlines()
            for line_num, line in enumerate(lines, start=1):
                matches = re.findall(r"@todo|TODO", line)
                if matches:
                    for _ in matches:
                        print(f"{file_name}:{line_num}: {line.strip()}")


def main():
    files = ["README.md"]
    for file in os.listdir("source"):
        if not file.endswith(".swp") and (file.endswith(".c") or file.endswith(".cpp")):
            files.append(f"source/{file}")
    for file in os.listdir("include"):
        if not file.endswith(".swp") and (file.endswith(".h") or file.endswith(".hpp")):
            files.append(f"include/{file}")
    for file in os.listdir("scripts"):
        if not file.endswith(".swp") and file.endswith(".py"):
            files.append(f"scripts/{file}")
    find_todo_patterns(files)


if __name__ == "__main__":
    main()
