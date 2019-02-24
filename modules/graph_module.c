#include <stdio.h>

#include "graph_standard.h"
#include "graph_utils.h"
#include "graph_algorithms.h"

int luaopen_graphs(lua_State *L)
{
	printf("LOADING MODULE...\n");

	const luaL_Reg functions[] = {
		{ "create_graph", create_graph },
		{ "add_node", add_node },
		{ "del_node", del_node },
		{ "add_directed", add_directed },
		{ "add_undirected", add_undirected },
		{ "del_edge", del_edge },

		{ "get_nodes", get_nodes },
		{ "get_edges", get_edges },
		{ "get_edge_by", get_edge_by },
		{ "get_edge", get_edge_id },
		{ "set_edge_weight", set_edge_weight },
		{ "get_node_pos", get_node_pos },
		{ "set_node_pos", set_node_pos },

		{ "print_graph", print_graph },
		{ "dijkstras", dijkstras },
		{ "depth_first_search", depth_first_search },
		{ "breadth_first_search", breadth_first_search },
		{ "prims", prims },
		
		{ NULL, NULL }
	};

	luaL_register(L, "graphs", functions);
	return 1;
}
