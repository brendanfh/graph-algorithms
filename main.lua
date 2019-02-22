require "lua.utils"
local Graph = require "lua.graph"

local graph = nil

function love.load()
	graph = Graph:new()

	graph:addNode(100, 100)
	graph:addNode(300, 100)
	graph:addNode(200, 200)
	graph:addNode(500, 400)

	graph:addArc(0, 1)
	graph:addEdge(0, 2)
	graph:addArc(1, 3)
	graph:addEdge(1, 2)

	love.graphics.setBackgroundColor(0.5, 0.5, 0.5)
end

function love.update()
end

function drawGraph(graph)
	local nodes = graph:getNodes()
	local edges = graph:getEdges()

	love.graphics.setLineWidth(4)
	for _, edge in pairs(edges) do
		love.graphics.setColor(0, 0, 0)
		local x1 = nodes[edge.from_node].x
		local y1 = nodes[edge.from_node].y
		local x2 = nodes[edge.to_node].x
		local y2 = nodes[edge.to_node].y

		love.graphics.line(x1, y1, x2, y2)

		if edge.directed then
			local cx = math.lerp(x1, x2, 0.5)
			local cy = math.lerp(y1, y2, 0.5)

			local alpha = math.atan2(x2 - x1, y2 - y1)

			local arrowSize = 15
			local triangle = {
				cx + arrowSize * math.sin(alpha + 0 * math.pi / 3), cy + arrowSize * math.cos(alpha + 0 * math.pi / 3),
				cx + arrowSize * math.sin(alpha + 2 * math.pi / 3), cy + arrowSize * math.cos(alpha + 2 * math.pi / 3),
				cx + arrowSize * math.sin(alpha + 4 * math.pi / 3), cy + arrowSize * math.cos(alpha + 4 * math.pi / 3)
			}

			love.graphics.polygon("fill", triangle)
		end
	end

	for _, node in pairs(nodes) do
		love.graphics.setColor(0, 0, 0)
		love.graphics.circle("fill", node.x, node.y, 20)
		love.graphics.setColor(255, 255, 255)
		love.graphics.circle("fill", node.x, node.y, 17)
	end
end

function love.draw()
	drawGraph(graph)
end
