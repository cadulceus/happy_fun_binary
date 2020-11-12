import sys
asm = '\"int3\\n\\t%sint3\\n\\t\"'
print asm % ("nop\\n\\t" * (int(sys.argv[1]) - 2))