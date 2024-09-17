#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 256
#define MAX_WORD_LENGTH 100
#define MAX_VOCABULARY 1000
#define COMMON_WORDS_COUNT 20
void buildShiftTable(char *pattern, int patternLen, int shiftTable[]) {
    for (int i = 0; i < MAX; i++) {
        shiftTable[i] = patternLen;
    }
    for (int j = 0; j < patternLen - 1; j++) {
        shiftTable[(unsigned char)pattern[j]] = patternLen - 1 - j;
    }
}
int horspoolSearch(char *text, char *pattern) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    int shiftTable[MAX];
    buildShiftTable(pattern, patternLen, shiftTable);
    int i = patternLen - 1;
    while (i < textLen) {
        int k = 0;
        while (k < patternLen && pattern[patternLen - 1 - k] == text[i - k]) {
            k++;
        }
        if (k == patternLen) {
            return i - patternLen + 1;
        }
        i += shiftTable[(unsigned char)text[i]];
    }
    return -1;
}
void tokenizeText(char *text, char words[][MAX_WORD_LENGTH], int *wordCount) {
    char *token = strtok(text, " \n\t");
    *wordCount = 0;
    while (token != NULL) {
        strncpy(words[*wordCount], token, MAX_WORD_LENGTH - 1);
        words[*wordCount][MAX_WORD_LENGTH - 1] = '\0';
        (*wordCount)++;
        token = strtok(NULL, " \n\t");
    }
}
void populateSourceVocabulary(char *sources[], int numSources, char *commonWords[], int commonCount, char sourceVocabulary[][MAX_WORD_LENGTH], int *vocabCount) {
    char word[MAX_WORD_LENGTH];
    int i, j, length;
    *vocabCount = 0;
    for (i = 0; i < numSources; i++) {
        char tempText[1000];
        strcpy(tempText, sources[i]);
        char *token = strtok(tempText, " \n\t");
        while (token != NULL) {
            length = strlen(token);
            if (length > 3) {
                int isCommon = 0;
                for (j = 0; j < commonCount; j++) {
                    if (strcmp(token, commonWords[j]) == 0) {
                        isCommon = 1;
                        break;
                    }
                }
                if (!isCommon) {
                    int isInVocabulary = 0;
                    for (j = 0; j < *vocabCount; j++) {
                        if (strcmp(token, sourceVocabulary[j]) == 0) {
                            isInVocabulary = 1;
                            break;
                        }
                    }
                    if (!isInVocabulary) {
                        strncpy(sourceVocabulary[*vocabCount], token, MAX_WORD_LENGTH - 1);
                        sourceVocabulary[*vocabCount][MAX_WORD_LENGTH - 1] = '\0';
                        (*vocabCount)++;
                    }
                }
            }
            token = strtok(NULL, " \n\t");
        }
    }
}
void checkPlagiarism(char *document, char sourceVocabulary[][MAX_WORD_LENGTH], int vocabCount, int *unique) {
    char word[MAX_WORD_LENGTH];
    int matchedWords = 0;
    int totalWords = 0;
    char docWords[MAX_WORD_LENGTH * 100];
    int wordCount = 0;
    strcpy(docWords, document);
    char words[MAX_WORD_LENGTH * 100][MAX_WORD_LENGTH];
    tokenizeText(docWords, words, &wordCount);
    totalWords = wordCount;
    for (int i = 0; i < vocabCount; i++) {
        strcpy(word, sourceVocabulary[i]);
        for (int j = 0; j < totalWords; j++) {
            if (horspoolSearch(words[j], word) != -1) {
                matchedWords++;
                (*unique)--;
                break;
            }
        }
    }
    if (totalWords + *unique > 0) {
        float plagiarismPercentage = 100.00 - ((float)*unique * 100 *2) / (totalWords + *unique);
        printf("Plagiarism Percentage: %.2f%%\n", plagiarismPercentage);
    } else {
        printf("No words to check.\n");
    }
}
int main() {
    char *commonWords[COMMON_WORDS_COUNT] = {
        "like", "more", "word", "words", "less", "greater", "four", "five", "eight",
        "point", "example", "some", "three", "many", "which", "when",
        "that", "there", "with", "where"
    };
    char *sources[] = {
        "This is a source document with some similar text.",
        "Another document with entirely different content.",
        "The student's paper may have been taken from this source."
    };
    int numSources = sizeof(sources) / sizeof(sources[0]);
    char sourceVocabulary[MAX_VOCABULARY][MAX_WORD_LENGTH];
    int vocabCount = 0;
    int wc = 0;
    int unique = 0;
    char document[1000];
    printf("Enter the student's text:\n");
    fgets(document, sizeof(document), stdin);
    document[strcspn(document, "\n")] = '\0';
    char docWords[MAX_WORD_LENGTH * 100];
    strcpy(docWords, document);
    char words[MAX_WORD_LENGTH * 100][MAX_WORD_LENGTH];
    tokenizeText(docWords, words, &wc);
    unique = wc;
    populateSourceVocabulary(sources, numSources, commonWords, COMMON_WORDS_COUNT, sourceVocabulary, &vocabCount);
    checkPlagiarism(document, sourceVocabulary, vocabCount, &unique);
    return 0;
}