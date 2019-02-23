function math.lerp(a, b, t)
	return a + (b - a) * t
end

function math.uuid()
	return ("xxxxxxxx-xxxx-4yxx-xxxxxxxx"):gsub('[xy]', function (c)
        local v = (c == 'x') and math.random(0, 0xf) or math.random(8, 0xb)
        return string.format('%x', v)
    end)
end

local function ripairsiter(t, i)
	i = i - 1
	if i ~= 0 then
		return i, t[i]
	end
end

function reversedipairs(t)
	return ripairsiter, t, #t + 1
end