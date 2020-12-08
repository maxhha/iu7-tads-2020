from random import shuffle, randint

def is_prime(num):
    if (num <= 1):
        return 0;

    if (num % 2 == 0 and num > 2):
        return 0;

    i = 3
    while (i < int(num ** 0.5)):

        if (num % i == 0):
            return 0;

        i += 2

    return 1;

def next_prime(n):
    prime = n + 1;
    while (not is_prime(prime)):
        prime+=1

    return prime;

FILE="in.txt"
MAX = 999
KEEP = 20

data = list(range(1, MAX+1, next_prime(KEEP + 1)))

shuffle(data)

data = data[:KEEP]

with open(FILE, "w") as f:
    for i in data:
        f.write(str(i) + '\n')



hashtable_size = next_prime(KEEP + 1)

def get_rand_params():
    return [
        randint(0, 0xFFFF if i % 2 == 0 else 15)
        for i in range(12)]

template = """
x = (((x + 0x{:x}) & 0xFFFF) + ((x << {:d}) & 0xFFFF)) & 0xFFFF;
x = ((x ^ 0x{:x}) ^ (x >> {:d})) & 0xFFFF;
x = (((x + 0x{:x}) & 0xFFFF) + ((x << {:d}) & 0xFFFF)) & 0xFFFF;
x = (((x + 0x{:x}) & 0xFFFF) ^ ((x << {:d}) & 0xFFFF)) & 0xFFFF;
x = (((x + 0x{:x}) & 0xFFFF) + ((x << {:d}) & 0xFFFF)) & 0xFFFF;
x = ((x ^ 0x{:x}) ^ (x >> {:d})) & 0xFFFF;
"""

c_template = """
unsigned short x = a;
x = (x + 0x{:04x}) + (x << {:d});
x = (x ^ 0x{:04x}) ^ (x >> {:d});
x = (x + 0x{:04x}) + (x << {:d});
x = (x + 0x{:04x}) ^ (x << {:d});
x = (x + 0x{:04x}) + (x << {:d});
x = (x ^ 0x{:04x}) ^ (x >> {:d});
"""

best_params = None
best_func = None
best_score = 0
best_hashes = None

for _ in range(5000):
    params = get_rand_params()
    code = template.format(*params)
    hashes = set()

    for x in data:
        exec(code, None, None)
        hashes.add(x % hashtable_size)

    score = len(hashes)

    if score > best_score:
        best_func = code
        best_score = score
        best_hashes = hashes
        best_params = params

print("//", len(set(i % hashtable_size for i in data)), "vs", best_score)
print("// hashes:", best_hashes)
print(best_func)

for i in data:
    print("hash(%d)" % i, end="")
    x = i
    exec(best_func, None, None)
    print(" =", x % hashtable_size)

print(c_template.format(*best_params))
