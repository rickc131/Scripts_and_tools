import random
import sys


def random_xor():
    chars = [chr(random_num) for random_num in range(0, 126)]
    num1 = random.randint(32, 125)
    num2 = random.randint(32, 125)
    return chars[num1], chars[num2]


def xor_for_str(xor_result):
    xor_result = ord(xor_result)  # 将原始字符转换为ASCII码
    char1, char2 = random_xor()
    xor = ord(char1) ^ ord(char2)
    while xor != xor_result:
        char1, char2 = random_xor()
        xor = ord(char1) ^ ord(char2)
    return f"(\"{char1}\"^\"{char2}\")."


if __name__ == "__main__":
    if len(sys.argv) > 1:
        for i in sys.argv[1]:
            print(xor_for_str(i), end='')
    else:
        print("Usage: python3 main.py assert")
