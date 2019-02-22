#include "graph_utils.h"

int get_nodes(lua_State *L)
{
	header head = (header) lua_touserdata(L, 1);
	lua_pop(L, 1); // Take off the pointer parameter

	lua_newtable(L); // Push a new table onto the stack

	for (connect jogger = head->front; jogger != NULL; jogger = jogger->next_node) {
		lua_pushnumber(L, jogger->node_id);
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

			int is_directed = (find_edge(head, swimmer->to_id, node_id, swimmer->weight) == NULL);
			lua_pushboolean(L, is_directed);
			lua_setfield(L, 3, "directed");

			lua_settable(L, 1);

			index++;
		}
	}

	return 1;
}

// number, number get_node_pos(graph pointer, int node_id)
int get_node_pos(lua_State *L)
{
	header head = (header) lua_touserdata(L, 1);
	int node_id = lua_tointeger(L, 2);
	lua_pop(L, 2);

	connect node = find_node(head, node_id);

	lua_pushnumber(L, node->x);
	lua_pushnumber(L, node->y);
	return 2;
}

// void set_node_pos(graph pointer, int node_id, number x, y)
int set_node_pos(lua_State *L)
{
	header head = (header) lua_touserdata(L, 1);
	int node_id = lua_tointeger(L, 2);
	lua_Number x = lua_tonumber(L, 3);
	lua_Number y = lua_tonumber(L, 4);
	lua_pop(L, 4);

	connect node = find_node(head, node_id);

	node->x = x;
	node->y = y;

	return 0;
}

int print_graph(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);

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

connect find_node(header head, int node_id)
{
	//walk through node list to find from node
	connect runner = head->front;
	if(runner == NULL)
	{
		return NULL; //graph is empty
	}

	//run through list of nodes until node with passed node_id is found
	while(runner->node_id != node_id)
	{
		if(runner == NULL)
			return NULL; //from id is never found

		runner = runner->next_node; //move to next node in list
	}

	return runner;
} 

edge find_edge(header head, int from_node, int to_node, int weight)
{
	connect node = find_node(head, from_node);

	if (node == NULL)
	{
		return NULL; // No edge could be found
	}

	edge jogger = node->neighbor_list;
	while (jogger != NULL)
	{
		if (jogger->to_id == to_node && (weight == ANY_WEIGHT || weight == jogger->weight))
		{
			break;
		}

		jogger = jogger->next_edge;
	}

	return jogger;
}