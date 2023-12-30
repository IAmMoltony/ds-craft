#!/bin/python3
"""
lstodos.py - script for listing TODOs in the code
"""

import os
import re


def find_files_pattern(files, directory, bad_end_patterns, good_end_patterns):
    """
    Find files in files list by patterns

    files - list of files to search in
    directory - directory where the files are in
    bad_end_patterns - if a file ends with one of the patterns from this list it will not be added
    good_end_patterns - if a file ends with one of the patterns from this list it will be added
    """
    good_files = []
    for file in files:
        bad_pass = True
        for bad_pattern in bad_end_patterns:
            if file.endswith(bad_pattern):
                bad_pass = False
                break
        if not bad_pass:
            continue

        good_pass = False
        for good_pattern in good_end_patterns:
            if file.endswith(good_pattern):
                good_pass = True
                break
        if not good_pass:
            continue

        good_files.append(f"{directory}/{file}")
    return good_files


def add_files_pattern(
    files_list, files, directory, bad_end_patterns, good_end_patterns
):
    """
    Add files into a list using good and bad end patterns

    Parameters:
    files_list - list where files that match the patterns will be added
    files - list of files to add into the files list
    directory - directory where the files are in
    bad_end_patterns - if a file ends with one of the patterns from this list it will not be added
    good_end_patterns - if a file ends with one of the patterns from this list it will be added
    """
    good = find_files_pattern(files, directory, bad_end_patterns, good_end_patterns)
    for good_file in good:
        files_list.append(good_file)


def find_files(files):
    """Find files with TODOs and put them into the files list"""
    files.append("README.md")
    add_files_pattern(files, os.listdir("source"), "source", [".swp"], [".c", ".cpp"])
    add_files_pattern(files, os.listdir("include"), "include", [".swp"], [".h", ".hpp"])
    add_files_pattern(files, os.listdir("scripts"), "scripts", [".swp"], [".py"])


def find_todo_patterns(files):
    """Find and print TODO patterns in the list of files"""
    for file_name in files:
        if (
            "lstodos.py" in file_name or "strencode.py" in file_name
        ):  # Idk why strencode.py fails (probably cuz it has Russian chars)
            continue
        with open(file_name, "r", encoding="utf-8") as file:
            lines = file.readlines()
            for line_num, line in enumerate(lines, start=1):
                matches = re.findall(r"@todo|TODO", line)
                if matches:
                    for _ in matches:
                        print(f"{file_name}:{line_num}: {line.strip()}")


def main():
    """
    Main function
    """
    files = []
    find_files(files)
    find_todo_patterns(files)


if __name__ == "__main__":
    main()
