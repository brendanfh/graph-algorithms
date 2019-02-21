#include <stdio.h>

#include "graph_standard.h"
#include "graph_utils.h"

int luaopen_graphs(lua_State *L)
{
	printf("LOADING MODULE\n");

	const luaL_Reg functions[] = {
		{ "create_graph", create_graph },
		{ "add_node", add_node },
		{ "del_node", del_node },
		{ "add_directed", add_directed },
		{ "add_undirected", add_undirected },
		{ "del_edge", del_edge },
		{ NULL, NULL }
	};

	luaL_register(L, "graphs", functions);
	return 1;
}