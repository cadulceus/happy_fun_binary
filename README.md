name: Happy Fun Binary (1-4)
Score: 150, 200, 300, 50
description: archmages host the worst house parties.?
SOLUTION:
This is a reversing challenge with a total of 4 flags encoded in the binary. The entire binary plays as a text based adventure, except the options are not printed out and must be reversed. The first part of the challenge involved finding the correct function offset to call, as the original call was "tampered with". Once the player inputs the correct offset, they are presented with an encryption function and a stubbed out/non functional decryption function that runs against a large encrypted array. the encryption function is a simple homebrew CBC + xor algorithm that is absolutely not secure. The key used is the offset that was given earlier, and the IV is 1 byte that is intended to either be brute forced or calculated from the first byte of ciphertext. The player can run a command that will write the array to disk, then dlopen() it and call into it. Assuming it was decrypted properly, the first flag is printed out when the player runs a command to print it.
The second challenge is a C implementation of arithmetic encoding, the encoded flag is given along with the probability mapping for each of its characters. the player must write a decoder to extract it.
The third challenge is a heavily obfuscated strcmp. The challenge asks for a flag, then uses rand() with a static seed to determine which of 3 byte-checker functions to call for every byte in the input flag. The static seed is modified by a hidden thread started by the shared object's constructor, and is not the same as the initial value in the binary. 2 of the 3 checker functions use the far jmp/call trick with a modified cs register value to execute 64 bit code. The first checker maps the original binary into 0x180000 using a 64 bit syscall, then uses a hardcoded list of offsets to compare each byte it receives to an offset of 180000, meaning large modifications to the original binary will likely break the check, and returns 1 if the input byte matches. The second checker (which should always execute after the first checker with the correct seed) performs a simple xor operation "(input + string_index) ^ byte_at_0x180001" where 0x180001 should compare the second byte of the ELF header, 'E', then returns 1 if the input byte matches. The third function calls the encryption function in the newly mapped original binary, and compares the input byte with the result of the encryption. Contestants are expected to either RE the flag out, or recognize the anti-debugging code and instrument the binary.
The final flag is in an encrypted zip file whose password is the 3 flags concatenated in order. The binary will demonstrate the means of concatenating the flags and decrypting them.
FLAGS:
part 1: flag{yes_this_is_all_one_big_critical_role_reference}
part 2: flag{beep_beeep_bbbeeep_beeeeppppp} // it had to have low entropy to work
part 3: flag{h3av3ns_gate_should_b3_r3nam3d_to_planar_sh1ft_1m0}
part 4: flag{sorry_no_dragons_hoard_just_internet_points}
NOTES:
./build.sh will build the release binary
./build_debug.sh will build a test binary that has the decoding functions implemented, plus some debugging lines.
the build scripts echo a funky line that you need to follow for the binary to work
python solver.py will run a solver that expects the debug version of the
everything else is used by the build script