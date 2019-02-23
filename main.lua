require "lua.utils"
local Graph = require "lua.graph"
local Regions = require "lua.regions"

local graph = nil
local regions = nil

function love.load()
	graph = Graph:new()

	graph:addNode(100, 100)
	graph:addNode(300, 100)
	graph:addNode(200, 200)
	graph:addNode(500, 400)
	graph:addNode(0, 0)

	graph:addArc(0, 1)
	graph:addEdge(0, 2)
	graph:addArc(1, 3)
	graph:addEdge(1, 2)

	--graph:dijkstras(0, 0)

	love.graphics.setBackgroundColor(0.5, 0.5, 0.5)

	regions = Regions:new()
	regions:add {
		priority = 1;
		rect = { 200, 0, 600, 600 };

		selectedNode = nil;

		update = function(self) end;
		draw = function(self)
			love.graphics.setColor(0.7, 0.8, 0.9)
			love.graphics.rectangle("fill", 0, 0, 600, 600)
			drawGraph(graph)
		end;

		mousedown = function(self, x, y)
			local nodes = graph:getNodes()

			for _, node in pairs(nodes) do
				local nx = node.x
				local ny = node.y
				local d = (x - nx) * (x - nx) + (y - ny) * (y - ny)

				if d <= 400 then
					self.selectedNode = node.id
				end
			end
		end;
		mouseup = function(self, x, y)
			self.selectedNode = nil
		end;

		mousemove = function(self, x, y, dx, dy)
			if self.selectedNode ~= nil then
				graph:setNodePos(self.selectedNode, x, y)
			end
		end;
		mouseenter = function(self)
		end;
		mouseleave = function(self)
			self.selectedNode = nil
		end;
	}

	regions:add {
		priority = 0;
		rect = { 0, 0, 0, 0 };

		update = function(self) end;
		draw = function(self) end;
		mousedown = function(self, x, y) end;
		mouseup = function(self, x, y) end;
		mousemove = function(self, x, y, dx, dy) end;
		mouseenter = function(self) end;
		mouseleave = function(self) end;
	}
end

function love.mousepressed(x, y, button, isTouch, presses)
	regions:mousedown(x, y)
end

function love.mousereleased(x, y, button, isTouch, presses)
	regions:mouseup(x, y)
end

function love.mousemoved(x, y, dx, dy)
	regions:mousemove(x, y, dx, dy)
end

function love.update()
	regions:update()
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
	regions:draw()
end
