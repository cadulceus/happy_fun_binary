gcc -c -Wall -fpic -Wl,-E -m32 binary_of_ballas.c
gcc -shared -m32 -o binary_of_ballas.so binary_of_ballas.o
gcc happy_fun_binary.c -masm=intel -m32 -ldl -o happy_fun_binary