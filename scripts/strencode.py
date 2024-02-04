#!/bin/python3
"""
strencode - script for encoding a Russian string into format that the game understands
"""

import sys

letter_map = {
    "А": "A",
    "Б": "B",
    "В": "C",
    "Г": "D",
    "Д": "E",
    "Е": "F",
    "Ё": "G",
    "Ж": "H",
    "З": "I",
    "И": "J",
    "Й": "K",
    "К": "L",
    "Л": "M",
    "М": "N",
    "Н": "O",
    "О": "P",
    "П": "Q",
    "Р": "R",
    "С": "S",
    "Т": "T",
    "У": "U",
    "Ф": "V",
    "Х": "W",
    "Ц": "X",
    "Ч": "Y",
    "Ш": "Z",
    "Щ": "[",
    "Ъ": "\\",
    "Ы": "]",
    "Ь": "^",
    "Э": "_",
    "Ю": "`",
    "Я": "a",
    "а": "b",
    "б": "c",
    "в": "d",
    "г": "e",
    "д": "f",
    "е": "g",
    "ё": "h",
    "ж": "i",
    "з": "j",
    "и": "k",
    "й": "l",
    "к": "m",
    "л": "n",
    "м": "o",
    "н": "p",
    "о": "q",
    "п": "r",
    "р": "s",
    "с": "t",
    "т": "u",
    "у": "v",
    "ф": "w",
    "х": "x",
    "ц": "y",
    "ч": "z",
    "ш": "}",
    "щ": "~",
    "ъ": "!",
    "ы": '"',
    "ь": "#",
    "э": "$",
    "ю": "%",
    "я": "&",
    " ": " ",
    ".": ".",
    ":": ":",
    ",": "@",
    "\n": "\n",
    "1": "1",
    "2": "2",
    "3": "3",
    "4": "4",
    "5": "5",
    "6": "6",
    "7": "7",
    "8": "8",
    "9": "9",
    "0": "0",
}


def print_usage():
    """
    Print script usage
    """
    print("Usage: python strencode.py <string to convert>")


def encode(str_ru: str):
    """
    Encode a string

    Parameters:
    str_ru - Russian string to encode

    Returns:
    Encoded string
    """
    newstr = ""
    for c in str_ru:
        newstr += letter_map[c]
    return newstr


def main():
    """
    Main function
    """
    argv = sys.argv
    argc = len(argv)
    if argc < 2:
        print_usage()
        sys.exit(1)

    content = " ".join(argv[1:])
    print(encode(content))


if __name__ == "__main__":
    main()
