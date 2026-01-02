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
uint32_t lookup_ip(CompressedTrie *trie, const char *ip_str);
bool lookup(Node *root, uint32_t ip, uint32_t *next_hop);
void parse_prefix(const char *cidr, uint32_t *ip, int *prefix_len);
void print_ip(uint32_t ip);
void print_ip_binary(uint32_t ip);
void print_ip_addr_string_and_binary(const char *cidr, uint32_t ip);
static inline uint32_t get_bits(uint32_t ip, int pos, int len);
static int common_prefix_len(uint32_t a, uint32_t b, int max_len);
static void split_node(Node **slot, Node *node, uint32_t ip, int pos, int plen, uint32_t nh);
void list_prefixes(Node *node, uint32_t acc_bits, int acc_len);
void print_prefix(uint32_t ip, int len, uint32_t nh);

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

    Node **slot = &trie->root;
    Node *current = trie->root;
    int pos = 0;

    while(1) {
        if (current->prefix_length > 0) {
            uint32_t bits = get_bits(ip, pos, current->prefix_length);
            if (bits != current->prefix) {
                split_node(slot, current, ip, pos, prefix_length, next_hop);
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
        slot = bit ? &current->is_one : &current->is_zero;

        if (!*slot) {
            int len = prefix_length - pos;
            uint32_t bits = get_bits(ip, pos, len);
            *slot = create_node(bits, len, true, next_hop);
            return;
        }
        current = *slot;
    }

    return;
}

uint32_t lookup_ip(CompressedTrie *trie, const char *ip_str) {
    struct in_addr addr;
    inet_pton(AF_INET, ip_str, &addr);
    uint32_t ip = ntohl(addr.s_addr);

    uint32_t next_hop;
    Node *root = trie->root;
    if (lookup(root, ip, &next_hop)) {
        return next_hop;
    }

    return 0; // no route found
}

bool lookup(Node *root, uint32_t ip, uint32_t *next_hop) {
    Node *current = root;
    int pos = 0;
    bool found = false;

    while (current) {
        // Match compressed bits at this node
        if (current->prefix_length > 0) {
            uint32_t bits = get_bits(ip, pos, current->prefix_length);
            if (bits != current->prefix) {
                break;
            }
            pos += current->prefix_length;
        }

        if (current->is_prefix) {
            *next_hop = current->next_hop;
            found = true;
        }

        if (pos == 32)
            break;

        int bit = (ip >> (31 - pos)) & 1;
        current = (bit == 0) ? current->is_zero : current->is_one;
    }
    return found;
}

static inline uint32_t get_bits(uint32_t ip, int pos, int len) {
    if (len == 0) return 0;
    return (ip >> (32 - pos - len)) & ((1u << len) - 1);
}

// The common_prefix_len function returns the length of the common prefix
static int common_prefix_len(uint32_t a, uint32_t b, int max_len) {
    for (int i = 0; i < max_len; i++) {
        int bit_a = (a >> (max_len - 1 - i)) & 1;
        int bit_b = (b >> (max_len - 1 - i)) & 1;
        if (bit_a != bit_b)
            return i;
    }
    return max_len;
}

static void split_node(Node **slot, Node *node, uint32_t ip, int pos, int prefix_length, uint32_t nh) {
    uint32_t incoming = get_bits(ip, pos, node->prefix_length);
    int common = common_prefix_len(node->prefix, incoming, node->prefix_length);

    // Create parent with common bits.
    uint32_t common_prefix = node->prefix >> (node->prefix_length - common);
    Node *parent = create_node(common_prefix, common, false, 0);

    // Adjust existing node
    int old_suffix_len = node->prefix_length - common;
    
    // 1u means unsigned 1 (non-negative)
    // &= is a bitwise AND assignment operator
    // x &= y is equivalent to x = x & y
    node->prefix &= (1u << old_suffix_len) - 1; // node->prefix = node->prefix & (1u << old_suffix_len) - 1;
    node->prefix_length = old_suffix_len;

    int old_bit = (node->prefix >> (old_suffix_len - 1)) & 1;
    if (old_bit == 0) parent->is_zero = node;
    else              parent->is_one  = node;

    // New leaf
    int new_len = prefix_length - pos - common;
    uint32_t new_bits = get_bits(ip, pos + common, new_len);
    Node *leaf = create_node(new_bits, new_len, true, nh);

    int new_bit = (new_bits >> (new_len - 1)) & 1;
    if (new_bit == 0) parent->is_zero = leaf;
    else              parent->is_one  = leaf;

    *slot = parent;
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

void print_prefix(uint32_t ip, int len, uint32_t nh) {
    struct in_addr addr = { .s_addr = htonl(ip) };
    char buf[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &addr, buf, sizeof(buf));
    printf("%s/%d -> next-hop %u\n", buf, len, nh);
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

void list_prefixes(Node *node, uint32_t acc_bits, int acc_len) {
    if (!node) return;

    // Append this node's prefix bits
    uint32_t new_bits = acc_bits;
    int new_len = acc_len;

    if (node->prefix_length > 0) {
        new_bits = (acc_bits << node->prefix_length) | node->prefix;
        new_len  = acc_len + node->prefix_length;
    }

    if (node->is_prefix) {
        uint32_t ip = new_bits << (32 - new_len);
        print_prefix(ip, new_len, node->next_hop);
    }

    list_prefixes(node->is_zero, new_bits, new_len);
    list_prefixes(node->is_one,  new_bits, new_len);
}

int main() {
    CompressedTrie* trie = create_trie();

    // Insert prefixes
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

    list_prefixes(trie->root, 0, 0);

    return 0;
}