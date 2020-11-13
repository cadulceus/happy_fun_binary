#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

//seed with dummy_val to determine func call order, dummy val should be modified to the correct value by debugger detection thread running 64 bit code
//func 1: generate 1000 bytes with a static rand() seed that changes if debugger detected, compare to pre-determined offsets
//func 2: simple byte by byte comparison in function that gets nopped out along with the nopping out function if debugger is detected
//func 3: reuse encryption function from happy_fun_binary on hardcoded char array
int dummy_val = 1337;

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

    __asm__("mov edi, %[current_char_casted]\n\t"
            "mov esi, %[char_cast]\n\t" 
            "mov ecx, %[c2_ind]\n\t"
            : : [char_cast] "r" (char_cast), [current_char_casted] "r" (current_char_casted), [c2_ind] "r" (c2_ind));
    // identifier for patching routine
    __asm__(".byte 0x02");
    __asm__(C2_NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
    __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char));
    c2_ind++;
    printf("success: %i\n", valid_char);
    strerror(errno);
    return valid_char;
}

int checker_3(char c)
{
    char xored[] = CHECKER_2;
    int valid_char = 0;
    int char_cast = (int)c;
    int current_char_casted = (int) xored[c2_ind];
    
    if (c2_ind >= sizeof(xored))
    {
        return valid_char;
    }

    __asm__("mov edi, %[current_char_casted]\n\t"
            "mov esi, %[char_cast]\n\t" 
            "mov ecx, %[c2_ind]\n\t"
            : : [char_cast] "r" (char_cast), [current_char_casted] "r" (current_char_casted), [c2_ind] "r" (c2_ind));
    // identifier for patching routine
    __asm__(".byte 0x03");
    __asm__(C2_NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
    __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char));
    c2_ind++;
    printf("success: %i\n", valid_char);
    return valid_char;
}

void chal_3()
{
    // flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}
    char input[64];
    puts("You emerge\n");
    fgets(input, 64, stdin);
    srand(31337);
    checker_1('f');
    for (int i = 0; i < strlen(input); i++)
    {
        srand(dummy_val);
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

int main()
{
    //int fd = open("./happy_fun_binary", O_RDWR, 0x0);
    //puts(strerror(errno));
    //mmap((void *)0x180000, 0x1000, 0x5, MAP_PRIVATE, fd, 0x0);
    //puts(strerror(errno));
    chal_3();
}