require "lua.utils"
local Graph = require "lua.graph"
local Regions = require "lua.regions"

local graph = nil
local regions = nil
local graph_region = nil

local GRAPH_FONT, SIDEBAR_FONT

function love.load()
	GRAPH_FONT = love.graphics.setNewFont(16)
	SIDEBAR_FONT = love.graphics.setNewFont(24)

	graph = Graph:new()

	graph:addNode(100, 100)
	graph:addNode(300, 100)
	graph:addNode(200, 200)
	graph:addNode(500, 400)
	graph:addNode(300, 300)

	graph:addArc(0, 1)
	graph:addEdge(0, 2)
	graph:addArc(1, 3, 99)
	graph:addEdge(1, 2)

	regions = Regions:new()
	graph_region = {
		-- needed for Regions calculations
		priority = 1;
		rect = { 200, 0, 600, 600 };

		-- other properties
		selectedNode = nil;
		postDrawFunction = nil;
		clickNodeFunction = nil;
		isMouseDown = false;

		update = function(self) end;
		draw = function(self)
			love.graphics.setColor(0.7, 0.8, 0.9)
			love.graphics.rectangle("fill", 0, 0, 600, 600)
			drawGraph(graph, self.selectedNode)

			if self.postDrawFunction then
				self.postDrawFunction(self)
			end
		end;

		mousedown = function(self, x, y)
			self.isMouseDown = true
			self.selectedNode = nil
			local nodes = graph:getNodes()

			for _, node in pairs(nodes) do
				local nx = node.x
				local ny = node.y
				local d = (x - nx) * (x - nx) + (y - ny) * (y - ny)

				if d <= 20 * 20 then
					if self.clickNodeFunction == nil then
						self.selectedNode = node.id
					else
						self.clickNodeFunction(node)
					end
				end
			end
		end;

		mouseup = function(self, x, y)
			self.isMouseDown = false
		end;

		mousemove = function(self, x, y, dx, dy)
			if self.selectedNode ~= nil and self.isMouseDown then
				graph:setNodePos(self.selectedNode, x, y)
			end
		end;

		mouseenter = function(self) end;

		mouseleave = function(self)
			self.isMouseDown = false
		end;
	}

	regions:add(graph_region)

	regions:add {
		priority = 1;
		rect = { 0, 0, 200, 600 };

		buttons = {
			{
				text = "Add node",
				click = function()
					graph:addNode(300, 300)
				end;
			};
			{
				text = "Delete node",
				click = function()
					graph:deleteNode(graph_region.selectedNode)
				end;
			};
			{
				text = "Add arc",
				click = function()
					local curr = graph_region.selectedNode
					if curr == nil then return end

					graph_region.clickNodeFunction = function(node)
						graph:addArc(curr, node.id, 1)
						graph_region.clickNodeFunction = nil
					end
				end;
			};
			{
				text = "Add edge",
				click = function()
					local curr = graph_region.selectedNode
					if curr == nil then return end

					graph_region.clickNodeFunction = function(node)
						graph:addEdge(curr, node.id, 1)
						graph_region.clickNodeFunction = nil
					end
				end;
			};
			{
				text = "Run Dijkstras",
				click = function()
					if graph_region.selectedNode ~= nil then
						createDijskstraStepper(graph_region.selectedNode)
					end
				end;
			};
		};
		mousePos = -1;

		update = function(self) end;

		draw = function(self)
			love.graphics.setFont(SIDEBAR_FONT)
			for i, button in ipairs(self.buttons) do
				if self.mousePos >= (i - 1) * 60 + 10 and self.mousePos <= i * 60 + 10 then
					love.graphics.setColor(.4, .4, .4)
				else
					love.graphics.setColor(.2, .2, .2)
				end
				love.graphics.rectangle("fill", 10, (i - 1) * 60 + 10, 180, 40)

				love.graphics.setColor(1, 1, 1)
				love.graphics.printf(button.text, 0, (i - 1) * 60 + 15, 200, "center")	
			end
		end;

		mousedown = function(self, x, y)
			for i, button in ipairs(self.buttons) do
				if y >= (i - 1) * 60 + 10 and y <= i * 60 + 10 then
					button.click()
				end
			end
		end;
		mouseup = function(self, x, y) end;
		mousemove = function(self, x, y, dx, dy)
			self.mousePos = y
		end;
		mouseenter = function(self) end;
		mouseleave = function(self)
			self.mousePos = -1
		end;
	}

	-- regions:add {
	-- 	priority = 0;
	-- 	rect = { 0, 0, 0, 0 };

	-- 	update = function(self) end;
	-- 	draw = function(self) end;
	-- 	mousedown = function(self, x, y) end;
	-- 	mouseup = function(self, x, y) end;
	-- 	mousemove = function(self, x, y, dx, dy) end;
	-- 	mouseenter = function(self) end;
	-- 	mouseleave = function(self) end;
	-- }
end

local dijkstrasOpen = false
function createDijskstraStepper(start)
	if dijkstrasOpen then return end

	dijkstrasOpen = true
	local step = -1

	local isDone = false
	function updateDijkstras()
		local dj, done = graph:dijkstras(start, step)
		isDone = done
		graph_region.postDrawFunction = function(reg)
			drawDijkstras(graph, dj)
		end
	end
	updateDijkstras()

	local reg_id
	local region = {
		priority = 10;
		rect = { 200, 550, 600, 50 };

		update = function(self) end;
		draw = function(self)
			love.graphics.setColor(0, 0, 0, 0.7)
			love.graphics.rectangle("fill", 0, 0, 600, 50)

			love.graphics.setColor(1, 1, 1)
			love.graphics.polygon("fill", 265, 25, 295, 10, 295, 40)
			love.graphics.polygon("fill", 335, 25, 305, 10, 305, 40)
		end;
		mousedown = function(self, x, y)
			if x >= 265 and x <= 295 then
				step = step - 1
				isDone = false
				if step < -1 then step = -1 end
			elseif x >= 305 and x <= 335 then
				if not isDone then
					step = step + 1
				end
			end

			updateDijkstras()
			if x >= 550 then
				regions:remove(reg_id)
				graph_region.postDrawFunction = nil
				dijkstrasOpen = false
			end
		end;
		mouseup = function(self, x, y) end;
		mousemove = function(self, x, y, dx, dy) end;
		mouseenter = function(self) end;
		mouseleave = function(self) end;
	}

	reg_id = regions:add(region)
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

-- DRAWING FUNCTIONS

function drawEdge(x1, y1, x2, y2, directed, weight, color)
	love.graphics.setColor(color)

	love.graphics.line(x1, y1, x2, y2)

	local cx = math.lerp(x1, x2, 0.5)
	local cy = math.lerp(y1, y2, 0.5)
	local arrowSize = 22

	if directed == 2 then
		local alpha = math.atan2(x2 - x1, y2 - y1)

		local triangle = {
			cx + arrowSize * math.sin(alpha + 0 * math.pi / 3), cy + arrowSize * math.cos(alpha + 0 * math.pi / 3),
			cx + arrowSize * math.sin(alpha + 2 * math.pi / 3), cy + arrowSize * math.cos(alpha + 2 * math.pi / 3),
			cx + arrowSize * math.sin(alpha + 4 * math.pi / 3), cy + arrowSize * math.cos(alpha + 4 * math.pi / 3)
		}

		love.graphics.polygon("fill", triangle)
	elseif directed == 1 then
		love.graphics.rectangle("fill", cx - arrowSize * .6, cy - arrowSize * .6, arrowSize * 1.2, arrowSize * 1.2)
	end

	love.graphics.setColor(1, 1, 1)
	love.graphics.printf(tostring(weight), cx - 20, cy - 10, 40, "center")
end

local NODE_INNER_RADIUS = 17
local NODE_OUTER_RADIUS = 20
function drawGraph(graph, selectedNode)
	local nodes = graph:getNodes()
	local edges = graph:getEdges()

	love.graphics.setFont(GRAPH_FONT)
	love.graphics.setLineWidth(4)
	for _, edge in pairs(edges) do
		local x1 = nodes[edge.from_node].x
		local y1 = nodes[edge.from_node].y
		local x2 = nodes[edge.to_node].x
		local y2 = nodes[edge.to_node].y

		drawEdge(x1, y1, x2, y2, (edge.directed and 2 or 1), edge.weight, {0, 0, 0})
	end

	for _, node in pairs(nodes) do
		if node.id == selectedNode then
			love.graphics.setColor(0, 0.9, 0)
		else
			love.graphics.setColor(0, 0, 0)
		end
		love.graphics.circle("fill", node.x, node.y, NODE_OUTER_RADIUS)
		love.graphics.setColor(255, 255, 255)
		love.graphics.circle("fill", node.x, node.y, NODE_INNER_RADIUS)
	end
end

function drawDijkstras(graph, dijkstras)
	love.graphics.setFont(GRAPH_FONT)

	if dijkstras.from >= 0 and dijkstras.to >= 0 then
		local x1, y1 = graph:getNodePos(dijkstras.from)
		local x2, y2 = graph:getNodePos(dijkstras.to)

		drawEdge(x1, y1, x2, y2, 0, dijkstras.weight, {1, 0, 0})
	end

	for _, node in ipairs(dijkstras) do
		if node.distance >= 0 then
			local x, y = graph:getNodePos(node.id)
			love.graphics.setColor(1, 0, 0)
			love.graphics.circle("fill", x, y, NODE_INNER_RADIUS)

			love.graphics.setColor(0, 0, 0)
			love.graphics.printf(tostring(node.distance), x - 20, y - 10, 40, "center")
		end		
	end
end

function love.draw()
	regions:draw()
end
