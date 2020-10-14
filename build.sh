gcc -c -Wall -fpic -Wl,-E -m32 halls_of_ballas.c
gcc -shared -m32 -o halls_of_ballas.so halls_of_ballas.o
gcc happy_fun_binary.c -masm=intel -m32 -ldl -o happy_fun_binary