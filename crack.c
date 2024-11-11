#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"
#include "fileutil.h"

#define PASS_LEN 50     // Maximum length any password will be.
#define HASH_LEN 33     // Length of hash plus one for null.

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        exit(1);
    }

    // Read the hashes file into an array.
    int size;
    char **hashes = loadFileAA(argv[1], &size);

    // Open the password file for reading.
    
	FILE *passwords = fopen(argv[2], "r");
	if (!passwords)
	{
	    perror("Can't open file");
	    exit(1);
	}
    
	char string[PASS_LEN];
    int hashesFound = 0;
	while (fgets(string, PASS_LEN, passwords) != NULL)
	{
		// trim the newline
		for (int i = 0; i < strlen(string); i++)
			if (string[i] == '\n')
				string[i] = '\0';

        // hash the string and search for it
        char *hash = md5(string, strlen(string));
		char *found = substringSearchAA(hash, hashes, size);
        if (found)
        {
            printf("Hash found: %s\n", found);
            hashesFound++;
        }
    }

    // Close the file
	fclose(passwords);
    // Display the number of hashes found.
    printf("Hashes found: %d\n", hashesFound);
    // Free up memory.
    freeAA(hashes, size);
}
