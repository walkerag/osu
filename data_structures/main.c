#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);

    while (1)
    {
        char c = fgetc(file);
        if( (c >= 'A' && c <= 'Z') ) c+=32;
        if ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')  
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            // .............
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    
    // Open file
    FILE* a_file;
    a_file = fopen(fileName,"r");

    clock_t timer = clock();
    
    HashMap* map = hashMapNew(10);
    
    // --- Concordance code begins here ---

    int wordCount;
    char* fileWord = nextWord(a_file);
    while( fileWord!=NULL ){

        // Get word value
        int* wordValue = hashMapGet(map, fileWord);

        // Update word count
        if( wordValue==0 ) {

        	wordCount = 0;

        } else {

        	wordCount = (*wordValue)++;

        }

        // Add file word with updated word count
        wordCount++;
        hashMapPut(map, fileWord , wordCount);

        // Free word
        free(fileWord);

    	// Get next word
    	fileWord = nextWord(a_file);

    }

    // --- Concordance code ends here ---
    
    // Close file
    fclose(a_file);

    hashMapPrint(map);
    
    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));
    
    hashMapDelete(map);
    return 0;
}
