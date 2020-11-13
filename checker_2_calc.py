import sys, re

out = ""
for i, c in enumerate(sys.argv[1]):
        #'f' = (x + 1) ^ 'E' -> x = ('f' ^ 'E') - 1
        out += hex((ord(c) ^ ord('E')) - i) + ", "
print "{%s}" % out[:-2]