#include <stdio.h>
#include <string>
#include <time.h>
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

double getBigramProb(const char *w1, const char *w2, Ngram bigram_lm, Vocab voc);

int main(int argc, char *argv[]){
    //clock_t t1, t2;
    //t1 = clock();
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
    //t2 = clock();
    //fprintf(stderr, "time = %lf secs\n", double(t2 - t1)/(double)(CLOCKS_PER_SEC));
    return 0;
}

void make_sentence_correct(Ngram bigram_lm, Vocab voc, VocabMap zhu_map, Vocab zhuyin, Vocab big5, char *line){
    //fprintf(stderr, "%s\n", line);
    VocabString word[MAX_WORD];
    unsigned length = Vocab::parseWords(line, word + 1, MAX_WORD); // get num of words
    length += 2;
    LogP probability[length][MAX_CANDIDATE];
    int num_of_candidate[length];
    VocabIndex candidate[length][MAX_CANDIDATE];    
    int backtrack[length][MAX_CANDIDATE];
    int count, idx;

    word[0] = "<s>";
    word[length - 1] = "</s>";
    num_of_candidate[0] = 1;
    num_of_candidate[length - 1] = 1;
    candidate[0][0] = big5.getIndex(word[0]);
    candidate[length - 1][0] = big5.getIndex(word[length - 1]);
    probability[0][0] = 0.0;
    VocabString w1, w2;
    VocabIndex wid1, wid2;
    VocabIndex unk = voc.getIndex(Vocab_Unknown);
    LogP max, tmp;
    for(int i = 1; i < length - 1; i++){
        VocabIndex index;
        Prob p;
        count = 0;
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

            w1 = big5.getWord(candidate[i][j]);
            
            wid1 = voc.getIndex(w1);
            
            wid1 = (wid1 == Vocab_None) ? unk : wid1;
            
            for(int k = 0; k < num_of_candidate[i - 1]; k++){
                
                w2 = big5.getWord(candidate[i - 1][k]);
                
                wid2 = voc.getIndex(w2);

                wid2 = (wid2 == Vocab_None) ? unk : wid2;
                
                VocabIndex context[] = {wid2, Vocab_None};
                tmp = probability[i - 1][k] + bigram_lm.wordProb(wid1, context);
                //fprintf(stderr, "%e\n", bigram_lm.wordProb(wid1, context));
                
                //tmp = probability[i - 1][k] + getBigramProb(w2, w1, bigram_lm, voc);

                if(tmp > max){
                    max = tmp;
                    idx = k;
                }
            }
            probability[i][j] = max;
            backtrack[i][j] = idx;
            //fprintf(stderr, "max = %d\n", max);
        }
    }

    VocabString correct_sentence[length];
    correct_sentence[0] = "<s>";
    int prev_index = 0;
    for(int i = length - 1; i > 0; i--){
        correct_sentence[i] = big5.getWord(candidate[i][prev_index]);
        prev_index = backtrack[i][prev_index];
    }

    for(int i = 0; i < length; i++){
        if(i != length - 1){
            printf("%s ", correct_sentence[i]);    
        }
        else{
            printf("%s", correct_sentence[i]);
        }
    }
    printf("\n");
    return;
}
double getBigramProb(const char *w1, const char *w2, Ngram bigram_lm, Vocab voc){
    VocabIndex wid1 = voc.getIndex(w1);
    VocabIndex wid2 = voc.getIndex(w2);
    wid1 = (wid1 == Vocab_None) ? voc.getIndex(Vocab_Unknown) : wid1;
    wid2 = (wid2 == Vocab_None) ? voc.getIndex(Vocab_Unknown) : wid2;
    
    VocabIndex context[] = {wid1, Vocab_None};
    //fprintf(stderr, "%e\n", bigram_lm.wordProb(wid2, context));
    return bigram_lm.wordProb(wid2, context);
}
// ./mydisambig -text testdata/1.txt -map ZhuYin-Big5.map -lm bigram.lm -order 2 > result2/1.txt