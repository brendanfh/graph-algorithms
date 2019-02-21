#ifndef __GRAPH_TYPES_H__
#define __GRAPH_TYPES_H__

typedef struct {
	int to_id;
	int weight;
} edge_struct,*edge;

typedef struct node {
	edge neighbor_list;
	int id;
	int edge_count;
	struct node* next_node;
} node,*connect;

typedef struct {
	connect front;
	int node_count; 
} head_structure, *header;

#endif