#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

typedef struct TrieNode {
    struct TrieNode *child[2];
    char* prefix;
} TrieNode;

Node* createNode(int val) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = val;
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node* root, int val) {
    if (root == NULL) return createNode(val);
    if (val < root->data) root->left = insert(root->left, val);
    else if (val > root->data) root->right = insert(root->right, val);
    return root;
}

int getHeight(Node* root) {
    if (root == NULL) return -1;
    int l = getHeight(root->left);
    int r = getHeight(root->right);
    return (l > r ? l : r) + 1;
}

TrieNode* createTrieNode() {
    return (TrieNode*)calloc(1, sizeof(TrieNode));
}

void insertPrefix(TrieNode* root, unsigned int ip, int len, char* original) {
    TrieNode* curr = root;
    for (int i = 31; i >= (32 - len); i--) {
        int bit = (ip >> i) & 1;
        if (!curr->child[bit]) curr->child[bit] = createTrieNode();
        curr = curr->child[bit];
    }
    curr->prefix = strdup(original);
}

char* lpmSearch(TrieNode* root, unsigned int ip) {
    TrieNode* curr = root;
    char* best = "Nenhum";
    for (int i = 31; i >= 0; i--) {
        int bit = (ip >> i) & 1;
        if (!curr || !curr->child[bit]) break;
        curr = curr->child[bit];
        if (curr->prefix) best = curr->prefix;
    }
    return best;
}

int main() {
    int Ns[] = {100, 1000, 10000};
    printf("=== GRUPO 1 ===\n");
    for(int i = 0; i < 3; i++) {
        Node* root = NULL;
        double start = omp_get_wtime();
        for(int j = 0; j < Ns[i]; j++) root = insert(root, rand() % 50000);
        printf("N=%d | Tempo: %.6fs | Altura: %d\n", Ns[i], omp_get_wtime() - start, getHeight(root));
    }

    printf("\n=== GRUPO 3 ===\n");
    int count = 0;
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+:count) schedule(dynamic)
    for(int i = 2; i <= 50000; i++) {
        bool prime = true;
        for(int j = 2; j * j <= i; j++) {
            if(i % j == 0) {
                prime = false;
                break;
            }
        }
        if(prime) count++;
    }
    printf("Primos: %d | Tempo: %.6fs\n", count, omp_get_wtime() - start);

    printf("\n=== GRUPO 4 ===\n");
    TrieNode* trie = createTrieNode();
    insertPrefix(trie, 0xC0A80000, 16, "192.168.0.0/16");
    insertPrefix(trie, 0xC0A80100, 24, "192.168.1.0/24");
    printf("IP 192.168.1.10 -> Match: %s\n", lpmSearch(trie, 0xC0A8010A));

    return 0;
}
