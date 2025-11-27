#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

Node* hashTable[TABLE_SIZE];


unsigned int fnv1a_hash(const char *key) {
    unsigned int hash = 2166136261u;
    while (*key) {
        hash ^= (unsigned char)*key; 
        hash *= 16777619;  
        key++;
    }
    return hash;
}

// Reference: https://en.wikipedia.org/wiki/Jenkins_hash_function
unsigned int jenkins_one_at_a_time_hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash += (unsigned char)*key++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

unsigned int hash_index(const char *key) {
    return jenkins_one_at_a_time_hash(key) % TABLE_SIZE;
}

void insert(const char *key, int value) {
    unsigned int index = hash_index(key);
    Node *head = hashTable[index]; // If there is no node on the index, then head will be NULL as hashTable[index] is NULL

    // Check if key already exists, update value
    Node *current = head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // Key does not exist, create new node
    Node *newNode = malloc(sizeof(Node));
    newNode->key = strdup(key);
    newNode->value = value;
    newNode->next = head;
    hashTable[index] = newNode;
}

Node* get(const char *key) {
    unsigned int index = hash_index(key);
    Node *current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int delete(const char *key) {
    unsigned int index = hash_index(key);
    Node *current = hashTable[index];
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                hashTable[index] = current->next;
            }
            free(current->key);
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    return 0;
}

void free_all() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable[i];
        while (current != NULL) {
            Node *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
        hashTable[i] = NULL;
    }
}

void print_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable[i];
        if (current) {
            printf("Index %d: ", i);
            while (current != NULL) {
                printf("(%s: %d) -> ", current->key, current->value);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

int main() {
    insert("apple", 10);
    insert("banana", 20);
    insert("grape", 30);
    insert("melon", 30);

    Node *node = get("banana");
    if (node) {
        printf("Key: %s, Value: %d\n", node->key, node->value);
    } else {
        printf("Key not found\n");  
    }

    print_table();

    free_all();
    return 0;
}