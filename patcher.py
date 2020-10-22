from pwn import *

def patch(routine):
    with open(routine[0], "r") as f:
        f.seek(routine[2])
        bytestr = f.read(routine[4])
    print "writing " + "".join([x.encode("hex") for x in bytestr])
    with open(routine[1], "r+b") as f:
        f.seek(routine[3])
        f.write(bytestr)

def patch_const(routine):
    for i in range(64): print hex(ord(routine[2][i])),
    print routine[2][:64]
    print len(routine[2])
    print routine[1]
    #print "writing " + "".join([x.encode("hex") for x in routine[2]])
    with open(routine[0], "r+b") as f:
        f.seek(routine[1])
        f.write(routine[2])


def find_pattern(fn, pstr):
    with open(fn, "r") as f:
        raw = f.read()
        off = raw.find(pstr)
        if off == -1:
            print "couldn't find pattern " + pstr
            exit(0)
        return off

def encrypt(plaintext, IV, key):
    current_block = IV
    ciphertext = []
    for byte_ind in range(0, len(plaintext)):
        #print "using ",key[current_block % len(key)], "as key byte, index ", current_block % len(key)
        current_block = current_block ^ key[current_block % len(key)]
        ciphertext.append((plaintext[byte_ind] + (current_block * current_block)) % 256)
        current_block = ciphertext[byte_ind]
    return "".join([chr(c) for c in ciphertext])

hfb = ELF("happy_fun_binary")
vault_loc = hfb.symbols['vault'] & 0x0FFF
IV = 0x69
key = vault_loc**3

print("Key: ", key, "IV: ", chr(IV))
print("Sea of entropy: ", hfb.symbols["sea_of_entropy"])
print("vault: ", vault_loc)
raw_input("Press enter to continue patching...")

f = open("binary_of_ballas.so", "r")
const_routines = []
routines = []

print [hex(ord(char)) for char in p32(key)]
# target file, target offset, value
const_routines.append(["happy_fun_binary", 
            hfb.symbols["sea_of_entropy"] - 0x1000, # the segment .data is in gets loaded at +0x1000 so we need to correct
            encrypt([ord(char) for char in f.read()], IV, [ord(char) for char in p32(key)])])
        #["bomblab", e.symbols["fptrs"]-0x08048000, p64(find_pattern("bomblab", "pch1") + find_pattern("phase1_64", "con3")-0x54+0x08048000-0x3) + p64(0x23)],

for routine in const_routines:
    print "current const routine: "
    print routine[0]
    patch_const(routine)
for routine in routines:
    print routine[0]
    print routine
    patch(routine)
