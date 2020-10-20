gcc -c -Wall -fpic -Wl,-E -m32 binary_of_ballas.c
gcc -shared -m32 -o binary_of_ballas.so binary_of_ballas.o
export BOB_SZ=$(wc -c < binary_of_ballas.so)
export SEA_OF_YEET=$(python -c "print 'e'*$BOB_SZ")
gcc happy_fun_binary.c -masm=intel -m32 -ldl -DBOB_SZ=$BOB_SZ -o happy_fun_binary
python patcher.py