require "graphs"

local Graph = {}
function Graph:new()
	local obj = {
		graph = graphs.create_graph()
	}

	local mt = { __index = Graph }
	setmetatable(obj, mt)

	return obj
end

-- Returns new node id
function Graph:addNode(x, y)
	x = x or 0
	y = y or 0

	local g = self.graph
	local nodeID = graphs.add_node(g)
	graphs.set_node_pos(g, nodeID, x, y)

	return nodeID
end

function Graph:deleteNode(nodeID)
	if nodeID == nil then return end

	assert(type(nodeID) == "number")
	local g = self.graph

	graphs.del_node(g, nodeID)
end

function Graph:addArc(fromNode, toNode, weight)
	assert(type(fromNode) == "number")
	assert(type(toNode) == "number")
	weight = weight or 1

	local g = self.graph
	local edgeID = graphs.add_directed(g, fromNode, toNode, weight)

	return edgeID
end

function Graph:addEdge(fromNode, toNode, weight)
	assert(type(fromNode) == "number")
	assert(type(toNode) == "number")
	weight = weight or 1

	local g = self.graph
	local edgeID1 = graphs.add_directed(g, fromNode, toNode, weight)
	local edgeID2 = graphs.add_directed(g, toNode, fromNode, weight)

	return edgeID1, edgeID2
end

function Graph:deleteEdge(edgeID)
	assert(type(edgeID) == "number")

	local g = self.graph
	graphs.del_edge(g, edgeID)
end

function Graph:getNodes()
	local g = self.graph
	local nodes = graphs.get_nodes(g)
	return nodes
end

function Graph:getEdges()
	local g = self.graph
	local edges = graphs.get_edges(g)
	return edges
end

function Graph:getNodePos(nodeID)
	return graphs.get_node_pos(self.graph, nodeID)
end

function Graph:setNodePos(nodeID, x, y)
	local g = self.graph
	graphs.set_node_pos(self.graph, nodeID, x, y)
end

function Graph:dijkstras(startID, iterations)
	return graphs.dijkstras(self.graph, startID, iterations)
end

return Graph