# -*- coding: utf-8 -*-

#====================
#File: letter_sta.py
#Author: Moming
#Date: 2015-10-14
#====================

import re
import os

Total = 0; #总字母数
words = []

#获取所有单词
readfile = open('WarAndPeace.txt', encoding = 'utf-8')

for line in readfile.readlines():
    lineArr = line.strip().split()
    for word in lineArr:
        data = re.findall(r'[a-zA-Z]*', word)
        for w in data:
            if w != '':
                words.append(w.lower())

readfile.close()
    
#输出前n个和后n个高频字母、统计频数、统计频率
def PrintSta(Dic, total_num, n):
    if n > len(Dic):
        print('n超出索引范围')
        return
    word_lst = []
    for word, freq in Dic.items():
        word_lst.append((freq, word))

    word_lst.sort(reverse = True)
    print('字母组合\t频数\t频率')
    for freq, word in word_lst[:n]:
        print('{0}\t{1}\t{2:.5}'.format(word, freq, freq/total_num))
    for freq, word in word_lst[-n:]:
        print('{0}\t{1}\t{2:.5}'.format(word, freq, freq/total_num))

#进行统计
def MySta(n):
    Dic = {}
    total_num = 0
    for word in words:
        for i in range(len(word) - n + 1):
            letter = word[i:i+n]
            total_num = total_num + 1
            if letter in Dic.keys():
                Dic[letter] = Dic[letter] + 1
            else:
                Dic[letter] = 1

    return Dic,total_num

#测试统计
Dic, total_num = MySta(1)
PrintSta(Dic, total_num, 13)
print('*-----------------------------------------*')
Dic, total_num = MySta(2)
PrintSta(Dic, total_num, 10)
print('*-----------------------------------------*')
Dic, total_num = MySta(3)
PrintSta(Dic, total_num, 10)
print('*-----------------------------------------*')
Dic, total_num = MySta(4)
PrintSta(Dic, total_num, 10)

os.system("pause")
