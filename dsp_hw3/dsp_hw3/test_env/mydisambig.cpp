#include <stdio.h>
#include <string>
#include "option.h"
#include "File.h"
#include "Prob.h"
#include "Ngram.h"
#include "Vocab.h"
#include "VocabMap.h"
#define MAX_WORD 512
#define MAX_CANDIDATE 1024
#define MIN_NUMBER -100000

void make_sentence_correct(Ngram bigram_lm, Vocab voc, VocabMap zhu_map, Vocab zhuyin, Vocab big5, char *line);

int main(int argc, char *argv[]){
    int ngram_order = atoi(argv[8]);
    Vocab voc, zhuyin, big5;
    VocabMap zhu_map(zhuyin, big5);
    Ngram bigram_lm(voc, ngram_order);
    
    big5.addWord("<s>");
    big5.addWord("</s>");

    File lmFile(argv[6], "r"); // bigram log
    bigram_lm.read(lmFile);
    lmFile.close();
    
    File mapfile(argv[4], "r"); // zhuyin to big5 map
    zhu_map.read(mapfile);
    mapfile.close();

    File testfile(argv[2], "r"); // seperated txt 
    char *line;
    while((line = testfile.getline())) {
        //fprintf(stderr, "%s\n", line);
        make_sentence_correct(bigram_lm, voc, zhu_map, zhuyin, big5, line);
    }
    testfile.close();
    return 0;
}

void make_sentence_correct(Ngram bigram_lm, Vocab voc, VocabMap zhu_map, Vocab zhuyin, Vocab big5, char *line){
    //fprintf(stderr, "%s\n", line);
    VocabString word[MAX_WORD];
    unsigned length = Vocab::parseWords(line, word + 1, MAX_WORD) + 2; // get num of words
    
    //-----------------------------------------------------------------
    LogP probability[length][MAX_CANDIDATE];
    int num_of_candidate[length];
    VocabIndex candidate[length][MAX_CANDIDATE];    
    int backtrack[length][MAX_CANDIDATE];

    word[0] = "<s>";
    word[length - 1] = "</s>";
    num_of_candidate[0] = 1;
    num_of_candidate[length - 1] = 1;
    candidate[0][0] = big5.getIndex("<s>");
    candidate[length - 1][0] = big5.getIndex("</s>");
    probability[0][0] = 0.0;
    VocabString word_j, word_k;
    VocabIndex index_j, index_k;
    LogP max, tmp;
    for(int i = 1; i < length - 1; i++){
        VocabIndex index;
        Prob p;
        int count = 0;
        VocabMapIter iteration(zhu_map, zhuyin.getIndex(word[i]));
        iteration.init();
        while(1){
            if(!iteration.next(index, p)){
                break;
            }
            candidate[i][count] = index;
            count++;
        }
        num_of_candidate[i] = count;
    }
    for(int i = 1; i < length; i++){
        //fprintf(stderr, "%d %d\n", i, length);
        for(int j = 0; j < num_of_candidate[i]; j++){
            max = MIN_NUMBER;

            word_j = big5.getWord(candidate[i][j]);
            index_j = voc.getIndex(word_j);
            
            index_j = (index_j == Vocab_None) ? voc.getIndex(Vocab_Unknown) : index_j;

            for(int k = 0; k < num_of_candidate[i - 1]; k++){
                
                word_k = big5.getWord(candidate[i - 1][k]);
                index_k = voc.getIndex(word_k);

                index_k = (index_k == Vocab_None) ? voc.getIndex(Vocab_Unknown) : index_k;
                
                VocabIndex context[] = {index_k, Vocab_None};

                tmp = probability[i - 1][k] + bigram_lm.wordProb(index_j, context);

                if(tmp > max){
                    max = tmp;
                    backtrack[i][j] = k;
                }
            }
            probability[i][j] = max;
            //fprintf(stderr, "max = %d\n", max);
        }
    }

    VocabString ans[length];
    ans[0] = "<s>";
    int prev_index = 0;
    for(int i = length - 1; i > 0; i--){
        ans[i] = big5.getWord(candidate[i][prev_index]);
        prev_index = backtrack[i][prev_index];
    }

    // print
    for(int i = 0; i < length - 1; i++)
        printf("%s ", ans[i]);
    printf("</s>\n");
    //-------------------------------------
    return;
}
// ./mydisambig -text testdata/1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/1.txt