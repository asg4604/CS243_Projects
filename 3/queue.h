/// Author: AbuBakr Ghaznavi
/// File: queue.h
/// Description: Defines the data and the functions necessary for the queue

#include <stdbool.h>
#ifndef QUEUE_H
#define QUEUE_H
/// The queue data structure being used to the program
typedef struct {
	/// The front of the queue
	struct link_node *front;
	/// The back of the queue
	struct link_node *rear;
	/// The current size of the queue
	size_t size;
	/// The equivalence function
	bool (*equiv)(void *item1, void *item2);
} queue_s, *queue_t;

/// Creates the queue given an equivalence function
/// @param equiv The function being used for equivalnce
/// @return The newly created queue
queue_t que_create(bool (*equiv)(void *item1, void *item2)); 

/// Enqueues an element into the queue
/// @param queue The queue being enqueued into
/// @param data The data being enqueued into the queue
void que_enqueue(queue_t queue, void *data); 

/// Dequeues an element from the queue and frees the queue space for that element
/// @param The queue being dequeued from
void que_dequeue(queue_t queue); 

/// Gets the front of the queue
/// @param queue
/// @return Returns the pointer to the front element of the queue
const void *que_front(queue_t queue); 

/// Frees the space of the queue
/// @param queue The queue being decoded from
void que_destroy(queue_t queue);

/// Gets the size of the queue
/// @param queue
/// @return The integer size of the queue
size_t que_size(queue_t queue);

#endif
