make clean
make
./mydisambig -text testdata_seg/1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/1.txt
./mydisambig -text testdata_seg/2.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/2.txt
./mydisambig -text testdata_seg/3.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/3.txt
./mydisambig -text testdata_seg/4.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/4.txt
./mydisambig -text testdata_seg/5.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/5.txt
./mydisambig -text testdata_seg/6.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/6.txt
./mydisambig -text testdata_seg/7.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/7.txt
./mydisambig -text testdata_seg/8.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/8.txt
./mydisambig -text testdata_seg/9.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/9.txt
./mydisambig -text testdata_seg/10.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/10.txt
./mydisambig -text testdata_seg/example.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/example.txt
./mydisambig -text testdata_seg/example_ans.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 >result/example_ans.txt
