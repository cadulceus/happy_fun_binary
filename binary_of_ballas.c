#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

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

#ifdef DEBUG
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
#endif

void chal_1()
{
    struct symbol_range mapping[(sizeof(ratios) / sizeof(struct symbol_ratio))];

    long double pointer = 0;
    char input[64];
    for (int i = 0; i < sizeof(mapping) / sizeof(struct symbol_range); i++)
    {
        mapping[i].c = ratios[i].c;
        mapping[i].lower = pointer;
        pointer = pointer + ratios[i].ratio;
        mapping[i].upper = pointer;
    }
    union
    {
        long double d;
        char c[sizeof(long double)];
    } flag_float = {.c = "\xb3\x35\xdf\xb8\x80\xf5\x28\xd0\xfd\x3f\x00\x00"};

    char flag_str[64] = "flag{beep_this_is_not_the_flag}";

    printf("And emerge in a square shaped room."
           " Across the walls you see a number of structures inscribed into the binary,"
           " and at the room's center lies a capsule atop a pedestal. To the left of the pedestal"
           " is a function that appears to feed directly into the pedestal. Below the function"
           " is an input string that appears to feed into it. The input string currently contains \"%s\".",
           flag_str);

    while (1)
    {
        fgets(input, 64, stdin);
        if (!strcmp(input, "examine the capsule\n"))
        {
            printf("Floating in the center of the capsule is a number that after a "
                   " moment of examination seems to approximate %Lf",
                   flag_float.d);
        }

        if (!strcmp(input, "modify the flag\n"))
        {
            puts("What would you like to change the flag to?");
            fgets(input, 64, stdin);
            strcpy(flag_str, input);
            puts("You change the input flag, hoping to gain more insight into the function's"
                 " behavior the next time you run it.");
        }

        if (!strcmp(input, "execute the function\n"))
        {
            puts(flag_str + 5);
            flag_float.d = encode(0, 1, flag_str + 5,
                                  mapping, (sizeof(ratios) / sizeof(struct symbol_ratio)));
            printf("As you call the function, you see the structures written across the wall light up"
                   " as they are read into the function, along with the flag value and some odd constants."
                   " In a few milliseconds, the function completes, and the value within the capsule changes to"
                   " match the output, %Lf",
                   flag_float.d);
        }

        if (!strcmp(input, "go back\n"))
        {
            printf("You decide you've done all you can here, and return to foyer.");
            return;
        }

        // currently encoded flag: beep_beeep_bbbeeep_beeeeppppp}
#ifdef DEBUG
        char decoded_flag[64] = "flag{";
        if (!strcmp(input, "decode flag\n"))
        {
            for (int i = 0; i < sizeof(long double); i++)
            {
                printf("%02x", ((unsigned char *)&flag_float.d)[i]);
            }
            printf("\n%Lf\n", flag_float.d);
            decode(0, 1, flag_float.d, decoded_flag, mapping, (sizeof(ratios) / sizeof(struct symbol_ratio)));
            puts(decoded_flag);
        }
#endif
    }
}

int dummy = 420;
int seed = 31337;

void __attribute__((constructor)) test()
{
    while (1)
    {
        return;
    }
}

int c1_ind = 0;
int checker_1(char c) // maps happy_fun_binary to 0x180000, contains offsets to flag characters in the binary
{
    int offsets[] = CHECKER_1;
    int valid_char = 0;
    int char_cast = (int)c;
    int current_offset = offsets[c1_ind];
    if (c1_ind >= sizeof(offsets) / sizeof(int))
    {
        return valid_char;
    }

    __asm__("mov edi, %[current_offset]\n\t"
            "mov esi, %[char_cast]\n\t" : : [char_cast] "r" (char_cast), [current_offset] "r" (current_offset));
    // identifier for patching routine
    __asm__(".byte 0x01");
    __asm__(C1_NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
    __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char));
    c1_ind++;
    printf("success: %i\n", valid_char);
    return valid_char;
}

int c2_ind = 0;
int checker_2(char c) // input char + 1 xor 2nd byte of happy_fun_binary
{
    char xored[] = CHECKER_2;
    int valid_char = 0;
    int char_cast = (int)c;
    int current_char_casted = (int) xored[c2_ind];
    
    if (c2_ind >= sizeof(xored))
    {
        return valid_char;
    }
    puts("starting checker2");

    __asm__("mov edi, %[current_char_casted]\n\t"
            "mov esi, %[char_cast]\n\t" 
            "mov ecx, %[c2_ind]\n\t"
            : : [char_cast] "r" (char_cast), [current_char_casted] "r" (current_char_casted), [c2_ind] "r" (c2_ind));
    // identifier for patching routine
    __asm__(".byte 0x02");
    __asm__(C2_NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
    __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char) : : "edi");
    c2_ind++;
    printf("success: %i\n", valid_char);
    return valid_char;
}

// int c2_ind = 0;
// int checker_3(char c)
// {
//     char xored[] = CHECKER_2;
//     int valid_char = 0;
//     int char_cast = (int)c;
//     int current_char_casted = (int) xored[c2_ind];
    
//     if (c2_ind >= sizeof(xored))
//     {
//         return valid_char;
//     }

//     __asm__("mov edi, %[current_char_casted]\n\t"
//             "mov esi, %[char_cast]\n\t" 
//             "mov ecx, %[c2_ind]\n\t"
//             : : [char_cast] "r" (char_cast), [current_char_casted] "r" (current_char_casted), [c2_ind] "r" (c2_ind));
//     // identifier for patching routine
//     __asm__(".byte 0x03");
//     __asm__(C2_NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
//     __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char));
//     c2_ind++;
//     printf("success: %i\n", valid_char);
//     return valid_char;
// }

void chal_2()
{
    // flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}
    char input[64];
    puts("You emerge\n");
    fgets(input, 64, stdin);
    srand(seed);
    checker_1('f');
    for (int i = 0; i < strlen(input); i++)
    {
        srand(dummy);
        switch (rand() % 3)
        {
        case 0:
        case 1:
        case 2:
            checker_2(input[i]);
        }

        //puts(strerror(errno));
        //fgets(input, 64, stdin);
    }
}

void foyer(void *happy_fun_binary_ptr)
{
    char *prologue_flag = "flag{yes_this_is_all_one_big_critical_role_reference}";
    char input[64];
    puts("You emerge into a grand and extravegant foyer. While sparsely furnished, intricately crafted code decorates every square"
         " inch of the walls and ceiling. In the center of the room lies a grand structure, carved into which"
         " are three slots. The three slots feed into a function that seems cryptographic in nature."
         " On the far side of the room lies 2 semi-circular doorways leading into darkness.\n");

    while (1)
    {
        fgets(input, 64, stdin);
        if (!strcmp(input, "examine the doormat\n"))
        {
            printf("You look down, and are surprised to see a welcome mat beneath your feet. On it reads"
                   " %s. That was easy!\n",
                   prologue_flag);
        }

        if (!strcmp(input, "examine the cryptographic function\n"))
        {
            puts("On closer inspection, you realize that its not yet implemented. oops!\n");
        }

        if (!strcmp(input, "enter the first doorway\n"))
        {
            puts("You step through the doorway...\n");
            chal_1();
        }

        if (!strcmp(input, "enter the second doorway\n"))
        {
            puts("You step through the doorway...\n");
            chal_2();
        }
    }
}