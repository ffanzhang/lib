import sys
import math
nums = []
for line in open("BigInteger.txt"):
    nums.append(int(line))

for a in nums:
    for b in nums:
        print(a + b)
        print(a - b)
        print(a * b)
        # python integer division rounds to negative inf,
        # where as cpp rounds to 0, currently testing pos
        # division only
        print(int(math.floor(abs(a) / abs(b))))

sys.stdout.flush()
