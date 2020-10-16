from collections import Counter
def encode_impl(lower, upper, string, ratios):
    total_range = upper - lower
    current_mapping = {}
    pointer = lower
    for char in ratios:
        current_mapping[char] = [pointer, pointer + ratios[char] * total_range]
        pointer = current_mapping[char][1]
    if len(string) == 1:
        print current_mapping[string]
        return current_mapping[string]
    return encode_impl(current_mapping[string[0]][0], current_mapping[string[0]][1], string[1:], ratios)

def encode():
    flag = "beep_beep_lettuce}"#"banana_pear_bookkeeper_naboo}"
    mapping = Counter([char for char in flag])
    total = float(sum(mapping.values()))
    for char in mapping:
        mapping[char] = mapping[char] / total
    encode_impl(0, 1, flag, mapping)

#def decode():

encode()