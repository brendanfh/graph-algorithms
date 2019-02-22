#ifndef __GRAPH_UTILS_H__
#define __GRAPH_UTILS_H__

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#include "graph_types.h"

/* Returns all nodes in the form:
	{
		id = <node_id>,
		x = <x_pos>,
		y = <y_pos>
	}
*/
LUA_FUNCTION(get_nodes);

/* Returns all edges in the form:
	{
		id = <edge_id>,
		from_node = <node_id>,
		to_node = <node_id>,
		weight = number,
	}
*/
LUA_FUNCTION(get_edges);
LUA_FUNCTION(print_graph);

#endif