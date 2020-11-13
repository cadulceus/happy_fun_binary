from pwn import *

r = process('happy_fun_binary')
e = ELF('happy_fun_binary')
#vault_loc = e.symbols['vault']

print r.recvuntil("0s and 1s.\n")
r.sendline("approach the main entrance")
print r.recvuntil("something is off.\n")
r.sendline("modify the gate")
print r.recvuntil("place in the gate?\n")
r.sendline("572")#hex(vault_loc & 0x0FFF)[2:])
print r.recvuntil("Probably?\n")
#print hex(vault_loc & 0x0FFF)[2:]
r.sendline("step through the gate")
print r.recvuntil("easily modifiable.\n")
r.sendline("modify the input")
print r.recvuntil("byte to?\n")
r.sendline("i")
print r.recvuntil("held before.\n")
r.sendline("use the upper function")
print r.recvuntil("do much.\n")
r.sendline("use the gate")
print r.recvuntil("darkness.\n")
r.sendline("enter the second doorway")
r.interactive()
