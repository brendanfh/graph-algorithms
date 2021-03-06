#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "graph_utils.h"
#include "graph_standard.h"

static void add_edge(header head, int from_node, int to_node, int edge_weight);

// graph pointer create_graph()
int create_graph(lua_State *L)
{
	header new_graph = (header)malloc(sizeof(head_structure));

	new_graph->front = NULL;
	new_graph->node_count = 0;
	new_graph->next_edge_id = 0;

	lua_pushlightuserdata(L, (void*)new_graph);

	return 1;
}

// node number (int) add_node(ptr)
int add_node(lua_State *L)
{
	header head = (header)lua_touserdata(L,1);
	connect new_node = (connect)malloc(sizeof(node));
	new_node->next_node = NULL;
	new_node->neighbor_list = NULL;
	new_node->x = 0;
	new_node->y = 0;

	if (head == NULL || head->node_count == 0 || head->front == NULL)
	{
		new_node->node_id = 0;
		head->front = new_node;
	}
	else
	{
		connect walker = head->front;
		while (walker->next_node != NULL && walker->node_id == walker->next_node->node_id - 1) {
			walker = walker->next_node;
		}
		if (walker->next_node == NULL)
		{
			new_node->node_id = head->node_count;
			walker->next_node = new_node;
		}
		else
		{
			new_node->node_id = walker->node_id + 1;
			new_node->next_node = walker->next_node;
			walker->next_node = new_node;
		}
	}

	head->node_count++;

	lua_pushnumber(L,new_node->node_id);

	return 1;
}

// void del_node(graph pointer, int nodeID)
int del_node(lua_State *L)
{
	header head = (header)lua_touserdata(L,1);
	int nid = lua_tointeger(L,2);

	//find the node
	connect walker = head->front;
	connect follower = walker;
	while(walker->node_id != nid && walker != NULL){
		if(walker->node_id > nid)
		{
			puts("NODE NOT FOUND FOR DELETE");
			return 0;
		}
		follower = walker;
		walker = walker->next_node;
	}
	if (walker == NULL)
	{
		puts("NODE NOT FOUND FOR DELETE");
		return 0;
	}


	//delete all edges that correspond to the node
	edge edge_walker = walker->neighbor_list;
	edge delete_walker = edge_walker;
	while (edge_walker != NULL)
	{
		edge_walker = edge_walker->next_edge;
		free(delete_walker);
		delete_walker = edge_walker;
	}
	connect node_walker = head->front;
	while (node_walker != NULL)
	{
		edge_walker = node_walker->neighbor_list;
		delete_walker = edge_walker;
		while (edge_walker != NULL)
		{
			if (edge_walker->to_id == nid)
			{
				if (edge_walker == node_walker->neighbor_list)
				{
					node_walker->neighbor_list = node_walker->neighbor_list->next_edge;
					free(edge_walker);
					edge_walker = node_walker->neighbor_list;
					delete_walker = edge_walker;
				}
				else
				{
					delete_walker->next_edge = edge_walker->next_edge;
					delete_walker = delete_walker->next_edge;
					edge_walker = edge_walker->next_edge;
					free(delete_walker);				
				}
			}
			else
			{
				delete_walker = edge_walker;
				edge_walker = edge_walker->next_edge;
			}
		}
		node_walker = node_walker->next_node;
	}

	//delete the node
	if (walker == head->front)
	{
		head->front = head->front->next_node;
	}
	else
	{
		follower->next_node = walker->next_node;
	}
	head->node_count--;
	free(walker);
	return 0;
}

static void add_edge(header head, int from_node, int to_node, int edge_weight)
{
	//walk through node list to find from node
	connect runner = find_node(head, from_node);

	if(runner == NULL)
		return;

	//setup edge search
	edge edge_run = runner->neighbor_list;

	//setup new edge
	edge new_edge = (edge)malloc(sizeof(edge_struct));
	new_edge->edge_id = head->next_edge_id++;
	new_edge->to_id = to_node;
	new_edge->weight = edge_weight;
	new_edge->next_edge = NULL;


	//place the new edge
	if(edge_run == NULL)
		runner->neighbor_list = new_edge; //this is first edge for node
	else
	{
		while(edge_run->next_edge != NULL) //go to end of neighbor list
		{
			edge_run = edge_run->next_edge;
		}

		edge_run->next_edge = new_edge;

	}
}

// add_directed(graph pointer, int from, int to, int weight)
int add_directed(lua_State *L)
{
	//grab variables from lua_State
	header head = (header)lua_touserdata(L,1);
	
	int from_node = lua_tointeger(L, 2);
	int to_node = lua_tointeger(L, 3);
	int edge_weight = lua_tointeger(L, 4);

	//add single edge with add_edge function
	add_edge(head, from_node, to_node, edge_weight);

	return 0;
}

// add_undirected(graph pointer, int from, int to, int weight)
int add_undirected(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);

	int from_node = lua_tointeger(L, 2);
	int to_node = lua_tointeger(L, 3);
	int edge_weight = lua_tointeger (L, 4);

	//add two edges (from -> to and to -> from)
	add_edge(head, from_node, to_node, edge_weight);
	add_edge(head, to_node, from_node, edge_weight);

	return 0;
}

// del_edge(graph pointer, edge_id)
int del_edge(lua_State *L)
{
	header head = (header)lua_touserdata(L, 1);
	int edge_id = lua_tointeger(L, 2);

	//we will have to walk through each node and walk through each edge of each node
	connect runner = head->front;
	while (runner != NULL)
	{
		//now we will have to go through every edge within the node
		edge curr_edge = runner->neighbor_list;

		if (curr_edge == NULL) {
			//no neighbors
		}
		else if (curr_edge->edge_id == edge_id) //first edge is one to remove
		{
			runner->neighbor_list = curr_edge->next_edge;
			free(curr_edge);
			return 0;
		}
		else //search through each edge
		{
			edge follower = curr_edge;
			curr_edge = curr_edge->next_edge;
			while (curr_edge != NULL)
			{
				if (curr_edge->edge_id == edge_id)
				{
					follower->next_edge = curr_edge->next_edge;
					free(curr_edge);
					return 0;
				}

				follower = curr_edge;
				curr_edge = curr_edge->next_edge;
			}
		}

		runner = runner->next_node;
	}

	return 0;
}

