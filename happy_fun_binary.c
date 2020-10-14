#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

const char sea_of_entropy[4096];
const char omae[] = "'omae wa mou shindeiru' 'NANI!?'";
const char p2_pass[] = "phase_2_secret_pass!!!!";
const long long fptrs[] = {0x316e6f6300000000, 0x2300000000000000, //phase_1 jmp2
    0x326e6f6300000000, 0x2300000000000000, 0x0000000000000000,
    0x336e6f6300000000, 0x2300000000000000}; // phase_1 jmp3

typedef void(*foyer);


void encrypt_vault() {

}

foyer decrypt_vault() {
    __asm__("NOP;\n\t");
    FILE* f;
    char c;
    char * fname = "halls_of_ballas.so";
    f = fopen(fname, "w");
    fwrite(sea_of_entropy, 1, sizeof(sea_of_entropy), f);
    fclose(f);
    void *handle = dlopen(fname, RTLD_NOW); // might need a ./?
    remove(fname);
    void (*foyer_ptr)() = dlsym(handle, "foyer");
    return foyer_ptr;
}

void vault()
{
    char input[64];
    puts(
    "You emerge from the other side of the gate, almost surprised that there were no nasty segfaults"
    " lying in wait. Instead, a sea of entropy lies before you, directly in front of which lies two functions."
    " The lower function appears decrepit and worn down, but still working.\n The other function appears almost"
    " Identical, as though it were a mirror of the other. However, where the main portion of the former function"
    " seems to be working as intended, this other function seems to have been hastily patched, as though to"
    " prevent others from using it. Perhaps its purpose can be divined through its twin...");
    fgets(input, 64, stdin);
    if (!strcmp(input, "Use the lower function\n")) {
        encrypt_vault();
    }
    if (!strcmp(input, "Use the upper function\n")) {
        decrypt_vault();
    }
}

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);

    size_t gate_value = 0x59454554; 
    char input[64];
    puts(
    "You step up to the entry point of the Binary, an ancient looking ruin composed of 0s and 1s.");
    fgets(input, 64, stdin);
    if (!strcmp(input, "approach the main entrance\n")) {
        puts("As you approach, ahead is a gate, but you quickly realize something is off.");
        while (1) {
            fgets(input, 64, stdin);
            if (!strcmp(input, "examine the gate\n")) {
                
                puts("You get closer to the gate, and quickly realize that its destination appears meaningless."
                " Rather than pointing further into the binary, it appears to be hastily constructed in the form of a crude joke."
                " On further inspection, it appears that the gate can easily be modified, if only you knew where it was"
                " supposed to go..."); 
            }
            if (!strcmp(input, "modify the gate\n")) {
                puts("What value do you place in the gate?");
                fgets(input, sizeof(size_t), stdin);
                gate_value = strtol(input, 0, 16);
            }
            if (!strcmp(input, "step through the gate\n")) {
                puts("You step through the gate, hoping that more than just a brick wall of zeroes lays beyond...");
                void (*gate)() = (void (*))gate_value;
                gate();
            }
            if (!strcmp(input, "leave\n")) {
                goto leave;
            }
        }
    }
    leave:
    if (!strcmp(input, "leave")) {
    puts(
    "You decide you've had enough and make your way back to where you came from, unsure of why you even bothered loading this binary in the first place.\n");
        exit(0);
    }

    FILE* f;
    char c;
    f = fopen("flag.txt", "r");
    c = fgetc(f);
    while(c != EOF){
        printf("%c", c);
        c = fgetc(f);
    }
    fclose(f);
	return 0;	
}


