#ifndef __UTILS_H__
#define __UTILS_H__

#define SQ_EMPTY -8675309 

typedef struct sq_node {
	int value;
	struct sq_node * next;
} sq_node, *sq_node_p;

int sq_top(sq_node_p stack_top);
int sq_front(sq_node_p queue_front);

sq_node_p sq_push(sq_node_p stack_top, int val);
sq_node_p sq_pop(sq_node_p stack_top);
sq_node_p sq_enque(sq_node_p front_of_queue, int val);
sq_node_p sq_deque(sq_node_p front_of_queue);

void sq_print(sq_node_p sq);

// Helper macros that make the call site of these functions cleaner
#define push(sq, val)  ((sq) = sq_push( (sq), (val)))
#define pop(sq, val)   ((sq) = sq_pop(  (sq), (val)))
#define enque(sq, val) ((sq) = sq_enque((sq), (val)))
#define deque(sq, val) ((sq) = sq_deque((sq), (val)))

#endif