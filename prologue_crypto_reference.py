from random import randint

def encrypt(plaintext, seed_byte, key):
    current_block = seed_byte
    ciphertext = []
    for byte_ind in range(0, len(plaintext)):
        current_block = current_block ^ key[current_block % len(key)]
        ciphertext.append((plaintext[byte_ind] + (current_block * current_block)) % 256)
        current_block = ciphertext[byte_ind]
    return ciphertext

def decrypt(ciphertext, seed_byte, key):
    current_block = seed_byte
    plaintext = []
    for byte_ind in range(0, len(ciphertext)):
        current_block = current_block ^ key[current_block % len(key)]
        plaintext.append((ciphertext[byte_ind] - (current_block * current_block)) % 256)
        current_block = ciphertext[byte_ind]
    print [chr(byte) for byte in plaintext]

ciphertext = encrypt([ord(char) for char in "meetyeetyeetAAAAAAAAAAABAFFFFFFFFFFFFFFFF"], 0x87, [0x04, 0x32, 0x71, 0x15])
decrypt(ciphertext, 0x87, [0x04, 0x32, 0x71, 0x15])