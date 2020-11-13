fasm checker_1.asm
fasm checker_2.asm
gcc first_n_rand_vals.c -o first_n_rand_vals
export FLAG3=flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}
export FLAG3_SZ=${#FLAG3}
export C1_SZ=$(wc -c checker_1.bin | awk '{print $1;}')
C1_SZ=$((C1_SZ - 1)) # decrementing 1 because an extra identifier byte is defined in c
export C2_SZ=$(wc -c checker_2.bin | awk '{print $1;}')
C2_SZ=$((C2_SZ - 1))
export C1_NOPS=$(python chal3_64bit_stub_gen.py $C1_SZ)
export C2_NOPS=$(python chal3_64bit_stub_gen.py $C2_SZ)
export CHECKER_1_BYTES="$(python find_first.py happy_fun_binary $FLAG3)"
export CHECKER_2_BYTES="$(python checker_2_calc.py $FLAG3)"
echo $CHECKER_2_BYTES
gcc -DC1_NOPS=$C1_NOPS -DCHECKER_1="$CHECKER_1_BYTES" -DC2_NOPS=$C2_NOPS -DCHECKER_2="$CHECKER_2_BYTES" -masm=intel -m32 -O0 -fno-pie -no-pie chal_3_test.c -o test 
python patcher_2.py