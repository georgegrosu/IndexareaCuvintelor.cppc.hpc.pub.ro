//
//  main.c
//  IndexareaCuvintelor
//
//  Created by George Mircea on 2/9/17.
//  Copyright © 2017 Grosu George-Mircea. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** wordsExtract( char *fileContent, int minimumLength, int maximumLength);
char** selectWords( char **words, int wordCount, int minimumLength, int maximumLength);
void exportIndexedWords( char **words, int wordCount, int occurrencesNumber, char *outputFileName);
char* toLowercase( char *word);

int main(int argc, const char * argv[]) {
    char *fileName = NULL;
    char *outputFileName = NULL;
    char *fileContent;
    int minimumLength, maximumLength, occurrencesNumber;
    char **words;
    int wordCount = 0, fileSize;
    
    if ( argc > 4) {
        fileName = malloc( strlen( argv[1]) + 1);
        strcpy( fileName, argv[1]);
        minimumLength = atoi( argv[2]);
        maximumLength = atoi( argv[3]);
        occurrencesNumber = atoi( argv[4]);
        
        FILE *inputFile = fopen( fileName, "rt");
        if ( inputFile == NULL) {
            exit( -1);
        }
        
        outputFileName = strtok( fileName, ".\0");
        strcat( outputFileName, ".idx");
        
        free( fileName);
        
        fseek( inputFile, 0, SEEK_END);
        fileSize = (int)ftell( inputFile);
        fileContent = malloc( fileSize * sizeof(char));
        fseek( inputFile, 0, SEEK_SET);
        
        fread( fileContent, fileSize, 1, inputFile);
        
        fclose( inputFile);
        
        words = wordsExtract( fileContent, minimumLength, maximumLength);
        
        while ( words[ wordCount] != NULL) {
            wordCount ++;
        }
        
        exportIndexedWords( words, wordCount, occurrencesNumber, outputFileName);
        
    }
    else {
        printf( "Introduceti corect parametri!\n");
    }
    
    return 0;
}

char* toLowercase( char *word) {
    unsigned long i;
    
    for ( i=0; i < strlen( word); i++) {
        word[i] = tolower( word[i]);
    }
    
    return word;
}

void exportIndexedWords( char **words, int wordCount, int occurrencesNumber, char *outputFileName) {
    int *checkedWord;
    int i, j;
    int wordOccurrences;
    int wordLength;
    
    checkedWord = calloc( wordCount, sizeof(int));
    
    FILE *outputFile = fopen( outputFileName, "wb");
    
    for ( i=0; i < wordCount-1; i++) {
        if ( checkedWord[i] == 0 ) {
            wordOccurrences = 1;
            checkedWord[i] = 1;
            for ( j=0; j < wordCount; j++) {
                if ( checkedWord[j] == 0 && strcmp( toLowercase( words[i]), toLowercase( words[j])) == 0) {
                    wordOccurrences++;
                    checkedWord[j] = 1;
                }
            }
            if ( wordOccurrences >= occurrencesNumber) {
                wordLength = (int)strlen( words[i]);
                
                fwrite( &wordLength, sizeof(int), 1, outputFile);
                fwrite( words[i], strlen( words[i]), 1, outputFile);
                fwrite( &wordOccurrences, sizeof(int), 1, outputFile);
            }
        }
    }
    
    fclose( outputFile);
    
}

char** selectWords( char **words, int wordCount, int minimumLength, int maximumLength) {
    int i;
    
    for ( i=0; i < wordCount; i++) {
        if ( strlen( words[i]) < (unsigned long)minimumLength || strlen( words[i]) > (unsigned long)maximumLength) {
            if ( i == wordCount-1) {
                words[ wordCount-1] = NULL;
                wordCount--;
            }
            else {
                words[i] = realloc( words[i], strlen( words[ wordCount-1]) + 1);
                strcpy( words[i], words[ wordCount-1]);
                words[ wordCount-1] = NULL;
                wordCount--;
            }
        }
    }
    
    words = realloc( words, wordCount * sizeof(char*));
    
    return words;
}

char** wordsExtract( char *fileContent, int minimumLength, int maximumLength) {
    char delimitators[] = " .,:;-\n/";
    char *wordKeeper;
    int wordCount = 0;
    char **words;
    
    wordKeeper = malloc( strlen( fileContent) + 1);
    if ( wordKeeper == NULL) {
        exit( -1);
    }
    
    words = malloc( sizeof(char*) );
    if ( words == NULL) {
        exit( -1);
    }
    wordKeeper = strtok( fileContent, delimitators);
    words[wordCount] = malloc( strlen(wordKeeper) + 1);
    if ( words[ wordCount] == NULL) {
        exit( -1);
    }
    strcpy( words[ wordCount], wordKeeper);
    
    wordKeeper = strtok( NULL, delimitators);
    
    while ( wordKeeper != NULL) {
        
        wordCount ++;
        words = realloc( words, (wordCount+1) * sizeof(char*));
        if ( words == NULL) {
            exit( -1);
        }
        words[wordCount] = malloc( strlen(wordKeeper) + 1);
        if ( words[ wordCount] == NULL) {
            exit( -1);
        }
        strcpy( words[ wordCount], wordKeeper);
        wordKeeper = strtok( NULL, delimitators);
        
    }
    
    free( wordKeeper);
    
    return selectWords( words, wordCount, minimumLength, maximumLength);
}
