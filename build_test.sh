fasm checker_1.asm
export C1_SZ=$(wc -c checker_1.bin)
export NOPS=$(python chal3_64bit_stub_gen.py $C1_SZ)
gcc -DNOPS=$NOPS -masm=intel -m32 -fno-pie -no-pie chal_3_test.c -o test 
python patcher_2.py