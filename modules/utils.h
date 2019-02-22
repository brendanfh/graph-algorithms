#ifndef __UTILS_H__
#define __UTILS_H__

#define SQ_EMPTY -8675309 

typedef struct sq_node {
	int value;
	struct sq_node * next;
} sq_node, *sq_node_p;


#endif