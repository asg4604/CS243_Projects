// Author: AbuBakr Ghaznavi
/// File: link_node.h
/// Description: Defines the functions and data necessary for a linked list
#ifndef LINK_NODE_H
#define LINK_NODE_H

/// Defines the structure for a link_node
typedef struct link_node {
	/// The pointer to data for the link_node 
	void *data;
	/// The pointer to the next node
	struct link_node* next;
} link_node_t;

/// Creates the link node with a pointer to some data
/// @param data
/// @return the new link node 
link_node_t *create_node(void *data);

/// Appends a link node to the end of a list
/// @param head The list that's being appended to
/// @param data the data that's being appended
link_node_t *append_node(link_node_t *head, void *data); 

/// Removes a node from the front a void and returns the data
/// @param head the list that was removed from
/// @return The node that was removed from
void *remove_from_front(link_node_t *head); 

/// Prints the list 
/// @param head The list that is being printed
/// @param pfn The pointer to the function for the printing
void printList(link_node_t *head, void (*pfn)(void *)); 

#endif
