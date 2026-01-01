#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

typedef struct Node {
    // int bit_index;
    struct Node *is_zero; // left child
    struct Node *is_one; // right child
    uint32_t prefix;
    int prefix_length;
    bool is_prefix;
    uint32_t next_hop;
} Node;

Node *create_node(uint32_t prefix, int prefix_length, bool is_prefix, uint32_t next_hop) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    // new_node->bit_index = bit_index;
    new_node->is_zero = NULL;
    new_node->is_one = NULL;
    new_node->prefix = prefix;
    new_node->prefix_length = prefix_length;
    new_node->is_prefix = is_prefix;
    new_node->next_hop = next_hop;
    return new_node;
}

typedef struct CompressedTrie {
    Node* root;
} CompressedTrie;

Node *create_node(uint32_t prefix, int prefix_length, bool is_prefix, uint32_t next_hop);
CompressedTrie *create_trie(void);
void insert(CompressedTrie *trie, const char *cidr, uint32_t next_hop);
// bool lookup(CompressedTrie *trie, uint32_t ip, uint32_t *next_hop);
// uint32_t lookup_ip(CompressedTrie *trie, const char *ip_str);
void parse_prefix(const char *cidr, uint32_t *ip, int *prefix_len);
void print_ip(uint32_t ip);
void print_ip_binary(uint32_t ip);
void print_ip_addr_string_and_binary(const char *cidr, uint32_t ip);
static inline uint32_t get_bits(uint32_t ip, int pos, int len);
static int common_prefix_len(uint32_t a, uint32_t b, int max_len);
static void split_node(Node *node, uint32_t ip, int pos, int plen, uint32_t nh);

CompressedTrie *create_trie() {
    CompressedTrie* trie = (CompressedTrie*)malloc(sizeof(CompressedTrie));
    if (!trie) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    trie->root = create_node(0, 0, false, 0);
    return trie;
}

void insert(CompressedTrie *trie, const char *cidr, uint32_t next_hop) {
    uint32_t ip;
    int prefix_length;

    parse_prefix(cidr, &ip, &prefix_length);
    print_ip_addr_string_and_binary(cidr, ip);

    Node *current = trie->root;
    int pos = 0;

    while(1) {
        if (current->prefix_length > 0) {
            uint32_t bits = get_bits(ip, pos, current->prefix_length);
            if (bits != current->prefix) {
                split_node(current, ip, pos, prefix_length, next_hop);
                return;
            }
            pos += current->prefix_length;
        }

        if (pos == prefix_length) {
            current->is_prefix = true;
            current->next_hop = next_hop;
            return;
        }

        // If the current node does not have childrens
        int bit = (ip >> (31 - pos)) & 1;
        Node **next = bit ? &current->is_one : &current->is_zero;

        if (!*next) {
            int len = prefix_length - pos;
            uint32_t bits = get_bits(ip, pos, len);
            *next = create_node(bits, len, true, next_hop);
            return;
        }
        current = *next;
    }

    return;
}

static inline uint32_t get_bits(uint32_t ip, int pos, int len) {
    if (len == 0) return 0;
    return (ip >> (32 - pos - len)) & ((1u << len) - 1);
}

static int common_prefix_len(uint32_t a, uint32_t b, int max_len) {
    for (int i = 0; i < max_len; i++) {
        int bit_a = (a >> (max_len - 1 - i)) & 1;
        int bit_b = (b >> (max_len - 1 - i)) & 1;
        if (bit_a != bit_b)
            return i;
    }
    return max_len;
}

static void split_node(Node *node, uint32_t ip, int pos, int prefix_length, uint32_t nh) {
    uint32_t incoming = get_bits(ip, pos, node->prefix_length);
    int common = common_prefix_len(node->prefix, incoming, node->prefix_length);

    /* Create parent with common bits */
    uint32_t common_prefix = node->prefix >> (node->prefix_length - common);
    Node *parent = create_node(common_prefix, common, false, 0);

    /* Adjust existing node */
    int old_suffix_len = node->prefix_length - common;
    node->prefix &= (1u << old_suffix_len) - 1;
    node->prefix_length = old_suffix_len;

    int old_bit = (node->prefix >> (old_suffix_len - 1)) & 1;
    if (old_bit == 0) {
        parent->is_zero = node;
    } else {
        parent->is_one  = node;
    }

    /* Create new leaf for incoming prefix */
    int new_len = prefix_length - pos - common;
    uint32_t new_bits = get_bits(ip, pos + common, new_len);

    Node *leaf = create_node(new_bits, new_len, true, nh);
    int new_bit = (new_bits >> (new_len - 1)) & 1;

    if (new_bit == 0) {
        parent->is_zero = leaf;
    } else {
        parent->is_one  = leaf;
    }             
    
    /* Replace node contents */
    *node = *parent;
    free(parent);
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

int main() {
    CompressedTrie* trie = create_trie();
    insert(trie,"10.10.10.0/8", 1);
    insert(trie, "10.10.10.0/24", 2);
    insert(trie, "10.10.11.0/24", 3);
    return 0;
}