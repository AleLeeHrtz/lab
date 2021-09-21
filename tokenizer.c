//
// Created by Alejandra on 9/9/21.
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


int main(){
    //Asks for input

    int limit = 500;

    char rawS[limit];
    printf("Enter a sentence. Limited up to 500 characters");
    fgets(rawS, limit , stdin);
    printf(("\n\n"));

    //Calls function tokenizer

    int adress = tokenizer(rawS);
}

int tokenizer(char rawS[]){


    //Here the array of tokens is declared with a length up to the number of words found + 1
    int count = countWds(rawS);

    char* toks[count + 1];

    int idx = 0;
    int WordIdx = 0;

    //since my code is based on looking for spaces, here it checks if the first char is not a space
    //to know there is a word at the first index and signal ir

    bool check = false;
    if (rawS[0] != ' '){
        check = true;
    }

    //Here, the original string is read char by char until a space is found, if so, the method countCinW
    //is called to know how much memory to allocate, then every character is copied down into the tokens
    //array with the correct position. This process is repeated until a \n is found, meaning the index reached
    //the end of the sentence.

    while (rawS[idx] != '\n'){
        if ((rawS[idx] == ' ' && (rawS[idx + 1] != ' ' && rawS[idx + 1] != '\n'))|| check){
            int charsN = countCinW( idx + 1, rawS);
            if (check){
                charsN++;
            }

            //

            toks[WordIdx] = (char*) malloc(charsN * sizeof(char));

            if (WordIdx != 0 ){
                idx++;
            }

            int tempIdx = 0;
            while (rawS[idx] != ' ' && rawS[idx] != '\n' ){
                toks[WordIdx][tempIdx] = rawS[idx];
                idx++;
                tempIdx++;

            }

            //

            printf("Token [%i]: %s\n\n", WordIdx, toks[WordIdx]);
            WordIdx++;
            //idx++;
            check = false;
        }
        else{
            idx++;
        }
    }


    return toks;
}

//This method counts the number of characters in each word and returns it
int countCinW(int indexer, char rawS[] ){
    int count = 0;
    int tempidx = indexer;

    while (rawS[tempidx] != ' ' && rawS[tempidx] != '\n'){
        count++;
        tempidx++;
    }

    return count;
}

//This method counts the number of words by looking at the spaces and returns it
int countWds(char rawS[]){
    int result = 0;
    int idx = 1;
    char temp = rawS[0];

    while(temp != '\n'){
        if (temp == ' ' && rawS[idx + 1] != '\n' && rawS[idx + 1] != ' ' ){
            result++;
        }
        temp = rawS[idx];
        idx++;
    }
    if (rawS[0] == '\n'){
        return 0;
    }
    return result + 1;
}