#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        // Make case insensitive
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
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{

	char* fileWord = nextWord(file);
	int wordCount;
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
		fileWord = nextWord(file);

	}

}

// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
int levenshtein(char *s1, char *s2) {
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++) {
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
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

	HashMap* map = hashMapNew(1000);
    
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {

        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        
        // Make word lower case
        // https://www.dreamincode.net/forums/topic/222380-going-through-each-element-of-a-char-array-in-c/
        for(int i = 0; inputBuffer[i] != '\0'; ++i) {

        	if( (inputBuffer[i] >= 'A' && inputBuffer[i] <= 'Z') ) inputBuffer[i]+=32;

        }

        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;

        } else if( hashMapContainsKey(map,inputBuffer) ){

        	printf("The inputted word %s is spelled correctly\n",inputBuffer);

        } else {

        	int distArray[5] = {1000,1000,1000,1000,1000};
        	char wordArray[5][256];

        	// Iterate over dictionary, find 5 smallest distances
        	int levDistance;
        	for(int i=0; i<map->capacity; i++){

        		struct HashLink* searchStart = map->table[i];
        		while( searchStart!=0 ){

        			levDistance = levenshtein(inputBuffer,searchStart->key);

        			for(int i=0; i<5; i++){

        				if( levDistance<distArray[i] ){

        					distArray[i] = levDistance;
        	        		strcpy(wordArray[i], searchStart->key);
        	        		levDistance = 1000;

        				}

        			}

        			searchStart = searchStart->next;

        		}

        	}

        	// Return 5 to user
        	printf("The inputted word %s is spelled incorrectly\n",inputBuffer);
        	printf("Did you mean %s", wordArray[0]);
        	for(int i=1; i<5; i++){printf(", %s",wordArray[i]);}
        	printf("?\n");

        }
        
    }
    
    hashMapDelete(map);
    return 0;
}

