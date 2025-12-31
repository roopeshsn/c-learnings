#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

typedef struct Node {
    int bit_index;
    struct Node *is_zero; // left child
    struct Node *is_one; // right child
    uint32_t prefix;
    int prefix_length;
    bool is_prefix;
    uint32_t next_hop;
} Node;

Node* create_node(int bit_index, uint32_t prefix, int prefix_length, bool is_prefix, uint32_t next_hop) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->bit_index = bit_index;
    new_node->is_zero = NULL;
    new_node->is_one = NULL;
    new_node->prefix = prefix;
    new_node->prefix_length = prefix_length;
    new_node->is_prefix = is_prefix;
    new_node->next_hop = next_hop;
    return new_node;
}

typedef struct BinaryTrie {
    Node* root;
} BinaryTrie;

Node *create_node(int bit_index, uint32_t prefix, int prefix_length, bool is_prefix, uint32_t next_hop);
BinaryTrie *create_trie(void);
BinaryTrie *insert(BinaryTrie *trie, const char *cidr, uint32_t next_hop);
bool lookup(BinaryTrie *trie, uint32_t ip, uint32_t *next_hop);
uint32_t lookup_ip(BinaryTrie *trie, const char *ip_str);
void parse_prefix(const char *cidr, uint32_t *ip, int *prefix_len);
void print_ip(uint32_t ip);
void print_ip_binary(uint32_t ip);
void print_ip_addr_string_and_binary(const char *cidr, uint32_t ip);

BinaryTrie *create_trie() {
    BinaryTrie* trie = (BinaryTrie*)malloc(sizeof(BinaryTrie));
    if (!trie) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    trie->root = create_node(-1, 0, 0, false, 0);
    return trie;
}

BinaryTrie *insert(BinaryTrie *trie, const char *cidr, uint32_t next_hop) {
    uint32_t ip;
    int prefix_len;

    parse_prefix(cidr, &ip, &prefix_len);
    print_ip_addr_string_and_binary(cidr, ip);

    Node *current = trie->root;

    for (int i = 0; i < prefix_len; i++) {
        int bit = (ip >> (31 - i)) & 1;

        if (bit == 0) {
            if (current->is_zero == NULL) {
                current->is_zero = create_node(i, 0, 0, false, 0);
            }
            current = current->is_zero;
        } else {
            if (current->is_one == NULL) {
                current->is_one = create_node(i, 0, 0, false, 0);
            }
            current = current->is_one;
        }
    }

    current->is_prefix = true;
    current->prefix = ip;
    current->prefix_length = prefix_len;
    current->next_hop = next_hop;

    return trie;
}

bool lookup(BinaryTrie *trie, uint32_t ip, uint32_t *next_hop) {
    Node *current = trie->root;
    bool found = false;

    for (int i = 0; i < 32; i++) {
        if (current == NULL)
            break;

        if (current->is_prefix) {
            *next_hop = current->next_hop;
            found = true;
        }

        int bit = (ip >> (31 - i)) & 1;

        if (bit == 0)
            current = current->is_zero;
        else
            current = current->is_one;
    }

    return found;
}

void list_prefixes(Node *node, uint32_t prefix, int depth) {
    if (node == NULL)
        return;

    if (node->is_prefix) {
        uint32_t ip = prefix << (32 - depth);

        printf("Prefix: ");
        print_ip(ip);
        printf("/%d\n", depth);
    }

    list_prefixes(node->is_zero, prefix << 1, depth + 1);
    list_prefixes(node->is_one, (prefix << 1) | 1, depth + 1);
}


void parse_prefix(const char *cidr, uint32_t *ip, int *prefix_len) {
    char ip_str[32];
    sscanf(cidr, "%[^/]/%d", ip_str, prefix_len);

    struct in_addr addr;
    inet_pton(AF_INET, ip_str, &addr);
    *ip = ntohl(addr.s_addr); // host byte order
}

void print_ip(uint32_t ip) {
    char buf[INET_ADDRSTRLEN];
    struct in_addr addr = { .s_addr = htonl(ip) };
    inet_ntop(AF_INET, &addr, buf, sizeof(buf));
    printf("%s", buf);
}

void print_ip_binary(uint32_t ip) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (ip >> i) & 1);

        /* Optional: add separators every 8 bits */
        if (i % 8 == 0 && i != 0)
            printf(" ");
    }
    printf("\n");
}

void print_ip_addr_string_and_binary(const char *cidr, uint32_t ip) {
    printf("IP Addr: %s ", cidr);
    printf("Binary: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (ip >> i) & 1);

        /* Optional: add separators every 8 bits */
        if (i % 8 == 0 && i != 0)
            printf(" ");
    }
    printf("\n");
}

uint32_t lookup_ip(BinaryTrie *trie, const char *ip_str) {
    struct in_addr addr;
    inet_pton(AF_INET, ip_str, &addr);
    uint32_t ip = ntohl(addr.s_addr);

    uint32_t next_hop;
    if (lookup(trie, ip, &next_hop)) {
        return next_hop;
    }

    return 0; // no route found
}

int main() {
    BinaryTrie* trie = create_trie();

    // Inserting prefixes
    insert(trie,"10.10.10.0/8", 1);
    insert(trie, "10.10.10.0/24", 2);
    insert(trie, "10.10.11.0/24", 3);

    // Lookup IPs
    uint32_t nh;
    char test_ip_addr[INET_ADDRSTRLEN];

    strcpy(test_ip_addr, "10.10.10.10");
    nh = lookup_ip(trie, test_ip_addr);
    printf("IP Addr: %s, Next hop: %u\n", test_ip_addr, nh);

    strcpy(test_ip_addr, "10.10.10.254");
    nh = lookup_ip(trie, test_ip_addr);
    printf("IP Addr: %s, Next hop: %u\n", test_ip_addr, nh);

    strcpy(test_ip_addr, "10.10.12.10");
    nh = lookup_ip(trie, test_ip_addr);
    printf("IP Addr: %s, Next hop: %u\n", test_ip_addr, nh);

    // List all prefixes
    list_prefixes(trie->root, 0, 0);

    return 0;
}