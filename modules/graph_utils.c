#include "graph_utils.h"

int get_nodes(lua_State *L)
{
	header head = (header) lua_touserdata(L, 1);
	lua_pop(L, 1); // Take off the pointer parameter

	lua_newtable(L); // Push a new table onto the stack

	int index = 1;
	for (connect jogger = head->front; jogger != NULL; jogger = jogger->next_node) {
		lua_pushnumber(L, index); // Push the next index of the node, sets up for settable call
		lua_newtable(L); // Push on the table that will store each node

		// Add the fields to our node table

		lua_pushnumber(L, jogger->node_id);
		lua_setfield(L, 3, "id");

		lua_pushnumber(L, jogger->x);
		lua_setfield(L, 3, "x");
		lua_pushnumber(L, jogger->y);
		lua_setfield(L, 3, "y");

		// Put our node table in the table we are returning
		lua_settable(L, 1);
		
		index++;
	}

	return 1;
}

int get_edges(lua_State *L)
{
	header head = (header) lua_touserdata(L, 1);
	lua_pop(L, 1);

	lua_newtable(L);

	int index = 1;
	for (connect jogger = head->front; jogger != NULL; jogger = jogger->next_node) {
		int node_id = jogger->node_id;

		for (edge swimmer = jogger->neighbor_list; swimmer != NULL; swimmer = swimmer->next_edge) {
			lua_pushnumber(L, index);
			lua_newtable(L);

			lua_pushnumber(L, swimmer->edge_id);
			lua_setfield(L, 3, "id");

			lua_pushnumber(L, node_id);
			lua_setfield(L, 3, "from_node");

			lua_pushnumber(L, swimmer->to_id);
			lua_setfield(L, 3, "to_node");

			lua_pushnumber(L, swimmer->weight);
			lua_setfield(L, 3, "weight");

			lua_settable(L, 1);

			index++;
		}
	}

	return 1;
}

int print_graph(lua_State *L)
{
	header head = (header)lua_touserdata(L,1);

	connect node_walker = head->front;
	edge edge_walker;

	while (node_walker != NULL) 
	{
		printf("%d: ", node_walker->node_id);
		edge_walker = node_walker->neighbor_list;
		while (edge_walker != NULL)
		{
			printf("(%d,%d), ", edge_walker->to_id, edge_walker->weight);
			edge_walker = edge_walker->next_edge;
		}
		printf("\n");
		node_walker = node_walker->next_node;
	}

	return 0;
}