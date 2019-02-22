require "graphs"

local graph = graphs.create_graph()
graphs.add_node(graph)
graphs.add_node(graph)
graphs.add_node(graph)
graphs.add_undirected(graph,0,1,10)
local edges = graphs.get_edges(graph)
for k, v in ipairs(edges) do
	print(k, v.id, v.from_node, v.to_node, v.weight)
end

graphs.del_node(graph,1)
graphs.add_node(graph)
--graphs.del_edge(graph,0)
--graphs.print_graph(graph)

local nodes = graphs.get_nodes(graph)
for k, v in ipairs(nodes) do
	print(k, v.id, v.x, v.y)
end