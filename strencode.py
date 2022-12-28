# strencode.py
# String encoder from russian to what game can understand
# Usage python strencode.py <string in russian> or --file <file to convert>

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
    "ы": "\"",
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
    print("Usage: python strencode.py <string to convert> or --file <file to convert>")

def encode(str_ru: str):
    newstr = ""
    for c in str_ru:
        newstr += letter_map[c]
    return newstr

def main():
    argv = sys.argv
    argc = len(argv)
    if argc < 2:
        print_usage()
        exit(1)

    content = ''
    if argv[1] == '--file':
        with open(argv[2], 'r', encoding='utf-8') as f:
            content = f.read()
    else:
        content = " ".join(argv[1:])

    print(encode(content))

if __name__ == "__main__":
    main()
