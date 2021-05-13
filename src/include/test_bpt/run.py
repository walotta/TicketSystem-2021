import os
import filecmp

test_size = 10000
for i in range(test_size):
    os.system('rm BptOf_test')
    os.system('rm out.txt')
    os.system('touch out.txt')
    os.system('./code > out.txt')
    if filecmp.cmp("out.txt", "right.txt"):
        print(i+1, 'is right')
    else:
        print('error occured')
        break
