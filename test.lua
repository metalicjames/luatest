function test()
    x = StringVector.new()

    x.push("test")
    x.push("andanotherone")
    x.push("wowowowow")

    if x.get(0) == "test" then
        return 27
    else
        return 4
    end
end
