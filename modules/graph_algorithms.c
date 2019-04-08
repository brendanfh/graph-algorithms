#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph_algorithms.h"
#include "graph_utils.h"

static dj_node_p dj_search(header head, int start_node)
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
			pop(stack);
		}
		else
		{
			visited_nodes = insert_dj(visited_nodes, edge_walker->to_id, 0);
			push(stack, edge_walker->to_id);
		}
	}

	return visited_nodes;
}

int breadth_first_search(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);
	int start_id = lua_tointeger(L, 2);
	int iterations = lua_tointeger(L, 3);
	lua_pop(L, 3);

	int previous_node = start_id;

	int current_iterations = 0;

	sq_node_p visited = NULL;
	sq_node_p from_visited_nodes = NULL;

	sq_node_p queue = NULL;
	sq_node_p from_queue = NULL;
	enque(queue, start_id);
	enque(from_queue, start_id);

	while(queue != NULL && current_iterations <= iterations)
	{
		int node_id = front(queue);
		int previous_node = front(from_queue);
		deque(queue);
		deque(from_queue);

		if(sq_contains(visited, node_id))
			continue;

		connect runner = find_node(head, node_id);

		edge edge_runner = runner->neighbor_list; //grab the neighbor list of our current node

		while(edge_runner != NULL)
		{
			enque(queue, edge_runner->to_id);
			enque(from_queue, runner->node_id);
			edge_runner = edge_runner->next_edge;
		}

		enque(from_visited_nodes, previous_node);
		enque(visited, node_id);

		current_iterations++;
	}

	//BUILD OUT RETURN TO LUA
	lua_newtable(L);

	int index = 1;
	while (visited != NULL)
	{
		lua_pushnumber(L, index);

		lua_newtable(L);

		lua_pushnumber(L, front(from_visited_nodes));
		lua_setfield(L, 3, "from");
		deque(from_visited_nodes);

		lua_pushnumber(L, front(visited));
		lua_setfield(L, 3, "to");
		deque(visited);

		lua_settable(L, 1);

		index++;
	}

	lua_pushboolean(L, queue == NULL);

	sq_delete(visited);
	sq_delete(from_visited_nodes);
	sq_delete(queue);
	return 2;
}

int depth_first_search(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);
	int start_id = lua_tointeger(L, 2);
	int iterations = lua_tointeger(L, 3);
	lua_pop(L, 3);

	int current_iterations = 0;

	sq_node_p visited = NULL;
	sq_node_p from_visited_nodes = NULL;
	//enque(visited, start_id);
	//enque(from_visited_nodes, start_id);

	sq_node_p stack = NULL;
	sq_node_p from_stack = NULL;
	push(stack, start_id);
	push(from_stack, start_id);

	while (stack != NULL && current_iterations <= iterations)
	{
		int node_id = top(stack);
		int previous_node = top(from_stack);
		pop(stack);
		pop(from_stack);

		if(sq_contains(visited, node_id))
			continue;

		connect runner = find_node(head, node_id);

		edge edge_runner = runner->neighbor_list; //grab the neighbor list of our current node

		while (edge_runner != NULL)
		{
			push(stack, edge_runner->to_id);
			push(from_stack, runner->node_id);
			edge_runner = edge_runner->next_edge;
		}

		enque(from_visited_nodes, previous_node);
		enque(visited, node_id);

		current_iterations++;
	}

	//BUILD OUT RETURN TO LUA
	lua_newtable(L);
	int index = 1;
	while (visited != NULL)
	{
		lua_pushnumber(L, index);

		lua_newtable(L);

		lua_pushnumber(L, front(from_visited_nodes));
		lua_setfield(L, 3, "from");
		deque(from_visited_nodes);

		lua_pushnumber(L, front(visited));
		lua_setfield(L, 3, "to");
		deque(visited);

		lua_settable(L, 1);

		index++;
	}

	lua_pushboolean(L, stack == NULL);

	sq_delete(visited);
	sq_delete(from_visited_nodes);
	sq_delete(stack);
	return 2;
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
	dj_node_p connected_nodes = dj_search(head, start_id);

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
			connect current_node = find_node(head, visited_tmp->node_id);

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

		if (is_in_list(connected_nodes, to) == 1)
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

int prims(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);
	int start_id = lua_tointeger(L, 2);
	int iterations = lua_tointeger(L, 3);
	lua_pop(L, 3);

	sq_node_p visited_nodes = NULL;
	sq_node_p from_visited_nodes = NULL;
	enque(visited_nodes, start_id);
	enque(from_visited_nodes, start_id);

	int min_edge_weight;
	int from_node_id;
	int to_node_id;

	int current_iterations = 0;
	int is_completed = 0;
	
	while (current_iterations <= iterations)
	{
		min_edge_weight = -1;
		from_node_id = -1;
		to_node_id = -1;

		edge edge_runner = NULL;
		sq_node_p node_runner = visited_nodes;
		while(node_runner != NULL)
		{
			edge_runner = find_node(head, node_runner->value)->neighbor_list;
			while(edge_runner != NULL)
			{
				if(sq_contains(visited_nodes, edge_runner->to_id) == 0 && (edge_runner->weight < min_edge_weight || min_edge_weight == -1))
				{
					min_edge_weight = edge_runner->weight;
					from_node_id = node_runner->value;
					to_node_id = edge_runner->to_id;
				}

				edge_runner = edge_runner->next_edge;
			}

			node_runner = node_runner->next;
		}

		if(min_edge_weight == -1)
		{
			is_completed = 1;
			break;
		}

		enque(visited_nodes, to_node_id);
		enque(from_visited_nodes, from_node_id);

		current_iterations++;
	}


	//RETURN STUFF
	lua_newtable(L);
	deque(visited_nodes);
	deque(from_visited_nodes);

	int index = 1;
	while (visited_nodes != NULL)
	{
		lua_pushnumber(L, index);

		lua_newtable(L);

		lua_pushnumber(L, front(from_visited_nodes));
		lua_setfield(L, 3, "from");
		deque(from_visited_nodes);

		lua_pushnumber(L, front(visited_nodes));
		lua_setfield(L, 3, "to");
		deque(visited_nodes);

		lua_settable(L, 1);

		index++;
	}

	lua_pushboolean(L, is_completed == 1);

	//MEMORY IS ALREADY FREE'D FROM DEQUEs
	//sq_delete(visited_nodes);
	//sq_delete(from_visited_nodes);
	return 2;
}


int kruskals(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);
	int iterations = lua_tointeger(L, 3);
	lua_pop(L, 2);

	sq_node_p visited_nodes = NULL;
	sq_node_p from_visited_nodes = NULL;

	int min_edge_weight;
	int from_node_id;
	int to_node_id;

	int current_iterations = 0;
	int is_completed = 0;
	
	while (current_iterations <= iterations)
	{
		min_edge_weight = -1;
		from_node_id = -1;
		to_node_id = -1;

		edge edge_runner = NULL;
		connect node_runner = head->front; 
		while(node_runner != NULL)
		{
			edge_runner = node_runner->neighbor_list;
			while(edge_runner != NULL)
			{
				if((sq_contains(visited_nodes, edge_runner->to_id) == 0 || sq_contains(visited_nodes, node_runner->node_id) == 0) && (edge_runner->weight < min_edge_weight || min_edge_weight == -1))
				{
					min_edge_weight = edge_runner->weight;
					from_node_id = node_runner->node_id;
					to_node_id = edge_runner->to_id;
				}

				edge_runner = edge_runner->next_edge;
			}

			node_runner = node_runner->next_node;
		}

		if(min_edge_weight == -1)
		{
			is_completed = 1;
			break;
		}

		enque(visited_nodes, to_node_id);
		enque(from_visited_nodes, from_node_id);		

		iterations++;
	}


	//RETURN STUFF
	lua_newtable(L);
	deque(visited_nodes);
	deque(from_visited_nodes);

	int index = 1;
	while (visited_nodes != NULL)
	{
		lua_pushnumber(L, index);

		lua_newtable(L);

		lua_pushnumber(L, front(from_visited_nodes));
		lua_setfield(L, 3, "from");
		deque(from_visited_nodes);

		lua_pushnumber(L, front(visited_nodes));
		lua_setfield(L, 3, "to");
		deque(visited_nodes);

		lua_settable(L, 1);

		index++;
	}

	lua_pushboolean(L, is_completed == 1);

	//MEMORY IS ALREADY FREE'D FROM DEQUEs
	//sq_delete(visited_nodes);
	//sq_delete(from_visited_nodes);
	return 2;
}
