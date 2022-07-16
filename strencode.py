# strencode.py
# String encoder from russian to what game can understand

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
}

def main():
    argv = sys.argv
    argc = len(argv)
    if argc < 2:
        print("Usage: python strencode.py <string in russian>")
        exit(1)
    
    newstr = ""
    str = " ".join(argv[1:])
    for c in str:
        newstr += letter_map[c]
    
    print(newstr)

if __name__ == "__main__":
    main()