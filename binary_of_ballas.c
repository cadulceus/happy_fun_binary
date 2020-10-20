#include <stdio.h>


struct symbol_range {
    char c;
    float lower;
    float upper;
};

struct symbol_ratio {
    char c;
    float ratio;
};

struct symbol_ratio ratios[] = {
    {.c = 0x65, .ratio = 0.3} // a
};

float encode(float lower, float upper, char *string,
            struct symbol_ratio *ratios, struct symbol_range *current_mapping)
{
    float total_range = upper - lower;
    float pointer = lower;
    for (int i = 0; i < sizeof(ratios) / sizeof(struct symbol_range); i++) {
        current_mapping[i].lower = pointer;
         current_mapping[i].upper = pointer + ratios[i].ratio * total_range;
        pointer = current_mapping[i].upper;
    }
    if (strlen(string) == 1) {
        return current_mapping[string];
    }
    return encode(current_mapping[string[0]][0], current_mapping[string[0]][1], string[1:], ratios)
}

v

def decode(lower, upper, encoded, ratios):
    total_range = upper - lower
    current_mapping = {}
    pointer = lower
    previous = ""
    for char in ratios:
        current_mapping[char] = pointer
        pointer = pointer + ratios[char] * total_range
        if current_mapping[char] > encoded:
            print previous
            if previous == '}':
                return previous
            else:
                return previous + decode(current_mapping[previous], current_mapping[char], encoded, ratios)  
        previous = char
    print previous
    return previous + decode(current_mapping[previous], upper, encoded, ratios)

def calculate_ratios(string):
    ratios = Counter([char for char in string])
    total = float(sum(ratios.values()))
    for char in ratios:
        ratios[char] = ratios[char] / total
    return ratios

void foyer() {
    puts("yeet");
}