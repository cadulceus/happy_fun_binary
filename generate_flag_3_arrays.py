#31337 sequence
seq = [0, 1, 1, 1, 1, 0, 0, 1, 1, 2, 0, 0, 2, 1, 1, 2, 2, 2, 0, 2, 0, 0, 2, 2, 0, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 0, 1, 2, 0, 2, 2, 0, 0, 2, 1, 2, 2, 2, 2, 2, 1, 0, 0, 1, 2, 0]
flag = "flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}"

def offset_checker(f_contents, c):
    return hex(f_contents.find(c))

def simple_xor(c, i):
    return hex((ord(c) ^ ord('E')) - i)

def encryptor(IV, key, input):
    return hex(((IV ^ ord(key[IV % 4]))**2 + ord(input)) % 256)

with open("happy_fun_binary", "r") as f:
    f_contents = f.read()
    part_1 = "{"
    part_2 = "{"
    part_3 = "{"
    for i, num in enumerate(seq):
        if num == 0:
            part_1 += offset_checker(f_contents, flag[i]) + ","
        if num == 1:
            part_2 += simple_xor(flag[i], i) + ","
        if num == 2:
            part_3 += encryptor(i, "YEET", flag[i]) + ","


print part_1[:-1] + "}"
print part_2[:-1] + "}"
print part_3[:-1] + "}"