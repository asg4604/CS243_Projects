/// Author: AbuBakr Ghaznavi
/// File: family_node.h
/// Description: Defines the data and functions necessary to perform 
///              Operations on family_nodes
#include <stdbool.h>
#ifndef FAMILY_NODE_H
#define FAMILY_NODE_H
/// Defines the structure for a member of the family_node
typedef struct family_node {
	/// The name of the family_node
	char *name; // Use strdup to get the value of the name
	/// The amount of children in the family_node
	int num_children;
	/// Pointer to an array of children
	struct family_node **children; // Can realloc if need more
	/// The amount of children that can be held at the moment
	int capacity; // Used for reallocating memory
} family_node_s;

typedef family_node_s *family_node_t;

/// Allocates memory for a new string  
/// @param s The string that will be copied
/// @return a pointer to the newly allocated string
char* strdup (const char* s);

/// Creates a family member node
/// @param name The name of the new member
/// @return The newly allocated node
family_node_t create_f_node(char *name);

/// Tests if two strings are equal
/// @param a first string
/// @param b second string
bool streq(char *a, char *b);

/// Tests if two names are equivalent given void pointers
/// @param name 1 Pointer to first name
/// @param name 2 Pointer to second name
bool equivalent_names(void *name1, void *name2);

/// Finds a node within the tree
/// @param tree The current tree
/// @param name The name of the member being searched for
/// @return The found member
family_node_t find_node(family_node_t tree, char *name);

/// Adds a child into the tree
/// @param parent_name The name that's being of the parent being added to 
/// @param child_name The name of the child being added to the parent
/// @return The newly adjusted tree
family_node_t add_child(family_node_t tree, char *parent_name, char *child_name);

/// Deallocates all memory from the tree
/// @param tree The tree to be destroyed
/// @return Pointer to the newly destroyed tree (should be NULL)
family_node_t destroy_tree( family_node_t tree );

/// Prints a singular family member
/// @param family_node the member to be printed
void print_family_node( family_node_t family_node );

/// Prints the entire tree given a tree node
/// @param tree The tree to be printed
/// @param name The name of the member to start from
void print_tree(family_node_t tree, char *name);

/// Recursive helper function for the height
/// @param tree Return the height of the tree
/// @return the height of the deepest current tree
int height_r(family_node_t tree);

/// Returns the height of the tree starting from name
/// @param tree The tree to be printed from
/// @param name The name to start from 
int height(family_node_t tree, char *name);

/// Returns the size of the tree
/// @param tree The tree for which we find the size
/// @return the integer size of the tree
int size(family_node_t tree);

/// Tests if the child is already a child of parent
/// @param The tree to check
/// @param parent_name the name of the parent to check
/// @param child_name the name of the child
bool check_child(family_node_t tree, char *parent_name, char *child_name);
#endif
