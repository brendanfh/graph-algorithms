#ifndef __GRAPH_STANDARD_H__
#define __GRAPH_STANDARD_H__

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#include "graph_types.h"

LUA_FUNCTION(create_graph);
LUA_FUNCTION(add_node);
LUA_FUNCTION(del_node);
LUA_FUNCTION(add_directed);
LUA_FUNCTION(add_undirected);
LUA_FUNCTION(del_edge);

#endif