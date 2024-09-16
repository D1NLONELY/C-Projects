#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 256  // Maximum number of characters in ASCII set
#define MAX_WORD_LENGTH 100
#define MAX_VOCABULARY 1000
#define COMMON_WORDS_COUNT 20

// Function to create the bad character shift table for Horspool algorithm
void buildShiftTable(char *pattern, int patternLen, int shiftTable[]) {
    for (int i = 0; i < MAX; i++) {
        shiftTable[i] = patternLen;  // Initialize all shifts to the pattern length
    }
    for (int j = 0; j < patternLen - 1; j++) {
        shiftTable[(unsigned char)pattern[j]] = patternLen - 1 - j;  // Update shift based on the pattern
    }
}

// Horspool algorithm for pattern matching
int horspoolSearch(char *text, char *pattern) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    int shiftTable[MAX];
    
    // Build the bad character shift table
    buildShiftTable(pattern, patternLen, shiftTable);

    int i = patternLen - 1;  // Start at the end of the pattern
    while (i < textLen) {
        int k = 0;
        while (k < patternLen && pattern[patternLen - 1 - k] == text[i - k]) {
            k++;
        }
        if (k == patternLen) {
            return i - patternLen + 1;  // Pattern found, return the starting index
        }
        i += shiftTable[(unsigned char)text[i]];  // Shift based on the mismatch
    }
    return -1;  // Pattern not found
}

// Function to tokenize the text into words
void tokenizeText(char *text, char words[][MAX_WORD_LENGTH], int *wordCount) {
    char *token = strtok(text, " \n\t");
    *wordCount = 0;
    while (token != NULL) {
        strncpy(words[*wordCount], token, MAX_WORD_LENGTH - 1);
        words[*wordCount][MAX_WORD_LENGTH - 1] = '\0';  // Ensure null termination
        (*wordCount)++;
        token = strtok(NULL, " \n\t");
    }
}

// Function to scan source texts and populate sourceVocabulary
void populateSourceVocabulary(char *sources[], int numSources, char *commonWords[], int commonCount, char sourceVocabulary[][MAX_WORD_LENGTH], int *vocabCount) {
    char word[MAX_WORD_LENGTH];
    int i, j, length;
    *vocabCount = 0;

    for (i = 0; i < numSources; i++) {
        char *text = sources[i];
        char *token = strtok(text, " \n\t");
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
                        strcpy(sourceVocabulary[*vocabCount], token);
                        (*vocabCount)++;
                    }
                }
            }
            token = strtok(NULL, " \n\t");
        }
    }
}

// Function to check plagiarism by searching for matching words
void checkPlagiarism(char *document, char sourceVocabulary[][MAX_WORD_LENGTH], int vocabCount, int *unique) {
    char word[MAX_WORD_LENGTH];
    int matchedWords = 0;
    int totalWords = 0;
    
    // Tokenize the document into words
    char docWords[MAX_WORD_LENGTH * 100];
    int wordCount = 0;
    strcpy(docWords, document);
    char words[MAX_WORD_LENGTH * 100][MAX_WORD_LENGTH];
    tokenizeText(docWords, words, &wordCount);

    totalWords = wordCount;
    
    // Search for each word in the source vocabulary in the student text
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

    // Calculate and display the plagiarism percentage
    if (totalWords + *unique > 0) {
        float plagiarismPercentage = ((float)*unique * 100) / (totalWords + *unique);
        printf("Plagiarism Percentage: %.2f%%\n", plagiarismPercentage);
    } else {
        printf("No words to check.\n");
    }
}

int main() {
    // Common words
    char *commonWords[COMMON_WORDS_COUNT] = {
        "like", "more", "word", "words", "less", "greater", "four", "five", "eight",
        "point", "example", "some", "three", "many", "which", "when",
        "that", "there", "with", "where"
    };
    
    // Simulated source documents
    char *sources[] = {
        "This is a source document with some similar text.",
        "Another document with entirely different content.",
        "The student's paper may have been taken from this source."
    };
    int numSources = sizeof(sources) / sizeof(sources[0]);
    
    // Initialize arrays and variables
    char sourceVocabulary[MAX_VOCABULARY][MAX_WORD_LENGTH];
    int vocabCount = 0;
    int wc = 0; // Word count for the student's document
    int unique = 0; // Unique words found

    // Get student's document from user
    char document[1000];
    printf("Enter the student's text:\n");
    fgets(document, sizeof(document), stdin);
    document[strcspn(document, "\n")] = '\0';  // Remove newline character

    // Tokenize the document into words and initialize word count
    char docWords[MAX_WORD_LENGTH * 100];
    strcpy(docWords, document);
    char words[MAX_WORD_LENGTH * 100][MAX_WORD_LENGTH];
    tokenizeText(docWords, words, &wc);

    // Set unique to the initial word count
    unique = wc;

    // Populate the source vocabulary array
    populateSourceVocabulary(sources, numSources, commonWords, COMMON_WORDS_COUNT, sourceVocabulary, &vocabCount);

    // Check plagiarism and calculate percentage
    checkPlagiarism(document, sourceVocabulary, vocabCount, &unique);

    return 0;
}
