import sys

input_file = sys.argv[1]
output_file = sys.argv[2]
#infile = 'Big5-ZhuYin.map'
#outfile = 'ZhuYin-Big5.map'
fp1 = open(input_file, 'r', encoding='BIG5-HKSCS')
fp2 = open(output_file, 'w', encoding='BIG5-HKSCS')

dictionary = dict()
#dic = collections.defaultdict(list)
for line in fp1:
    chinese = line.split()[0]
    zhuins = line.split()[1]
    zhuins = zhuins.split('/')
    for zhuin in zhuins:
        if zhuin[0] in dictionary:
            dictionary[zhuin[0]].append(chinese)
        else:
            dictionary[zhuin[0]] = [chinese]
        #print(zhuin[0])
    dictionary[chinese] = chinese
        #print("ch = " + ch, "zhuins[0] = ", zhuins[0])

for index in dictionary:
    fp2.write(index + '\t')
    list1 = dictionary[index]
    length = len(list1)
    for i in range(0, length):
        if i == length - 1:
            fp2.write(list1[i] + '\n')
        else:
            fp2.write(list1[i] + ' ')

