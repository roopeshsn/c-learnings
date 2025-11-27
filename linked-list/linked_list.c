#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BGPPathNode {
    int id;
    char neighbor[16];
    int local_preference;
    int as_path_length;
    char next_hop[16];
    struct BGPPathNode *next;
} BGPPathNode;

BGPPathNode* create_node(int id, char *neighbor, int local_preference, int as_path_length, char *next_hop) {
    BGPPathNode *new_node = malloc(sizeof(BGPPathNode));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE); 
    }
    new_node->id = id;
    strncpy(new_node->neighbor, neighbor, 16);
    strncpy(new_node->next_hop, next_hop, 16);
    new_node->local_preference = local_preference;
    new_node->as_path_length = as_path_length;
    new_node->next = NULL;
    return new_node;
}

void traverse_list(BGPPathNode *head) {
    BGPPathNode *current = head;
    while(current != NULL) {
        printf("%d -> ", current->id);
        // printf("Neighbor: %s, Local Preference: %d, AS Path Length: %d, Next Hop: %s\n",
        //        current->neighbor, current->local_preference, current->as_path_length, current->next_hop);
        current = current->next;
    }
    printf("NULL");
    printf("\n");
}

int is_better(BGPPathNode *a, BGPPathNode *b) {
    if (a->local_preference > b->local_preference){
        return 1;
    } else {
        return 0;
    }

    // If local-pref is equal, shorter AS path is better
    if (a->as_path_length < b->as_path_length) {
        return 1;
    } else {
        return 0;
    }

    return 0;
}

void reorder_list(BGPPathNode **head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    // Considering head as the best path node
    BGPPathNode *best_path_node = *head;
    BGPPathNode *best_path_node_prev = NULL;
    BGPPathNode *prev = *head;
    BGPPathNode *curr = (*head)->next;

    while (curr != NULL) {
        if(is_better(curr, best_path_node)) {
            best_path_node = curr;
            best_path_node_prev = prev;
        } 
        prev = curr;
        curr = curr->next;
    }

    if (best_path_node == *head) {
        return;
    }

    // Linking best previous to the best next
    best_path_node_prev->next = best_path_node->next;
    // Linking best to head and making best as head
    best_path_node->next = *head;
    *head = best_path_node;
}

int main() {
    BGPPathNode *head;
    BGPPathNode *p1 = create_node(1, "10.10.10.1", 100, 3, "10.10.10.1");
    BGPPathNode *p2 = create_node(2, "10.10.10.2", 200, 3, "10.10.10.2");
    BGPPathNode *p3 = create_node(3, "10.10.10.3", 100, 2, "10.10.10.3");

    head = p1;
    p1->next = p2;
    p2->next = p3;

    traverse_list(head);
    reorder_list(&head);
    traverse_list(head);

    return 0;
}