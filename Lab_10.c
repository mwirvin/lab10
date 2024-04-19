#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26 
// Trie node structure
struct Trie {
    struct Trie *children[ALPHABET_SIZE];
    int count; // To store the frequency of the word
};

// Initializes a new trie node
struct Trie *getNewTrie() {
    struct Trie *pNode = (struct Trie *)malloc(sizeof(struct Trie));
    if (pNode) {
        int i;
        pNode->count = 0;
        for (i = 0; i < ALPHABET_SIZE; i++) {
            pNode->children[i] = NULL;
        }
    }
    return pNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *root, char *word) {
    struct Trie *pCrawl = root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = getNewTrie();
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *root, char *word) {
    struct Trie *pCrawl = root;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
        word++;
    }
    return pCrawl->count;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i])
            deallocateTrie(root->children[i]);
    }
    free(root);
}


struct Trie *createTrie() {
    return getNewTrie();
}
// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords){
    FILE* inFile = fopen(filename, "r");
    if (inFile == NULL) return -1;

    int count;
    fscanf(inFile, "%d", &count); 

    char word[100];
    for (int i = 0; i < count; i++) {
        fscanf(inFile, "%s", word);
        pInWords[i] = (char *)calloc(strlen(word) + 1, sizeof(char));
        strcpy(pInWords[i], word);
    }

    fclose(inFile);
    return count;
}

int main(void) {
    char *inWords[256]; 

    int numWords = readDictionary("dictionary.txt", inWords);
    if (numWords < 0) {
        printf("Failed to read dictionary\n");
        return 1;
    }

    struct Trie *root = createTrie();

    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }

    char *testWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("%s : %d\n", testWords[i], numberOfOccurances(root, testWords[i]));
    }

    deallocateTrie(root);
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
