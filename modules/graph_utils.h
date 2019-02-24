#ifndef __GRAPH_UTILS_H__
#define __GRAPH_UTILS_H__

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#include "graph_types.h"

#define ANY_WEIGHT -56489

/* Returns all nodes in the form:
	{
		id = <node_id>,
		x = <x_pos>,
		y = <y_pos>
	}
	indexed by node_id
*/
LUA_FUNCTION(get_nodes);

/* Returns all edges in the form:
	{
		id = <edge_id>,
		from_node = <node_id>,
		to_node = <node_id>,
		weight = number,
		directed = boolean,
	}
*/
LUA_FUNCTION(get_edges);
LUA_FUNCTION(get_edge_by);
LUA_FUNCTION(get_edge_id);
LUA_FUNCTION(set_edge_weight);

LUA_FUNCTION(get_node_pos);
LUA_FUNCTION(set_node_pos);

LUA_FUNCTION(print_graph);

connect find_node(header head, int node_id);
edge    find_edge(header head, int from_node, int to_node, int weight);
edge 	get_edge(header head, int edge_id, connect* node);

#endif