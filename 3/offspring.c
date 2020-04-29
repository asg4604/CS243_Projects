/// Author: AbuBakr Ghaznavi
/// File: offspring.c
/// Description: The main program for the offspring program
//				 Takes input from the user and and processes the tree using commands


#define _POSIX_SOURCE
#include <unistd.h>
#include "family_node.h"
#include "trimit.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
// Throw this in the header
#define MAXLEN 1024
#define CMD_CASE(command, cur_command) if (strcmp(command, cur_command) == 0) 
// Test code to get the values from the file
// Change prints to add
// Updated function will take a tree as a param

// Creating a global tree
family_node_t tree = NULL;

/// Checks if the line only has one name
/// @param line The input line being read
/// @return boolean true, if there is only one name, false if there is more
bool one_name(char *line) {
	char *new_space = malloc(sizeof(char) * (strlen(line) + 1));
	strcpy(new_space, line);
	int count = 0;
	for (char *token = strtok(new_space, ",\n"); token != NULL; token = strtok(NULL, ",\n")) {
		count++;
	}
	free(new_space);
	return count < 2;
} 


/// Reads the file in and creates a tree
/// @param file_name
/// @return The tree generated from the file
family_node_t read_file(char *file_name) {
	// The file that will be read into the tree
	FILE *fp = fopen(file_name, "r");
	// The current field being returned by strtok
	char *field = NULL;
	// The name of the parent
	char *parent_name = NULL;
	// The name of the current child
	char *cur_child_name = NULL;
	// The input line
	char line[MAXLEN];
	// Number of nodes created in the tree so far
	int num_created = 0;
	
	int fields_processed = 0;
	// Initialize the root and the node found equal to NULL
	family_node_t root = NULL;
	family_node_t found = NULL;
	
	// Process the current line until the End of File
	while (!feof(fp)) {
		// Gets the line
		fgets(line, MAXLEN, fp);
		one_name(line);
		fields_processed = 0;
		// If the line is empty, just skip the line
		if (line[0] == '\n') continue;
		
		// If we have reached the end of the file, just stop processing
		if (feof(fp)) break;
		
		if (one_name(line) && num_created > 0) {
			fprintf(stderr, "error: need more than a parent name for a non root family member.\n");
		}
		// Start seperating values by commans
		field = strtok(line, ",");
		// Set the parent name to be the first comma seperated value
		parent_name = trim(field);
		// DEBUG PRINT
//		printf("Parent: %s\n", parent_name);
		
		// Creating the first initial load
		if (num_created == 0) {
			root = create_f_node(parent_name);
			num_created++;
		} 

		// Processing children
		while(field != NULL) {
			field = strtok(NULL, ",\n");
			fields_processed++;
			if (field == NULL) break; 
			cur_child_name = trim(field);
			// Looking for the current child
			// If the this child is the root of the tree
			if (strcmp(cur_child_name, root->name) == 0) {
				// Can add without checking capacity because this node is fresh
				// Makes the current tree the child of the new root.
				family_node_t new_root = create_f_node(parent_name);
				new_root->children[0] = root;
				new_root->num_children++;
				root = new_root;
				break;
			}
			// Adding a node in the case that we aren't adding a parent to the whole tree
			found = find_node(root, parent_name);
			// If we can't find the right parent to add to
			if ( found == NULL ) {
				fprintf(stderr, "error: \'%s\' is not in the tree and \'%s\' is not the root.\n", parent_name, cur_child_name);
				// Move on to the next line of input
				break;	
			}
				
			// Makes sure that this node can be added to before attempt adding
			add_child(root, parent_name, cur_child_name);
			// DEBUG PRINT
//			printf("\tChild: %s\n", trim(field));
		}
	}
	// Free the file pointer
	fclose(fp);
	return root;
}

/// Processes the command and its arguments to make adjustments for the tree
/// @param command The command being read in
/// @param args An array of arguments containing the first and second arguments for the command
void processCommand(char *command, char *args[2]) {
	// if (tree == NULL && !(strcmp(command, "add") == 0)) {
	//	fprintf(stderr, "error: tree is empty\n");
	//	return;
	// }

	if (strcmp(command, "size") == 0) {
		family_node_t size_check_node = NULL;
		if (args[0] == NULL) {
			size_check_node = tree;
		} else {
			size_check_node = find_node(tree, args[0]);
		}
		int fam_size = size(size_check_node);
		// Modify output
		printf("size: %d\n", fam_size);
	}
	if (strcmp(command, "print") == 0) {
		if (tree == NULL) {
//			fprintf(stderr, "error: tree is empty\n");
			fprintf(stderr, "error: \'%s\' not found.\n", args[0]);	
			return;
		}
		if (args[0] != NULL) {
			family_node_t current_node = find_node(tree, args[0]);
			if (current_node == NULL) {
				fprintf(stderr, "error: \'%s\' not found.\n", args[0]);
				return;
			}
			print_tree(tree, args[0]);
		} else {
			print_tree(tree, NULL);
		}
	}
	if (strcmp(command, "height") == 0) {
		int tree_height = height(tree, args[0]);
		if (args[0] != NULL) {
			if (strlen(args[0]) < 1) {
				args[0] = NULL;
			} 
		}
		printf("height: %d\n", tree_height - 1);
	}
	// The code to add to a tree
	if (strcmp(command, "add") == 0) {
		if (args[0] == NULL) {
			// fprintf(stderr, "error: no argument for parent\n");
			fprintf(stderr, "usage: add parent_name, child_name\n");
			return;
		}
		if (args[1] == NULL) {
			// fprintf(stderr, "error: no argument for child\n");
			fprintf(stderr, "usage: add parent_name, child_name\n");
			return;
		}
		if (tree == NULL) {
			// Adding to null tree
			family_node_t new_node = create_f_node(args[0]);
			add_child(new_node, args[0], args[1]);
			tree = new_node;
			return;
		}
		// family_node_t child_found_node;
		family_node_t found_node = find_node(tree, args[0]);
		if (found_node == NULL) {
			// In the case that the child is the root node
			if (strcmp(tree->name, args[1]) == 0) {
				family_node_t new_root = create_f_node(args[0]);
				new_root->children[0] = tree;
				new_root->num_children++;
				tree = new_root;
				return;
			}
			fprintf(stderr, "error: \'%s\' is not in the tree and \'%s\' is not the root.\n", args[0], args[1]);
			return;
		}
		if (tree == NULL) {
			tree = create_f_node(args[0]);
			return;
		} else {
			// child_found_node = find_node(tree, args[1]);
			// Means that the child node already exists 
			//if (child_found_node != NULL) {
			//	fprintf(stderr, "error: \'%s\' is already a child of \'%s\'\n", args[1], args[0]);
			//	return;
			//}
			add_child(tree, args[0], args[1]);
			return;
		}
	}
	if (strcmp(command, "init") == 0) {
		if (tree == NULL) {
			fprintf(stderr, "error: tree already initialized.\n");
			return;
		}
		tree = destroy_tree(tree);
		tree = NULL;
		return;
	}
	if (strcmp(command, "quit") == 0) {
		tree = destroy_tree(tree);
		tree = NULL;
		exit(EXIT_SUCCESS);
	}
	if (strcmp(command, "find") == 0) {
				family_node_t current_node = NULL;		
				if (args[0] == NULL) {
					current_node = tree;
				} else {
					current_node = find_node(tree, args[0]);
				}
				if (current_node == NULL) {
					fprintf(stderr, "error: \'%s\' not found.\n", (args[0] == NULL) ? "" : args[0]);
				} else {
					print_family_node(current_node);
				}
	}	
	if (strcmp(command, "help") == 0) {
		printf("%-35s %s.\n", "Command", "Information");
	 	printf("\n");
		printf("%-35s %s.\n", "add parent_name, child_name", "Adds a child to a parent's list of children");
		printf("%-35s %s.\n", "find [name]", "Finds and prints the family member and their children");
		printf("%-35s %s.\n", "size [name]", "Finds size of family tree starting with [name] (root if not provided)");
		printf("%-35s %s.\n", "height [name]", "Finds height of family tree starting with [name] (root if not provided)");
		printf("%-35s %s.\n", "init", "Clears the family tree");
		printf("%-35s %s.\n", "help", "Displays this help message");
		printf("%-35s %s.\n", "quit", "Clears the family tree and exits the program");
	}
}
/// The read eval print loop that drives the program
void readAndProcess() {
	char *command = NULL;
	char *arg = NULL;
	char space[MAXLEN];
	char *args[2];
	int arg_idx = 0;
	while (!feof(stdin)) {
		args[0] = NULL;
		args[1] = NULL;
		arg_idx = 0;
		char *buffer = space;
		printf("offspring> ");
		fgets(buffer, MAXLEN, stdin);
		// If we get EOF, just exit the processing loop and print a newline
		if (feof(stdin)) { 
			printf("\n"); 
			return;
		}
		printf("+ %s\n", buffer);
		arg = strtok(buffer, " ");
		command = trim(arg);
// DEBUG		printf("COMMAND: %s\n", trim(command));
		while ( (arg = strtok(NULL, ",")) != NULL ) {
			if (arg != NULL) {
// DEBUG		printf("ARGUMENT: %s\n", trim(arg));	
				args[arg_idx++] = trim(arg);
				// Ignore any arguments to the command other than the first two.
				if (arg_idx >= 2) break;
			}
		}
		processCommand(command, args);
	}
}


/// Checks if the file exists and is readable
/// @param path The string path to the file
/// @return True if the file is accessable for reading, False if otherwise
bool check_file(char *path) {
	if (access(path, R_OK | F_OK) != -1) {
		return true;
	} else {
		return false;
	}
}

/// The main driver for the program
/// @param argc The amount of arguments
/// @param argv An array to strings containing the arguments to the program
int main(int argc, char** argv) {
	// No file passed in
	if (argc < 2) {
		// Tree defaults to NULL
		tree = NULL;
	} else {
		bool file_readable = check_file(argv[1]);
		if (!file_readable) {
			fprintf(stderr, "error: could not open file \'%s\'.\n", argv[1]);
			tree = NULL;
		} else {
			tree = read_file(argv[1]);
		}
	}
	readAndProcess();
	destroy_tree( tree );	
}
