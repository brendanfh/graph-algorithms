require "graphs"

function love.load()
	local graph = graphs.create_graph()

	local node1 = graphs.add_node(graph)
	local node2 = graphs.add_node(graph)
	local node3 = graphs.add_node(graph)

	graphs.add_directed(graph, node1, node2, 2)
	graphs.add_directed(graph, node2, node3, 4)
end

function love.update()
end

function love.draw()
end