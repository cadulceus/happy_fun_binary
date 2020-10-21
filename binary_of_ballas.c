#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol_range
{
    char c;
    long double lower;
    long double upper;
};

struct symbol_ratio
{
    char c;
    long double ratio;
};

struct symbol_ratio ratios[] = {
{.c = 0x70, .ratio = 0.263158f}, //p
{.c = 0x62, .ratio = 0.210526f}, //b
{.c = 0x65, .ratio = 0.368421f}, //e
{.c = 0x7d, .ratio = 0.026316f}, //}
{.c = 0x5f, .ratio = 0.131579f}, //_
};

struct symbol_range *find_range_for_char(char c, struct symbol_range *range_array, size_t array_len)
{
    for (int i = 0; i < array_len; i++)
    {
        if (c == range_array[i].c)
        {
            return &range_array[i];
        }
    }
    return NULL;
}

long double shortest_valid_double(long double lower, long double upper)
{
    char lower_str[20];
    char upper_str[20];

    const int MAX_PRECISION = 16;

    gcvt(lower, MAX_PRECISION, lower_str);
    gcvt(lower, MAX_PRECISION, upper_str);

    for (int i = 0; i < MAX_PRECISION; i++)
    {
        if (lower_str[i] != upper_str[i])
        {
            upper_str[i + 1] = 0;
            return strtod(upper_str, NULL);
        }
    }
    return upper;
}

long double encode(long double lower, long double upper, char *string, struct symbol_range *current_mapping, size_t mapping_len)
{
    long double total_range = upper - lower;
    long double pointer = lower;
    for (int i = 0; i < sizeof(ratios) / sizeof(struct symbol_ratio); i++)
    {
        current_mapping[i].lower = pointer;
        current_mapping[i].upper = pointer + (ratios[i].ratio * total_range);
        pointer = current_mapping[i].upper;
    }
    char current_char = string[0];
    struct symbol_range *char_range = find_range_for_char(current_char, current_mapping, mapping_len);
    if (strlen(string) == 1)
    {
        return char_range->lower + ((char_range->upper - char_range->lower) / 2);
    }
    return encode(char_range->lower, char_range->upper, string + 1, current_mapping, mapping_len);
}

void decode(long double lower, long double upper, long double encoded_float, char *decoded_buf, struct symbol_range *current_mapping, size_t mapping_len)
{
    long double total_range = upper - lower;
    long double pointer = lower;
    for (int i = 0; i < sizeof(ratios) / sizeof(struct symbol_ratio); i++)
    {
        current_mapping[i].lower = pointer;
        current_mapping[i].upper = pointer + ratios[i].ratio * total_range;
        pointer = current_mapping[i].upper;

        if (current_mapping[i].upper > encoded_float && encoded_float > current_mapping[i].lower)
        {
            decoded_buf[strlen(decoded_buf)] = current_mapping[i].c;
            if (current_mapping[i].c != '}')
            {
                decode(current_mapping[i].lower, current_mapping[i].upper, encoded_float, decoded_buf, current_mapping, mapping_len);
            }
            return;
        }
    }
    return;
}

void chal_1()
{
    struct symbol_range mapping[(sizeof(ratios) / sizeof(struct symbol_ratio))];

    long double pointer = 0;
    for (int i = 0; i < sizeof(mapping) / sizeof(struct symbol_range); i++)
    {
        mapping[i].c = ratios[i].c;
        mapping[i].lower = pointer;
        pointer = pointer + ratios[i].ratio;
        mapping[i].upper = pointer;
        // printf("%c lower: %Lf upper: %Lf\n", mapping[i].c, mapping[i].lower, mapping[i].upper);
    }

    // double flag = 0.4065626736106059008675764359;
    char flag_str[64] = "flag{";
    long double flag_float = encode(0, 1, "beep_beeep_bbbeeep_beeeeppppp}", mapping, (sizeof(ratios) / sizeof(struct symbol_ratio)));
    for (int i = 0; i < sizeof(long double); i++) {
        printf("%02x", ((unsigned char *) &flag_float)[i]);
    }
    printf("\n\n%li", (long) flag_float);
    char *arr = "\xb3\x35\xdf\xb8\x80\xf5\x28\xd0\xfd\x3f\x00\x00";
    printf("\nlol %.17Lg", (long double) *arr);
    decode(0, 1, flag_float, flag_str, mapping, (sizeof(ratios) / sizeof(struct symbol_ratio)));
    puts(flag_str);
}

void foyer()
{
    puts("yeet");
    chal_1();
}