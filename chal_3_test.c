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

int ind = 0;
int checker_1(char c)
{
    int offsets[] = {0x43f, 0x1b5, 0x3eb, 0x3dd, 0x1631, 0x3b8, 0x4ea, 0x3eb, 0x1e7, 0x4ea, 0x1be, 0x1c2, 0x3d4, 0x3dd, 0x3eb, 0x136, 0x3da, 0x3d4, 0x1c2, 0x3b8, 0x1c3, 0x1bf, 0x1b5, 0xe8, 0x3d4, 0x1b7, 0x4ea, 0x3d4, 0x3db, 0x4ea, 0x1be, 0x3eb, 0x3f3, 0x4ea, 0xe8, 0x3d4, 0x136, 0x1c3, 0x3d4, 0x1a4, 0x1b5, 0x3eb, 0x1be, 0x3eb, 0x3db, 0x3d4, 0x1c2, 0x3b8, 0x4d4, 0x43f, 0x136, 0x3d4, 0x4d4, 0x3f3, 0x4de, 0x1de};
    int valid_char = 0;
    int char_cast = (int)c;
    int current_offset = offsets[ind];
    
    if (ind >= sizeof(offsets) / sizeof(int))
    {
        return valid_char;
    }

    __asm__("mov edi, %[current_offset]\n\t"
            "mov esi, %[char_cast]\n\t" : : [char_cast] "r" (char_cast), [current_offset] "r" (current_offset));
    __asm__(NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
    __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char));
    ind++;
    printf("success: %i\n", valid_char);
    return valid_char;
}

int checker_2(char c)
{
    int offsets[] = {0x43f, 0x1b5, 0x3eb, 0x3dd, 0x1631, 0x3b8, 0x4ea, 0x3eb, 0x1e7, 0x4ea, 0x1be, 0x1c2, 0x3d4, 0x3dd, 0x3eb, 0x136, 0x3da, 0x3d4, 0x1c2, 0x3b8, 0x1c3, 0x1bf, 0x1b5, 0xe8, 0x3d4, 0x1b7, 0x4ea, 0x3d4, 0x3db, 0x4ea, 0x1be, 0x3eb, 0x3f3, 0x4ea, 0xe8, 0x3d4, 0x136, 0x1c3, 0x3d4, 0x1a4, 0x1b5, 0x3eb, 0x1be, 0x3eb, 0x3db, 0x3d4, 0x1c2, 0x3b8, 0x4d4, 0x43f, 0x136, 0x3d4, 0x4d4, 0x3f3, 0x4de, 0x1de};
    int valid_char = 0;
    int char_cast = (int)c;
    int current_offset = offsets[ind];
    
    if (ind >= sizeof(offsets) / sizeof(int))
    {
        return valid_char;
    }

    __asm__("mov edi, %[current_offset]\n\t"
            "mov esi, %[char_cast]\n\t" : : [char_cast] "r" (char_cast), [current_offset] "r" (current_offset));
    __asm__(NOPS : : : "eax", "ebx", "ecx", "edx", "esi"); // to be patched by patcher to contain 64 bit code
    __asm__("mov %[valid_char], edi" : [valid_char] "=m" (valid_char));
    ind++;
    printf("success: %i\n", valid_char);
    return valid_char;
}

void chal_3()
{
    // flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}
    char input[64];
    puts("You emerge\n");
    fgets(input, 64, stdin);
    srand(32)
    for (int i = 0; i < rand(); i++)
    {
        srand(dummy_val);
        switch (rand() % 3)
        {
        case 0:
        case 1:
        case 2:
            checker_1(input[i]);
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