import sys
nums = []
for line in open("BigInteger.txt"):
    nums.append(int(line))

for a in nums:
    for b in nums:
        print(a + b)
        print(a - b)
        print(a * b)
        print(a / b)

sys.stdout.flush()
