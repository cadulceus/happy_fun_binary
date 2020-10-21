from collections import Counter
from decimal import *
def encode(lower, upper, string, ratios):
    total_range = upper - lower
    current_mapping = {}
    pointer = lower
    for char in ratios:
        current_mapping[char] = [pointer, pointer + ratios[char] * total_range]
        pointer = current_mapping[char][1]
    if len(string) == 1:
        return current_mapping[string]
    return encode(current_mapping[string[0]][0], current_mapping[string[0]][1], string[1:], ratios)

def decode(lower, upper, encoded, ratios):
    total_range = upper - lower
    current_mapping = {}
    pointer = lower
    previous = ""
    for char in ratios:
        current_mapping[char] = pointer
        pointer = pointer + ratios[char] * total_range
        if current_mapping[char] > encoded:
            if previous == '}':
                return previous
            else:
                return previous + decode(current_mapping[previous], current_mapping[char], encoded, ratios)  
        previous = char
    return previous + decode(current_mapping[previous], upper, encoded, ratios)

def calculate_ratios(string):
    ratios = Counter([char for char in string])
    total = Decimal(sum(ratios.values()))
    for char in ratios:
        ratios[char] = ratios[char] / total
    return ratios

flag = "beep_beeep_bbbeeep_beeeeppppp_bep_bep}"
ratios = calculate_ratios(flag)
encoded_flag_range = encode(0, 1, flag, ratios)
encoded_flag = encoded_flag_range[0] + ((encoded_flag_range[1] - encoded_flag_range[0]) / 2)
print decode(0, 1, encoded_flag, ratios), encoded_flag

for key in ratios:
    print "{.c = %s, .ratio = %ff}, //%s" % (hex(ord(key)), ratios[key], key)