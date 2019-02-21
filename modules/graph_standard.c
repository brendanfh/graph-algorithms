#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#include "graph_types.h"

int create_graph(lua_State *L)
{
	header newGraph = (header)malloc(sizeof(head_structure));

	newGraph->front = NULL;
	newGraph->node_count = 0;

	lua_pushlightuserdata(L, (void*)newGraph);

	return 1;
}

// node number (int) add_node(ptr)
int add_node(lua_State *L)
{
	header head = (header)lua_touserdata(L,1);
	connect newNode = (connect)malloc(sizeof(node));
	newNode->next_node = NULL;

	if (head->node_count == 0)
	{
		newNode->id = 0;
		head->front = newNode;
	}
	else
	{
		connect walker = head->front;
		while (walker->next_node != NULL && walker->id == walker->next_node->id - 1) {
			walker = walker->next_node;
		}
		if (walker->next_node == NULL)
		{
			newNode->id = head->node_count;
			walker->next_node = newNode;
		}
		else
		{
			newNode->id = walker->id + 1;
			newNode->next_node = walker->next_node;
			walker->next_node = newNode;
		}
	}
	head->node_count++;

	lua_pushnumber(L,newNode->id);

	return 1;
}

int del_node(lua_State *L)
{
	header head = (header)lua_touserdata(L,1);
	int nodeID = lua_tointeger(L,2);

	connect walker = head->front;
	connect follower = walker;
	while(walker->id != nodeID){

	}


	return 0;
}


int add_directed(lua_State *L)
{
	header head = (header)lua_touserdata(L,1);
	int from_node = lua_tointeger(L, 2);
	int to_node = lua_tointeger(L, 3);

	connect runner = head->front;

	if(runner == NULL)
	{
		puts("WTF bro you can't do this");
		return 0; //graph is empty
	}

	while(runner->id != from_node)
	{
		if(runner == NULL)
			return 0; //from id is never found

		runner = runner->next_node;
	}

	edge edge_run = runner->neighbor_list;
	edge new_edge;

	if(edge_run == NULL)
	{
		edge_run = (edge)malloc(sizeof(edge_struct));
		edge_run->to_id = to_node;
		
	}


	return 1;
}

int luaopen_graphs(lua_State *L)
{
	printf("LOADING MODULE\n");

	const luaL_Reg functions[] = {
		{ "create_graph", create_graph },
		{ "add_node", add_node },
		{ "del_node", del_node },
		{ "add_directed", add_directed },
		{ NULL, NULL }
	};

	luaL_register(L, "graphs", functions);
	return 1;
}