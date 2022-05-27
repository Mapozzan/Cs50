// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH+1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

int sum = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor = table[hash(word)];

    if (strcasecmp(cursor->word, word) == 0)
    {
        return true;
    }
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

//After try a lot hash funtion, and nothing work's well, I decide use a simple one.
unsigned int hash(const char *word)
{
    int n = (int) tolower(word[0]) - 97;
    return n;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // opens the dictionary and initializes temporary space to hold the words
    FILE *file = fopen(dictionary, "r");
    
    char palavra[LENGTH+1];
    
    if (file == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", palavra) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, palavra);
        sum++;
        n->next = table[hash(palavra)];
        table[hash(palavra)] = n;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return sum;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // creates two pointers to traverse the linked list and cancel its element without losing its address
    node *temp;
    node *cursor;

    // repeats for every index in the table
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }

        cursor = table[i];
        temp = cursor;

        // until the end of the list keeps freeing the memory allocated in load
        while (cursor->next != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
        free(cursor);
    }
    return true;
}
