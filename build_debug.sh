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

# it was too much of a pain to automate this, run generate_flag_3_arrays.py to get these magic values
export CHECKER_1_BYTES="{0x43f,0x3b8,0x4ea,0x1be,0x1c2,0x1c2,0x1c3,0x1bf,0x3d4,0x3d4,0x3d4,0x3eb,0x1be,0x3d4,0x4d4,0x2d8}"
export CHECKER_2_BYTES="{0x28,0x22,0x1f,0x3a,0x1d,0x2b,0x15,0x16,0xe,0x5c,-0x1,0x1b,0x59,0x55,-0x1,0xd,0xb,-0x1,-0xd}"
export CHECKER_3_BYTES="{0xc3,0x98,0xcd,0x36,0xef,0x19,0x55,0xed,0xc7,0x5a,0x9e,0x6f,0x19,0x4d,0x62,0x9f,0x2c,0x81,0x42,0xf6,0xd9}"

echo "flag{sorry_no_dragons_hoard_just_internet_points}" > flag4.txt
zip --password "flag{yes_this_is_all_one_big_critical_role_reference}flag{beep_beeep_bbbeeep_beeeeppppp}flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}" chest.zip flag4.txt
export ZIP_SZ=$(wc -c chest.zip | awk '{print $1;}')

export ENCRYPT_OFFSET="0x138d"
gcc -c -DDEBUG=1 -DENCRYPT_OFFSET=$ENCRYPT_OFFSET -DC1_NOPS=$C1_NOPS -DCHECKER_1="$CHECKER_1_BYTES" -DC2_NOPS=$C2_NOPS -DCHECKER_2="$CHECKER_2_BYTES" -DCHECKER_3="$CHECKER_3_BYTES" -DZIP_SZ=$ZIP_SZ -masm=intel -Wall -fpic -ggdb -Wl,-E -m32 -O0  binary_of_ballas.c
gcc -shared -m32 -o binary_of_ballas.so binary_of_ballas.o

export BOB_SZ=$(wc -c < binary_of_ballas.so)
gcc happy_fun_binary.c -masm=intel -m32 -ldl -DDEBUG -DBOB_SZ=$BOB_SZ -o happy_fun_binary
echo "rerun this build file with ENCRYPT_OFFSET=0x$(readelf -s happy_fun_binary | grep encrypt | awk '{print $2;}')!"
#strip -s --keep-symbol="foyer" binary_of_ballas.so
python patcher.py
#strip -s happy_fun_binary