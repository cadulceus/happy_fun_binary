from random import randint

def encrypt(plaintext, IV, key):
    current_block = IV
    ciphertext = []
    for byte_ind in range(0, len(plaintext)):
        #print "using ",key[current_block % len(key)], "as key byte, index ", current_block % len(key)
        current_block = current_block ^ key[current_block % len(key)]
        ciphertext.append((plaintext[byte_ind] + (current_block * current_block)) % 256)
        current_block = ciphertext[byte_ind]
    return ciphertext

def decrypt(ciphertext, IV, key):
    current_block = IV
    plaintext = []
    for byte_ind in range(0, len(ciphertext)):
        print "using ",key[current_block % len(key)], "as key byte, index ", current_block % len(key)
        current_block = current_block ^ key[current_block % len(key)]
        plaintext.append((ciphertext[byte_ind] - (current_block * current_block)) % 256)
        current_block = ciphertext[byte_ind]
    print [chr(byte) for byte in plaintext]

key = [0xee, 0x05, 0x00, 0x00]
iv = 0x67
ciphertext = encrypt([ord(char) for char in "yeetyeet\x00"], iv, key)
for b in ciphertext: print hex(b)[2:],
decrypt(ciphertext, iv, key)