#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
int mem_ __attribute__ ((section ("fixedloc")));

// must be initialized to something or it goes to .bss instead of .data
unsigned char sea_of_entropy[BOB_SZ] = "yeeeeeeet";

typedef void(*foyer);
size_t gate_offset = 0x59454554;

void encrypt(char *plaintext, size_t plaintext_len, char IV, unsigned char *key)
{
    unsigned char current_block = IV;
    for (int byte_ind = 0; byte_ind < plaintext_len; byte_ind++)
    {
        current_block = current_block ^ key[current_block % sizeof(size_t)];
        plaintext[byte_ind] = (plaintext[byte_ind] + (current_block * current_block));
        current_block = plaintext[byte_ind];
    }
}

void decrypt(char *ciphertext, size_t plaintext_len, char IV, unsigned char *key)
{
#ifdef DEBUG
    unsigned char current_block = IV;
    unsigned char unmodified_ciphertext;
    for (int byte_ind = 0; byte_ind < plaintext_len; byte_ind++)
    {
        unmodified_ciphertext = ciphertext[byte_ind];
        current_block = current_block ^ key[current_block % sizeof(size_t)];
        ciphertext[byte_ind] = ciphertext[byte_ind] - (current_block * current_block);
        current_block = unmodified_ciphertext;
    }
    for (int i = 0; i < 64; i++)
    {
        printf("%02x", ciphertext[i]);
    }
#endif
}

void vault_gate()
{
    __asm__("NOP;\n\t");
    FILE *f;
    unsigned char c;
    unsigned char *fname = "binary_of_ballas.so";
    f = fopen(fname, "w");
    fwrite(sea_of_entropy, 1, sizeof(sea_of_entropy), f);
    fclose(f);
    void *handle = dlopen("./binary_of_ballas.so", RTLD_NOW);
    remove(fname);
    void (*foyer_ptr)() = dlsym(handle, "foyer");
    foyer_ptr();
}

void vault()
{
    char input[64];
    puts(
        "You emerge from the other side of the gate, almost surprised that there were no nasty segfaults"
        " lying in wait. Instead, a sea of entropy lies before you, directly in front of which lies two functions, "
        " As well as another inactive gate. At a glance, the gate appears a little more sophisticated than the last,"
        " as if it were capable of locking on to some sort of symbol buried somewhere in the expanse of meaningless data."
        " The lower function appears decrepit and worn down, but still working.\n The other function appears almost"
        " Identical, as though it were a mirror of the other. However, where the main portion of the former function"
        " seems to be working as intended, the contents of the other function seems to have been left out entirely, as though to"
        " prevent others from using it. Both functions appear to take a number of parameters, one of which is easily"
        " modifiable.\n");

    char IV = (char)(rand() & 0xFF);
    u_int32_t key = gate_offset * gate_offset * gate_offset;
    while (1)
    {
        fgets(input, 64, stdin);
        if (!strcmp(input, "modify the input\n"))
        {
            puts("What would you like to change the byte to?\n");
            IV = fgetc(stdin);
            puts("you modify the input, hoping it will be more useful than the random value it held before.\n");
        }
        if (!strcmp(input, "use the lower function\n"))
        {
            encrypt(sea_of_entropy, sizeof(sea_of_entropy), IV, (unsigned char *)&key);
            puts("You run the lower function, and watch as the sea of bytes folds and turns upon itself."
                 " Before long it looks nothing like it did milliseconds ago.\n");
        }
        if (!strcmp(input, "use the upper function\n"))
        {
            decrypt(sea_of_entropy, sizeof(sea_of_entropy), IV, (unsigned char *)&key);
            puts("You run the upper function. it didn't do much.\n");
        }
        if (!strcmp(input, "use the gate\n"))
        {
            puts("Gathering together your registers, you activate the gate, hoping any modifications"
                 " you made will help you avoid a painful segmentation fault\n");
            vault_gate();
        }
    }
}

void checker_2(char *byte)
{
}

int dummy_val = 1337;
void inline change_32_to_64()
{
    if (dummy_val != 1337)
    {
        dummy_val = 31337;
        return;
    }
    __asm__("jmp 0x33:0x31706d6a\n\t");
}


void inline change_64_to_32()
{
    __asm__("jmp 0x33:0x31706d6a\n\t");
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    char input[64];
    puts(
        "You step up to the entry point of the Binary, an ancient looking ruin composed of 0s and 1s.\n");
    fgets(input, 64, stdin);
    if (!strcmp(input, "approach the main entrance\n"))
    {
        puts("As you approach, ahead is a gate, but you quickly realize something is off.");
        while (1)
        {

            size_t gate_value;
            fgets(input, 64, stdin);
            if (!strcmp(input, "examine the gate\n"))
            {
                puts("You get closer to the gate, and quickly realize that its destination appears meaningless."
                     " Rather than pointing further into the binary, it appears to be hastily constructed in the form of a crude joke."
                     " On further inspection, it appears that the gate can easily be modified, if only you knew where it was"
                     " supposed to go...\n");
            }
            if (!strcmp(input, "modify the gate\n"))
            {
                puts("What value do you place in the gate?\n");
                fgets(input, sizeof(size_t), stdin);
                gate_offset = strtol(input, 0, 16);
                gate_value = (((size_t)main >> 12) << 12) + gate_offset;
                puts("You place the new value into the gate. It... looks right? Probably?\n");
            }
            if (!strcmp(input, "step through the gate\n"))
            {
                puts("You step through the gate, hoping that more than just a brick wall of zeroes lays beyond...\n");
                void (*gate)() = (void(*))gate_value;
                gate();
            }
            if (!strcmp(input, "leave\n"))
            {
                break;
            }
        }
    }
    puts(
        "You decide you've had enough and make your way back to where you came from, unsure of why you even bothered loading this binary in the first place.\n");
}
