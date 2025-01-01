print( "Made by FlyNesty" )
print( "flynesty.space" )

local _fetch_stubmodule do
    local current_module = 1
    local modules_list = {}
    local active_modules = {}

    for _, object in game:FindService("CoreGui").RobloxGui.Modules:GetDescendants() do
    	if not object:IsA("ModuleScript") then
    		if object.Name:match("AvatarExperience") then
    			for _, o in object:GetDescendants() do
    				if o.Name == "Flag" or o.Name == "Test" then
    					for _, oa in o.GetDescendants() do
    					    if not oa:IsA("ModuleScript") then continue end
    					    table.insert( modules_list, oa:Clone() )
    				    end
    				end
    			end
    		else
    			if object.Name:match("ReportAnything") or object.Name:match("TestHelpers") then
    				for _, o in object:GetDescendants() do
    				    if not o:IsA("ModuleScript") then continue end
    				    table.insert( modules_list, o:Clone() )
    				end
    			end
    		end
    	end
    end

    local function find_new_module()
    	local idx = math.random( 1, #modules_list )
    	while idx == current_module or active_modules[idx] do
    		idx = math.random( 1, #modules_list )
    	end
    	return idx
    end

    function _fetch_stubmodule()
    	local idx = find_new_module()

    	active_modules[current_module] = nil
    	current_module = idx
    	active_modules[current_module] = true

    	return modules_list[idx]
    end
end

local fetch_stubmodule = _fetch_stubmodule

if script.Name == "JestGlobals" then
	local indicator = Instance.new("BoolValue")
	indicator.Name = "Exec"
	indicator.Parent = script

	local holder = Instance.new("ObjectValue")
	holder.Parent = script
	holder.Name = "Holder"
	holder.Value = fetch_stubmodule:Clone()
	print(holder.Value)

	local lsindicator = Instance.new("BoolValue")
	lsindicator.Name = "Loadstring"
	lsindicator.Parent = script

	local lsholder = Instance.new("ObjectValue")
	lsholder.Parent = script
	lsholder.Name = "LoadstringHolder"
	lsholder.Value = fetch_stubmodule:Clone()
	print( lsholder.Value )
end
