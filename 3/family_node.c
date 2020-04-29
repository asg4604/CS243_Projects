/// Author: AbuBakr Ghaznavi
/// File: family_node.c
/// Description: Implements the necessary operations and functions
/// 			 to perform on trees and family_nodes
#include "family_node.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
#define NUM_CHILDREN 100
/// Copies a string with new memory
char* strdup (const char* s)
{
  size_t slen = strlen(s);
  char* result = malloc(slen + 1);
  if(result == NULL)
  {
    return NULL;
  }

  memcpy(result, s, slen+1);
  return result;
}
	
// Checks if a child already exists in a parent's chilldren collection
bool check_child(family_node_t tree, char *parent_name, char *child_name) {
	family_node_t parent_node = find_node(tree, parent_name);
	for (int x = 0; x < parent_node->num_children; x++) {
		if (strcmp(child_name, parent_node->children[x]->name) == 0) {
			return true;
		}
	}
	return false;
}

/// Creates a new family_node
family_node_t create_f_node(char *name) {
	family_node_t new_fam_node = (family_node_t) malloc( sizeof(family_node_s) );
	new_fam_node->name = strdup(name);
	new_fam_node->num_children = 0;
	new_fam_node->capacity = NUM_CHILDREN;
	new_fam_node->children = (family_node_t *) malloc( sizeof(family_node_t) * new_fam_node->capacity);
	// Initializing all of the children to null
	for (int x = 0; x < NUM_CHILDREN; x++) {
		new_fam_node->children[x] = NULL;
	}
	return new_fam_node;
}

/// Tests if strings are equal
bool streq(char *a, char *b) {
	return strcmp(a, b) == 0;
}

/// Uses void pointers to test if two strings are equal
bool equivalent_names(void *name1, void *name2) {
	char *a = (char *) name1;
	char *b = (char *) name2;
	return streq(a, b);
}

/// Finds a family node
family_node_t find_node(family_node_t tree, char *name) {
	queue_t queue = que_create(equivalent_names);
	que_enqueue(queue, tree);
	if (tree == NULL) {
		fprintf(stderr,"error: trying to find from empty tree.\n");
		return NULL;
	}
	if (name == NULL) {
		return NULL;
	}
	family_node_t front = NULL;
	while ( que_size(queue) > 0 ) {
		front = (family_node_t ) que_front(queue);
		que_dequeue(queue);
		if ( streq(name, front->name) ) {
		// P2	
			que_destroy(queue);
			return front;
		} else {
			for (int x = 0; x < front->num_children; x++) {
				if (front->children[x] != NULL) {
			// DEBUG		printf("DEBUG: Child added to queue %s\n", front->children[x]->name);
					que_enqueue(queue, front->children[x]);
				}
			}
		}
	}
	que_destroy(queue);
	return NULL;
}

/// Adds a child to a parent and reallocs if more spaces is needed
family_node_t add_child(family_node_t tree, char *parent_name, char *child_name) {
	family_node_t parent_node = find_node(tree, parent_name);
	if ( check_child(tree, parent_name, child_name) ) {
		fprintf(stderr, "error: \'%s\' is already a child of \'%s\'.\n", child_name, parent_name);
		return NULL;
	}
	// Double allocation space for children if not enough
	if (parent_node->num_children >= parent_node->capacity) {
		size_t new_size = (sizeof(family_node_s)) * parent_node->capacity * 2;
		parent_node->children = (family_node_t*) realloc(parent_node->children, new_size);
		if (parent_node->children == NULL) {
			// TODO No more memory for child nodes
			fprintf(stderr, "Error: Out of memory to add children to \'%s\'.\n", parent_name);
			exit(EXIT_FAILURE);
		} else {
			parent_node->capacity *= 2;
		}
	}
	parent_node->children[parent_node->num_children++] = create_f_node(child_name);
	return tree;
}

/// Alternative method of destroying a tree for debugging purposes
family_node_t D_destroy_tree( family_node_t tree ) {
	if (tree == NULL) {
		return NULL;
	}
	queue_t queue = que_create(equivalent_names);
	que_enqueue(queue, tree);
	family_node_t front = NULL;
	while ( que_size(queue) > 0 ) {
		front = (family_node_t ) que_front(queue);
//DEBUG		printf("DEBUG: Child freed from queue %s\n", front->name);
		// Freeing before enqueing the children?
// E		free(front->name);
// E		free(front);
		que_dequeue(queue);
			for (int x = 0; x < front->num_children; x++) {
				if (front->children[x] != NULL) {
					que_enqueue(queue, front->children[x]);
				}
			}
		free(front->name);
		free(front);
		}
	que_destroy(queue);
	return NULL;	
}

/// Destroys a tree and frees its memory
family_node_t destroy_tree( family_node_t tree ) {
	// If the tree is NULL do nothing just return
	if ( tree == NULL ) {
		return NULL;
	}
	for (int x = 0; x < tree->num_children; x++) {
		destroy_tree( tree->children[x] );
	}
	free( tree->name );
	free( tree->children );
	free( tree );
	return NULL;
}
/// Prints a family node
void print_family_node( family_node_t family_node ) {
	if ( family_node->num_children == 0 ) {
		printf("%s had no offspring.\n", family_node->name);
		return;
	}
	printf("%s had ", family_node->name);
	for (int x = 0; x < family_node->num_children; x++) {
		if (x == family_node->num_children - 1 && family_node->num_children > 1) {
			printf(" and ");
		}
		printf("%s", family_node->children[x]->name);
		if (x < family_node->num_children - 2) {
			printf(", ");
		}
	}
	printf(".\n");
}
/// Prints the tree starting from a name
void print_tree(family_node_t tree, char *name) {
	family_node_t parent_to_print;
	if (name == NULL) {
		parent_to_print = tree;
	} else {
		parent_to_print = find_node(tree,name);
	}
	// Breadth first traversal
	queue_t queue = que_create(equivalent_names);
	que_enqueue(queue, parent_to_print);
	family_node_t front = NULL;
	while ( que_size(queue) > 0 ) {
		front = (family_node_t ) que_front(queue);
		que_dequeue(queue);
		if ( 1 ) {
			// print family_node			
			print_family_node(front);
			for (int x = 0; x < front->num_children; x++) {
				if (front->children[x] != NULL) {
// DEBUG					// printf("DEBUG: Child added to queue %s\n", front->children[x]->name);
					que_enqueue(queue, front->children[x]);
				}
			}
		}
	}
	que_destroy(queue);
}


// The height function to recurse
int height_r(family_node_t tree) {
	if (tree == NULL) {
		return 0;
	}
	int max_height = 0;
	int cur_height = 0;
	for (int x = 0; x < tree->num_children; x++) {
		cur_height = height_r(tree->children[x]);
		if (cur_height > max_height) {
			max_height = cur_height;
		}
	}
	return 1 + max_height;
}

/// Finds the height of the tree
int height(family_node_t tree, char *name) {
	family_node_t parent_start;
	if (name == NULL) {
		parent_start = tree;
	} else {
		parent_start = find_node(tree, name);
		if (parent_start == NULL) {
			return -1;
		}
	}
	return height_r(parent_start);
}

/// Finds the size of the tree
int size(family_node_t tree) {
	// Breadth first traversal
	if (tree == NULL) {
		return 0;
	}
	int acc = 0;
	queue_t queue = que_create(equivalent_names);
	que_enqueue(queue, tree);
	family_node_t front = NULL;
	while ( que_size(queue) > 0 ) {
		front = (family_node_t ) que_front(queue);
		que_dequeue(queue);
		if ( 1 ) {
			// print family_node			
			acc++;
			for (int x = 0; x < front->num_children; x++) {
				if (front->children[x] != NULL) {
					// printf("DEBUG: Child added to queue %s\n", front->children[x]->name);
					que_enqueue(queue, front->children[x]);
				}
			}
		}
	}
	que_destroy(queue);
	queue = NULL;
	return acc;
}

/// Potential main function to test capabilities of the node
// int main() {
// 	// Queue testing
// 	int a = 1;
// 	int b = 2;
// 	int c = 3;
// 	int d = 4;
// //	queue_t queue = que_create(equivalent_names);
// //	que_enqueue(queue, &a);
// //	que_dequeue(queue);
// //	que_enqueue(queue, &b);
// //	printf("Front %d\n", *( (int *) que_front(queue)));
// 
// 	
// 	// Create a tree
// 	family_node_t dad = create_f_node("Joe");
// 	dad->num_children++;
// 	family_node_t son = create_f_node("Stan");
// 	family_node_t grandson = create_f_node("peepee");
// 	family_node_t grandson_2 = create_f_node("poopoo");
// 	family_node_t grandson_3 = create_f_node("papa");
// 	
// 	son->children[0] = grandson;
// 	son->children[1] = grandson_2;
// 	son->children[2] = grandson_3;
// 	
// 	son->num_children+=3;
// 	dad->children[0] = son;
// 	printf("Joe's son: %s\n", dad->children[0]->name);
// 		 
// 	// Adding using the add_children operator
// 	add_child(dad, "Joe", "young_bm");
// 	add_child(dad, "Joe", "lololol");
// 	// Finding Stan
// 	print_family_node(son);
// 	printf("Printing TREE\n");
// 	print_tree(dad, "Joe");
// 	printf("Getting tree height for Joe: %d\n", height_r(dad));
// 	printf("Getting size for Joe: %d\n", size(dad));
// }
