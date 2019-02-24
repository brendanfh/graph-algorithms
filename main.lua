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
	regions = Regions:new()

	graph_region = {
		-- needed for Regions calculations
		priority = 1;
		rect = { 200, 0, 1000, 800 };

		-- other properties
		selectedNode = nil;
		postDrawFunction = nil;
		clickNodeFunction = nil;
		isMouseDown = false;

		update = function(self) end;
		draw = function(self)
			love.graphics.setColor(0.7, 0.8, 0.9)
			love.graphics.rectangle("fill", 0, 0, 1000, 800)
			drawGraph(graph, self.selectedNode)

			if self.postDrawFunction then
				self.postDrawFunction(self)
			end
		end;

		mousedown = function(self, x, y)
			self.isMouseDown = true
			self.selectedNode = nil
			local nodes = graph:getNodes()
			local edges = graph:getEdges()

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

					return
				end
			end

			for _, edge in pairs(edges) do
				local x1, y1 = graph:getNodePos(edge.from_node)
				local x2, y2 = graph:getNodePos(edge.to_node)

				local d1 = math.sqrt(math.sqrDist(x1, y1, x, y))
				local d2 = math.sqrt(math.sqrDist(x2, y2, x, y))
				local d3 = math.sqrt(math.sqrDist(x1, y1, x2, y2))

				if d1 + d2 <= d3 + 2 then
					createWeightChanger(edge.id, (x1 + x2) / 2 + 100, (y1 + y2) / 2 - 50)
					return
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

	-- Sidebar
	regions:add {
		priority = 1;
		rect = { 0, 0, 200, 800 };

		buttons = {
			{
				text = "Clear",
				click = function()
					graph:clear()
				end;
			};
			{
				text = "Add node",
				placeX = 20,
				placeY = 20,
				click = function(self)
					graph:addNode(self.placeX, self.placeY)
					self.placeX = self.placeX + 40
					if self.placeX >= 1000 then
						self.placeX = 20
						self.placeY = self.placeY + 40
					end
				end;
			};
			{
				text = "Delete node",
				click = function()
					graph:deleteNode(graph_region.selectedNode)
					graph_region.selectedNode = nil
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
				text = "Delete arc";
				click = function()
					local curr = graph_region.selectedNode
					if curr == nil then return end

					graph_region.clickNodeFunction = function(node)
						local id = graph:getEdgeID(curr, node.id, nil)
						if id >= 0 then
							graph:deleteArc(id)
						end
						graph_region.clickNodeFunction = nil
					end
				end;
			};
			{
				text = "Run Dijkstras",
				click = function()
					if graph_region.selectedNode ~= nil then
						function dijkstrasStepper(start, step)
							local dj, done = graph:dijkstras(start, step)
							graph_region.postDrawFunction = function(reg)
								drawDijkstras(graph, dj)
							end
							return done
						end
						createStepper(graph_region.selectedNode, dijkstrasStepper)
					end
				end;
			};
			{
				text = "Run DFS",
				click = function()
					if graph_region.selectedNode ~= nil then
						function dfsStepper(start, step)
							local dfs, done = graph:depthFirst(start, step)
							graph_region.postDrawFunction = function(reg)
								drawTree(graph, dfs)
							end
							return done
						end

						createStepper(graph_region.selectedNode, dfsStepper, 0)
					end
				end;
			};
			{
				text = "Run BFS",
				click = function()
					if graph_region.selectedNode ~= nil then
						function bfsStepper(start, step)
							local bfs, done = graph:breadthFirst(start, step)
							graph_region.postDrawFunction = function(reg)
								drawTree(graph, bfs)
							end
							return done
						end
						
						createStepper(graph_region.selectedNode, bfsStepper, 0)
					end
				end;
			};
			{
				text = "Run Prims",
				click = function()
					if graph_region.selectedNode ~= nil then
						function primStepper(start, step)
							local prims, done = graph:prims(start, step)
							graph_region.postDrawFunction = function(reg)
								drawTree(graph, prims)
							end
							return done
						end
						
						createStepper(graph_region.selectedNode, primStepper, 0)
					end
				end;
			};
		};
		mousePos = -1;

		update = function(self) end;

		draw = function(self)
			love.graphics.setColor(1, 1, 1)
			love.graphics.rectangle("fill", unpack(self.rect))
			love.graphics.setFont(SIDEBAR_FONT)

			for i, button in ipairs(self.buttons) do
				if self.mousePos >= (i - 1) * 60 + 10 and self.mousePos < i * 60 - 10 then
					love.graphics.setColor(.9, .9, .9)
				else
					love.graphics.setColor(.7, .7, .7)
				end
				love.graphics.rectangle("fill", 10, (i - 1) * 60 + 10, 180, 40)

				love.graphics.setColor(0, 0, 0)
				love.graphics.printf(button.text, 0, (i - 1) * 60 + 15, 200, "center")	
			end
		end;

		mousedown = function(self, x, y)
			for i, button in ipairs(self.buttons) do
				if y >= (i - 1) * 60 + 10 and y <= i * 60 + 10 then
					button:click()
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

local stepperOpen = false
function createStepper(start, update, startStep)
	if stepperOpen then return end

	stepperOpen = true
	startStep = startStep or -1
	local step = startStep

	update(start, step)

	local isDone = false
	local reg_id
	local width = 1000
	local region = {
		priority = 10;
		rect = { 200, 750, width, 50 };

		update = function(self) end;
		draw = function(self)
			love.graphics.setFont(SIDEBAR_FONT)
			love.graphics.setColor(0, 0, 0, 0.7)
			love.graphics.rectangle("fill", 0, 0, width, 50)

			love.graphics.setColor(1, 1, 1)
			love.graphics.polygon("fill", (width / 2) - 35, 25, (width / 2) - 5, 10, (width / 2) - 5, 40)
			love.graphics.polygon("fill", (width / 2) + 35, 25, (width / 2) + 5, 10, (width / 2) + 5, 40)

			love.graphics.printf("X", width - 60, 5, 50, "center", 0, 1.4, 1.4)
		end;
		mousedown = function(self, x, y)
			if x >= (width / 2) - 35 and x <= (width / 2) - 5 then
				step = step - 1
				isDone = false
				if step < startStep then step = startStep end
			elseif x >= (width / 2) + 5 and x <= (width / 2) + 35 then
				if not isDone then
					step = step + 1
				end
			end

			isDone = update(start, step)
			if x >= width - 50 then
				regions:remove(reg_id)
				graph_region.postDrawFunction = nil
				stepperOpen = false
			end
		end;
		mouseup = function(self, x, y) end;
		mousemove = function(self, x, y, dx, dy) end;
		mouseenter = function(self) end;
		mouseleave = function(self) end;
	}

	reg_id = regions:add(region)
end

function createWeightChanger(edgeID, x, y)
	if y < 0 then
		y = 0
	end

	local reg_id
	reg_id = regions:add {
		priority = 100;
		rect = { x, y, 200, 100 };

		update = function(self) end;
		draw = function(self)
			love.graphics.setColor(0, 0, 0, 0.8)
			love.graphics.rectangle("fill", 0, 0, 200, 100)

			love.graphics.setColor(1, 1, 1)
			love.graphics.setFont(SIDEBAR_FONT)
			love.graphics.printf("Weight", 0, 0, 200, "center")

			love.graphics.setFont(GRAPH_FONT)
			love.graphics.printf("X", 0, 0, 190, "right")

			love.graphics.setFont(SIDEBAR_FONT)
			local edge = graph:getEdge(edgeID)
			if edge then
				love.graphics.printf(tostring(edge.weight), 0, 45, 200, "center")

				love.graphics.setColor(0, 1, 0)
				love.graphics.circle("fill", 160, 60, 20)

				love.graphics.setColor(1, 0, 0)
				love.graphics.circle("fill", 40, 60, 20)

				love.graphics.setColor(1, 1, 1)
				love.graphics.rectangle("fill", 156, 45, 8, 30)
				love.graphics.rectangle("fill", 145, 56, 30, 8)
				love.graphics.rectangle("fill", 25, 56, 30, 8)
			end
		end;
		mousedown = function(self, x, y)
			if math.sqrDist(x, y, 160, 60) <= 20 * 20 then
				local edge = graph:getEdge(edgeID)
				local otherEdge = graph:getEdgeID(edge.to_node, edge.from_node, edge.weight)

				if otherEdge ~= -1 then
					graph:setEdgeWeight(otherEdge, edge.weight + 1)
				end

				graph:setEdgeWeight(edge.id, edge.weight + 1)
			end
			if math.sqrDist(x, y, 40, 60) <= 20 * 20 then
				local edge = graph:getEdge(edgeID)
				local otherEdge = graph:getEdgeID(edge.to_node, edge.from_node, edge.weight)

				if edge.weight >= 2 then
					if otherEdge ~= -1 then
						graph:setEdgeWeight(otherEdge, edge.weight - 1)
					end

					graph:setEdgeWeight(edge.id, edge.weight - 1)
				end
			end

			if x >= 170 and y <= 30 then
				regions:remove(reg_id)
			end
		end;
		mouseup = function(self, x, y) end;
		mousemove = function(self, x, y, dx, dy) end;
		mouseenter = function(self) end;
		mouseleave = function(self) end;
	}
end

-- DRAWING FUNCTIONS
local NODE_INNER_RADIUS = 17
local NODE_OUTER_RADIUS = 20

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

	if weight ~= nil then
		love.graphics.setColor(1, 1, 1)
		love.graphics.printf(tostring(weight), cx - 20, cy - 10, 40, "center")
	end
end

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

function drawTree(graph, tree)
	love.graphics.setFont(GRAPH_FONT)

	for _, edge in ipairs(tree) do
		local x1, y1 = graph:getNodePos(edge.from)
		local x2, y2 = graph:getNodePos(edge.to)

		drawEdge(x1, y1, x2, y2, 0, nil, {1, 0, 0})
	end
end


-- Love 
function love.update()
	regions:update()
end

function love.draw()
	regions:draw()
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
