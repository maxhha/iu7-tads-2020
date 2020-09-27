import random

random.seed(202009271900)

N = 60

brands = [
    "Nissan",
    "Porsche",
    "Audi",
    "Hyundai",
    "Ford",
    "Volkswagen",
    "Honda",
    "BMW",
    "Mercedes-Benz",
    "Toyota",
]

countries = [
    "China",
    "USA",
    "Japan",
    "Germany",
    "South Korea",
]

colors = [
    "black",
    "white",
    "pink",
    "gray",
    "yellow",
    "blue",
    "deepblue",
    "purple",
]

for i in range(N):
    print("add")
    print(random.choice(brands))
    print(random.choice(countries))
    print(str(random.randint(5, 90))+"00000")
    print(random.choice(colors))
    is_new = random.choice([True] + [False]*5)
    print("1\n" if is_new else "0")

    if is_new:
        print(str(random.randint(1,10)))
    else:
        year = random.randint(0,24)+1995
        mileage = (2020 - year)*random.randint(11000,15000)
        repairs_n = sum(random.randint(0, 4) for j in range(2020 - year))
        repairs_n += random.randint(0, 3)
        repairs_n //= 10
        owners_n = (2020 - year) // 3 + random.randint(0, 3)
        owners_n = int(owners_n / 4)

        print(str(year))
        print(str(mileage))
        print(str(repairs_n))
        print(str(owners_n))
