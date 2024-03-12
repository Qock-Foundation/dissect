n = int(input())
print(n)
for i in range(n):
    arr = list(map(int, input().split()))
    for j in range(n):
        for f in range(2):
            print(*([arr[2 * j + f]] * 12), end=' ')
        print(' ', end='')
    print()
