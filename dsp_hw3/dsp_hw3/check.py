import sys

input_file = sys.argv[1]
output_file = sys.argv[2]
#infile = 'Big5-ZhuYin.map'
#outfile = 'ZhuYin-Big5.map'
fp1 = open(input_file, 'r', encoding='BIG5-HKSCS')
fp2 = open(output_file, 'r', encoding='BIG5-HKSCS')
count = accuracy = 0.0
dictionary = dict()
#dic = collections.defaultdict(list)
list1 = []
list2 = []
for line1 in fp1:
    list1.append(line1)
for line2 in fp2:
    list2.append(line2)
    #print(line2)
for i in range(0, len(list2)):
    #print(list2[i])
    count += 1
    if(list1[i] == list2[i]):
        accuracy += 1

print(accuracy / count)