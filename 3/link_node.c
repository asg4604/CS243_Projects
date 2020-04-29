/// Name: AbuBakr Ghaznavi
/// File: link_node.c
/// Description: Defines the operations for a linked_list
#include <stdio.h>
#include <stdlib.h>
#include "link_node.h"

/// Creates a new node and puts in the data
link_node_t *create_node(void *data) {
	link_node_t *new_node = (link_node_t*) malloc(sizeof(link_node_t));
	if (new_node == NULL) {
		// TODO error that there is no more space left
	}
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

// Appends the link node to the end of the list
// Returns the pointer to the end
link_node_t *append_node(link_node_t *head, void *data) {
	link_node_t *new_node = create_node(data);
	// If the head of the node is null, then make this a new list
	// This will be beginning and end of the list
	if (head == NULL) {
		return new_node;
	}

	// Traverses down the list
	link_node_t *current_node = head;
	link_node_t *next_node = current_node->next;
	while (next_node != NULL) {
		next_node = current_node->next;
		if (next_node == NULL) break;
		// Experimental
		current_node = next_node;
	}
	// Reaches the end of the list
	current_node->next = new_node;
	return new_node;
}

// Return the pointer to the data
// Not working at the moment
void *remove_from_front(link_node_t *head) {
	if (head == NULL) {
		return NULL;	
	}
	void *data_from_removed = head->data;
	link_node_t *node_to_remove = head;
	head = head->next;
	free(node_to_remove);
	return data_from_removed;
}

// Applies the print function (pfn) to every element in the list
void printList(link_node_t *head, void (*pfn)(void *)) {
	link_node_t *current_node = head;
	while (current_node != NULL) {
		pfn(current_node->data);
		current_node = current_node->next;
	}
	return;
}
