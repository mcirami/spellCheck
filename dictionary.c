/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

#define TABLESIZE 800

// to set node elements
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
    
}node;

// for string compare
bool string_comp(const char* s, const char* t);

// to hash word
unsigned int hash(const char* s);

//to alloc new node for hash table
node* node_alloc(void);

// to initialize hash table
void hashtable_init(void);

// to keep track of words in dictionary
volatile int counter = 0;

// to create hash table
node* hashtable[TABLESIZE];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // to set hash value of word
    const unsigned int index = hash(word);
    
    // set pointers move through linked list
    register node* current = hashtable[index];
    register node* previous = NULL;
    
    // move through linked list
    while (current->next != NULL)
    {
        previous = current;
        current = current->next;
        
        // calls function to compare word from text to word in hash table
        bool found = string_comp(word, current->word);
        if (found)
        {
           return true;
        }
    }
    
    return false;

}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{   
    // open dictionary file
    register FILE* inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        return false;
    }
    
    // call function to initialize hash table
    hashtable_init();
    
    // go through dictionary file adding each word to hash table
    while(!feof(inptr))
    {
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Could not create new node\n");
            return false;
        }
        
        fscanf(inptr,"%s", new_node->word);
        
        if (feof(inptr))
        {
            free(new_node);
            fclose(inptr);
            
            return true;
        }
        else
        {
            counter++;
        }
        
        // to set hash value of word 
        const unsigned int index = hash(new_node->word);
        
        // add word to hashtable
        if (hashtable[index]->next == NULL)
        {
            hashtable[index]->next = new_node;
            new_node->next = NULL;
        }
        else
        {
            node* temp = hashtable[index]->next;
            hashtable[index]->next = new_node;
            new_node->next = temp;
        }
    }
    
    return false;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return number of words in dictionary
    return counter;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // iterate through whole hash table freeing all nodes
    for (int i = 0; i < TABLESIZE; i++)
    {
        register node* cursor = hashtable[i];
        
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        
    }
    
    return true;
}


/**
 * Allocate memory for each node
 */
node* node_alloc(void)
{
    node* temp = malloc(sizeof(node));
    
    if (temp == NULL)
    {
        return NULL;
    }
        
    temp->next = NULL;
    
    return temp;
}

/**
 * Initialize each element in hash table with node
 */
void hashtable_init(void)
{
    for (int x = 0; x < TABLESIZE; x++)
    {
        hashtable[x] = node_alloc();
        if (hashtable[x] == NULL)
        {
            printf("Hash table could not be allocated\n");
        }
    }
}

/**
 * Hash word and return value
 * Hash function from http://stackoverflow.com/questions/2962207/constructing-a-hash-table-hash-function
*/
unsigned int hash(const char* s) 
{
    register unsigned int num = 0;
    
	while(*s++ != '\0')
	{
        num += tolower(*s);
	}		
    num = num % TABLESIZE;

    return num;
}

/**
 * Compare word from text to word in dictionary
*/
bool string_comp(const char* s, const char* t)
{
    if (s != NULL && t != NULL)
    {      
        if (strcasecmp(s, t) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    return false;
}


