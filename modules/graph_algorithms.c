#include <stdio.h>
#include <stdlib.h>

#include "graph_algorithms.h"

static ll_node_p depth_first_search(header head, int start_node)
{
	ll_node_p visited_nodes = insert_ll(NULL,start_node,0);
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
			visited_nodes = insert_ll(visited_nodes, edge_walker->to_id, 0);
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

	connect walker = head->front;

	ll_node_p has_visited = insert_ll(NULL, start_id, 0);
	ll_node_p shortest_distance = insert_ll(NULL, start_id, 0);
	ll_node_p connected_nodes = depth_first_search(head,start_id);


	while (connected_nodes != NULL)
	{
		ll_node_p visited_tmp = has_visited;
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
				if (is_in_list(has_visited, edge_walker->to_id) == 0 || distance_to_current_node + edge_walker->weight < get_distance(shortest_distance,edge_walker->to_id))
				{
					if (shortest_distance_tmp == -1 || distance_to_current_node + edge_walker->weight < shortest_distance_tmp)
					{
						shortest_distance_tmp = distance_to_current_node + edge_walker->weight;
						from = has_visited->node_id;
						to = edge_walker->to_id;
					}
				}
			}
			visited_tmp = visited_tmp->next;
		}
		remove_ll(connected_nodes, to);
		//shortest_distance
	}
	//Start at start node
	//Check shortest distance to all connected nodes from all visited nodes
	//Repeat with closest node	


	return 1;
}

