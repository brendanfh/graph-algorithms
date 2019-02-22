#ifndef __GRAPH_TYPES_H__
#define __GRAPH_TYPES_H__

typedef struct edge_struct {
	int edge_id;
	int to_id;
	int weight;
	struct edge_struct* next_edge;
} edge_struct, *edge;

typedef struct node {
	edge neighbor_list;
	int node_id;
	int edge_count;
	struct node* next_node;

	lua_Number x, y;
} node, *connect;

typedef struct {
	connect front;
	int node_count;
	int next_edge_id;
} head_structure, *header;

#define LUA_FUNCTION(func) int (func)(lua_State* L)

#endif