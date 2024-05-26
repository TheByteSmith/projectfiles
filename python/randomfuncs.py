from typing import List


def primes(n: int):
    sieve = [True] * n

    res = []

    for i in range(2, n):
        if sieve[i]:
            res.append(i)
            for j in range(i * i, n, i):
                sieve[j] = False

    return res


def biggestDif(list: List[int]):
    if len(list) <= 1:
        return 0

    return max(list) - min(list)


def powers(num: int, exponent: int):
    endnum: int = num

    if exponent == 0:
        return 1
    if exponent == 1:
        return num

    for i in range(exponent - 1):
        endnum *= num

    return endnum


print(powers(4, 2))
