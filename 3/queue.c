/// Author: AbuBakr Ghaznavi
/// File: queue.c
/// Description: Implements the operations to perform on a queue
#include <stdio.h>
#include <stdlib.h>
#include "link_node.h"
#include "queue.h"
#include <string.h>
// Method used to print an integer
// @param ptr The pointer to an integer
void printInt(void *ptr) {
	printf("%d\n", *( (int *) ptr));
}


/// Creates the queue with the equivalence function
queue_t que_create(bool (*equiv)(void *item1, void* item2)) {
	queue_t new_queue = (queue_t) malloc( sizeof(queue_s) );
	new_queue->front = NULL;
	new_queue->rear = NULL;
	new_queue->equiv = equiv;
	new_queue->size = 0;
	return new_queue;
}

/// Enqueues an element into the queue
void que_enqueue(queue_t queue, void *data) {
	// The case that the queue is empty
	if ( (queue->front == NULL && queue->rear == NULL) || que_size(queue) == 0 ) {
		// Create a new node; make head and tail this new node
		link_node_t *new_link_node = create_node(data);
		queue->front = new_link_node;
		queue->rear = new_link_node;
		queue->size += 1;
		return;
	}
	link_node_t *end_node = append_node(queue->front, data);
	queue->size += 1;
	queue->rear = end_node;	
	return;
}

// Return the data from the removed node pointer
void que_dequeue(queue_t queue) {
	if (queue->front == NULL && queue->rear == NULL) {
		// TODO print error for empty
		// write a function for empty queue			
		fprintf(stderr, "Cannot dequeue empty queue\n");
		exit(EXIT_FAILURE);
		return;
	}
	link_node_t *node_to_remove = queue->front;
	// Since dequeue is void don't need this pointer
	// void *data_from_removed = node_to_remove->data;
	queue->front = node_to_remove->next;
	free(node_to_remove);
	queue->size -= 1;
	return; 
}

/// Gets the pointer to the front element of the queue
const void *que_front(queue_t queue) {
	if (queue->rear == NULL) {
		// TODO print error for empty queue
		// Exit
	}
	return queue->front->data;
}

/// Destroys the entire queue
void que_destroy(queue_t queue) {
	link_node_t *current_node = queue->front;
	link_node_t *next_node;
	while (current_node != NULL) {
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
// P2		if (current_node == next_node) break;
	}
	queue->front = NULL;
	queue->rear = NULL;
	free(queue);
	queue = NULL;
	return;
}

/// The equivalence function used to fill the queue
/// @param x pointer to the first element
/// @param y pointer to the second element
/// @return the boolean value of whether or not they are equivalent
bool equiv(void *x, void *y) {
	char *a = (char *) x;
	char *b = (char *) y;
	return strcmp(a, b) == 0;
}

/// Returns the size of the queue
size_t que_size(queue_t queue) {
	return queue->size;
}

// int main() {
// 	int a = 2;
// 	int b = 3;
// 	int c = 4;
// 	int d = 5;
// 	queue_t queue = que_create(equiv);
// 	printf("Empty queue size: %d\n", que_size(queue));
// 	que_enqueue(queue, &a);
// 	printf("1 added queue size: %d\n", que_size(queue));
// 	que_enqueue(queue, &b);
// 	printf("2 added queue size: %d\n", que_size(queue));
// 	que_dequeue(queue);	
// 	printf("1 removed queue size: %d\n", que_size(queue));
// //	printf("Dequeue\n");
// //	dequeue(queue);
// //	printList(queue->front, printInt);
// //	printf("Dequeue\n");
// //	dequeue(queue);
// //	printList(queue->front, printInt);
// //	printf("Dequeue\n");
// //	dequeue(queue);
// //	printList(queue->front, printInt);
// //	printf("Dequeue\n");
// //	dequeue(queue);
// //	printList(queue->front, printInt);
// //	dequeue(queue);
// //	printList(queue->front, printInt);
// 	return 0;
// }
