#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

unsigned char sea_of_entropy[] = "yeetyeet";

typedef void(*foyer);
size_t gate_offset = 0x59454554;

void encrypt(char *plaintext, size_t plaintext_len, char IV, unsigned char *key)
{
    unsigned char current_block = IV;
    for (int byte_ind = 0; byte_ind < plaintext_len; byte_ind++)
    {
        printf("Using %2x as key byte, index %i\n",
               ((unsigned char *)key)[current_block % sizeof(size_t)], current_block % sizeof(size_t));
        current_block = current_block ^ key[current_block % sizeof(size_t)];
        plaintext[byte_ind] = (plaintext[byte_ind] + (current_block * current_block));
        current_block = plaintext[byte_ind];
    }
}

void decrypt(char *ciphertext, size_t plaintext_len, char IV, unsigned char *key)
{
    unsigned char current_block = IV;
    unsigned char unmodified_ciphertext;
    for (int byte_ind = 0; byte_ind < plaintext_len; byte_ind++)
    {
        printf("Using %2x as key byte, index %i\n",
               ((unsigned char *)key)[current_block % sizeof(size_t)], current_block % sizeof(size_t));
        unmodified_ciphertext = ciphertext[byte_ind];
        current_block = current_block ^ key[current_block % sizeof(size_t)];
        ciphertext[byte_ind] = ciphertext[byte_ind] - (current_block * current_block);
        current_block = unmodified_ciphertext;
    }
}

void vault_gate()
{
    __asm__("NOP;\n\t");
    FILE *f;
    unsigned char c;
    unsigned char *fname = "halls_of_ballas.so";
    f = fopen(fname, "w");
    fwrite(sea_of_entropy, 1, sizeof(sea_of_entropy), f);
    fclose(f);
    void *handle = dlopen("./halls_of_ballas.so", RTLD_NOW);
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
        " seems to be working as intended, this other function seems to have been hastily patched, as though to"
        " prevent others from using it. Perhaps its purpose can be divined through its twin...\n");

    char IV = (char)(rand() & 0xFF);
    while (1)
    {
        fgets(input, 64, stdin);
        if (!strcmp(input, "use the lower function\n"))
        {
            for (int i = 0; i < 8; i++)
            {
                printf("%2x", sea_of_entropy[i]);
            }
            printf("\nIV: %2x\n", IV);
            encrypt(sea_of_entropy, sizeof(sea_of_entropy), IV, (unsigned char *)&gate_offset);
            puts("You run the lower function, and watch as the sea of bytes folds and turns upon itself"
                 " Before long it looks nothing like it did milliseconds ago.\n");
            for (int i = 0; i < 8; i++)
            {
                printf("%2x", sea_of_entropy[i]);
            }
        }
        if (!strcmp(input, "use the upper function\n"))
        {
            for (int i = 0; i < 8; i++)
            {
                printf("%2x", sea_of_entropy[i]);
            }
            printf("\nIV: %2x\n", IV);
            decrypt(sea_of_entropy, sizeof(sea_of_entropy), IV, (unsigned char *)&gate_offset);
            puts("You run the upper function, watching for the sea of bytes to shift around into"
                 " what you hope is a meaningful pattern\n");
            for (int i = 0; i < 8; i++)
            {
                printf("%2x", sea_of_entropy[i]);
            }
        }
        if (!strcmp(input, "use the gate\n"))
        {
            puts("Gathering together your registers, you activate the gate, hoping any modifications"
                 " you made will help you avoid a painful segmentation fault\n");
            vault_gate();
        }
    }
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    char input[64];
    puts(
        "You step up to the entry point of the Binary, an ancient looking ruin composed of 0s and 1s.");
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
                     " supposed to go...");
            }
            if (!strcmp(input, "modify the gate\n"))
            {
                puts("What value do you place in the gate?");
                fgets(input, sizeof(size_t), stdin);
                gate_offset = strtol(input, 0, 16);
                gate_value = (((size_t)main >> 12) << 12) + gate_offset;
                puts("You place the new value into the gate. It... looks right? Probably?");
            }
            if (!strcmp(input, "step through the gate\n"))
            {
                puts("You step through the gate, hoping that more than just a brick wall of zeroes lays beyond...");
                void (*gate)() = (void(*))gate_value;
                gate();
            }
            if (!strcmp(input, "leave\n"))
            {
                goto leave;
            }
        }
    }
leave:
    if (!strcmp(input, "leave"))
    {
        puts(
            "You decide you've had enough and make your way back to where you came from, unsure of why you even bothered loading this binary in the first place.\n");
        exit(0);
    }

    FILE *f;
    char c;
    f = fopen("flag.txt", "r");
    c = fgetc(f);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(f);
    }
    fclose(f);
    return 0;
}
