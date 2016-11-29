function verify()
    local genCrypto = Crypto.new(true)
    local publicKey = genCrypto:getPublicKey()
    local signature = genCrypto:sign("this is a test signature")

    local i = 0
    while i < 20000 do
        signature = genCrypto:sign("this is a test signature")
        genCrypto:verify("this is a test signature", signature)
        i = i + 1
    end

    genCrypto = nil

    local crypto = Crypto.new()
    crypto:setPublicKey(publicKey)
    if chainTip:height() >= 500 and crypto:verify("this is a test signature", signature) then
        return true
    else
        return false
    end
end

function compile()
    local s = string.dump(verify)
    local lz4 = require("lz4")
    local options = {compression_level = 16}
    return lz4.compress(s, options)
end
