with open('dmonitor.trace') as f:
    content = f.read()

sections = content.split('\n\n')
del sections[-1]

lines = sections[-1].split('\n')
mutexs = lines[1:]

if len(mutexs) % 2 == 1 or\
        (len(mutexs) == 2 and mutexs[0] == mutexs[1]):
    print(mutexs)
    print('CASE 1')
    print('NO DEADLOCK')
else:
    i = 0
    flag = 1
    while True:
        j = 0+1
        while True:
            print(len(mutexs), i, j, mutexs)
            print()
            if mutexs[0] == mutexs[j]:
                del mutexs[j]
                del mutexs[0]
                # i -= 2
                flag = 0
                break
            if j == len(mutexs) - 1:
                flag = 1
                break
            j += 1
        if flag == 1:
            print('CASE 2')
            print('NO DEADLOCK')
            break
        if 0 == len(mutexs):
            print('DEADLOCK')
            break
        # i += 1

