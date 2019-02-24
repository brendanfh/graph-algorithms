#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph_algorithms.h"

static dj_node_p depth_first_search(header head, int start_node)
{
	dj_node_p visited_nodes = insert_dj(NULL,start_node,0);
	sq_node_p stack = sq_push(NULL, start_node);
	while (stack != NULL)
	{
		int node_id = top(stack);
		connect walker = head->front;
		while (walker != NULL && walker->node_id != node_id)
		{
			walker = walker->next_node;
		}
		if (walker == NULL)
		{
			return NULL;
		}

		edge edge_walker = walker->neighbor_list;
		while (edge_walker != NULL && is_in_list(visited_nodes, edge_walker->to_id) == 1) 
		{
			edge_walker = edge_walker->next_edge;
		}
		if (edge_walker == NULL)
		{
			stack = sq_pop(stack);
		}
		else
		{
			visited_nodes = insert_dj(visited_nodes, edge_walker->to_id, 0);
			stack = sq_push(stack, edge_walker->to_id);
		}
	}
	return visited_nodes;
}

int dijkstras(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);
	int start_id = lua_tointeger(L, 2);
	int iterations = lua_tointeger(L, 3);
	lua_pop(L, 3);

	int current_iterations = 0;

	connect walker = head->front;

	dj_node_p has_visited = insert_dj(NULL, start_id, 0);
	dj_node_p shortest_distance = insert_dj(NULL, start_id, 0);
	dj_node_p connected_nodes = depth_first_search(head, start_id);

	connected_nodes = remove_dj(connected_nodes, start_id);
	dj_node_p distance_walker = connected_nodes;
	while (distance_walker != NULL)
	{
		insert_dj(shortest_distance, distance_walker->node_id, -1);
		distance_walker = distance_walker->next;
	}
	int last_from = -1;
	int last_to = -1;
	int last_weight;

	while (connected_nodes != NULL && current_iterations <= iterations)
	{
		dj_node_p visited_tmp = has_visited;
		int shortest_distance_tmp = -1;
		int from = -1;
		int to = -1;
		while (visited_tmp != NULL)
		{
			connect current_node = head->front;
			while (current_node->node_id != visited_tmp->node_id)
			{
				current_node = current_node->next_node;
			}

			edge edge_walker = current_node->neighbor_list;
			while (edge_walker != NULL)
			{
				int distance_to_current_node = get_distance(shortest_distance, current_node->node_id);
				if (is_in_list(has_visited, edge_walker->to_id) == 0 || distance_to_current_node + edge_walker->weight < get_distance(shortest_distance, edge_walker->to_id))
				{
					if (shortest_distance_tmp == -1 || distance_to_current_node + edge_walker->weight < shortest_distance_tmp)
					{
						shortest_distance_tmp = distance_to_current_node + edge_walker->weight;
						from = current_node->node_id;
						to = edge_walker->to_id;
						last_weight = edge_walker->weight;
					}
				}
				edge_walker = edge_walker->next_edge;
			}
			visited_tmp = visited_tmp->next;
		}
		last_to = to;
		last_from = from;

		if (is_in_list(connected_nodes,to) == 1)
		{
			connected_nodes = remove_dj(connected_nodes, to);
		}


		dj_node_p sdwalker = shortest_distance;
		while (sdwalker->node_id != to)
		{
			sdwalker = sdwalker->next;
		}

		sdwalker->distance = shortest_distance_tmp;

		if (is_in_list(has_visited,to) == 0)
		{
			insert_dj(has_visited, to, 0);
		}
		current_iterations++;
	}

	delete_dj(has_visited);
	delete_dj(connected_nodes);

	lua_newtable(L);
	dj_node_p sdwalker = shortest_distance;
	int index = 1;
	while (sdwalker != NULL)
	{
		lua_pushnumber(L, index);
		lua_newtable(L);

		lua_pushnumber(L, sdwalker->node_id);
		lua_setfield(L, 3, "id");

		lua_pushnumber(L, sdwalker->distance);
		lua_setfield(L, 3, "distance");

		lua_settable(L, 1);

		sdwalker = sdwalker->next;
		index++;
	}

	lua_pushnumber(L, last_from);
	lua_setfield(L, 1, "from");

	lua_pushnumber(L, last_to);
	lua_setfield(L, 1, "to");

	lua_pushnumber(L, last_weight);
	lua_setfield(L, 1, "weight");

	lua_pushboolean(L, connected_nodes == NULL);

	//put shorest_distance linked list into table and return table
	delete_dj(shortest_distance);
	return 2;
}

