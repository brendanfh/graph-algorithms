local Regions = {}
function Regions:new()
	local obj = {}

	local mt = { __index = Regions }
	setmetatable(obj, mt)

	return obj
end

function Regions:add(region)
	local id = math.uuid()
	region.id = id

	table.insert(self, region)

	table.sort(self, function(a, b)
		return a.priority > b.priority
	end)

	return id
end

function Regions:remove(regionID)
	local idx = -1
	for i, reg in ipairs(self) do
		if reg.id == regionID then
			idx = i
			break
		end
	end

	if idx >= 0 then
		table.remove(self, idx)
		table.sort(self, function(a, b)
			return a.priority > b.priority
		end)
	end
end

function Regions:draw()
	for i = #self, 1, -1 do
		local region = self[i]
		local x, y, w, h = unpack(region.rect)
		love.graphics.setScissor(x, y, w, h)
		love.graphics.push()
		love.graphics.translate(x, y)

		region:draw()

		love.graphics.pop()
		love.graphics.setScissor()
	end
end

function Regions:mousedown(x, y)
	for _, region in ipairs(self) do
		local rx, ry, rw, rh = unpack(region.rect)

		if x >= rx and x <= rx + rw
		 	and y >= ry and y <= ry + rh then

		 	region:mousedown(x - rx, y - ry)
		 	break
		end
	end
end

function Regions:mouseup(x, y)
	for _, region in ipairs(self) do
		local rx, ry, rw, rh = unpack(region.rect)

		if x >= rx and x <= rx + rw
		 	and y >= ry and y <= ry + rh then

		 	region:mouseup(x - rx, y - ry)
		 	break
		end
	end
end

function Regions:mousemove(x, y, dx, dy)
	local invoked = false
	for _, region in ipairs(self) do
		local rx, ry, rw, rh = unpack(region.rect)

		if not invoked
			and x >= rx and x <= rx + rw
		 	and y >= ry and y <= ry + rh then

		 	region:mousemove(x - rx, y - ry, dx, dy)

		 	if not region.ismouseover then
		 		region:mouseenter()
		 	end

		 	region.ismouseover = true
		 	invoked = true
		else
			if region.ismouseover then
				region.ismouseover = false
				region:mouseleave()
			end
		end
	end
end

function Regions:update()
	for _, region in ipairs(self) do
		region:update()
	end
end

return Regions