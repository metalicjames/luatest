function test()
    x = StringVector.new()

    x.push("test")
    x.push("andanotherone")
    x.push("wowowowow")

    if x.get(0) == "test" then
        return 0
    else
        return 4
    end
end

-- make environment
local env = {} -- add functions you know are safe here

-- run code under environment
local function run(untrusted_code)
  local untrusted_function, message = load(untrusted_code, nil, 't', env)
  if not untrusted_function then return nil, message end
  return pcall(untrusted_function)
end

function sandboxTest()

    -- test
    if run([[print(debug.getinfo(1))]]) then
        return false
    end

    if not run([[x=1]]) then
        return false
    end

    if run([[while 1 do end]]) then
        return false
    end

    if run([[x = StringVector.new()

    x.push("test")
    x.push("andanotherone")
    x.push("wowowowow")

    if x.get(0) == "test" then
        return 0
    else
        return 4
    end]]) then
        return false
    end

    return true
end
