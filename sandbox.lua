-- save a pointer to globals that would be unreachable in sandbox
local e=_ENV

-- sample sandbox environment
sandbox_env = {
  StringVector = {new = StringVector.new, get = StringVector.get, push = StringVector.push,},
  Crypto = {new = Crypto.new, getPublicKey = Crypto.getPublicKey, getPrivateKey = Crypto.getPrivateKey,
            setPublicKey = Crypto.setPublicKey, setPrivateKey = Crypto.setPrivateKey,
            getStatus = Crypto.getStatus, sign = Crypto.sign, verify = Crypto.verify,},
  chainTip = {height = chainTip.height},
  print = print,
  --[[ipairs = ipairs,
  next = next,
  pairs = pairs,
  pcall = pcall,
  tonumber = tonumber,
  tostring = tostring,
  type = type,
  unpack = unpack,
  coroutine = { create = coroutine.create, resume = coroutine.resume,
      running = coroutine.running, status = coroutine.status,
      wrap = coroutine.wrap },
  string = { byte = string.byte, char = string.char, find = string.find,
      format = string.format, gmatch = string.gmatch, gsub = string.gsub,
      len = string.len, lower = string.lower, match = string.match,
      rep = string.rep, reverse = string.reverse, sub = string.sub,
      upper = string.upper },
  table = { insert = table.insert, maxn = table.maxn, remove = table.remove,
      sort = table.sort },
  math = { abs = math.abs, acos = math.acos, asin = math.asin,
      atan = math.atan, atan2 = math.atan2, ceil = math.ceil, cos = math.cos,
      cosh = math.cosh, deg = math.deg, exp = math.exp, floor = math.floor,
      fmod = math.fmod, frexp = math.frexp, huge = math.huge,
      ldexp = math.ldexp, log = math.log, log10 = math.log10, max = math.max,
      min = math.min, modf = math.modf, pi = math.pi, pow = math.pow,
      rad = math.rad, random = math.random, sin = math.sin, sinh = math.sinh,
      sqrt = math.sqrt, tan = math.tan, tanh = math.tanh },
  os = { clock = os.clock, difftime = os.difftime, time = os.time },]]--
}

function run_sandbox(sb_env, sb_func, ...)
  local sb_orig_env=_ENV
  if (not sb_func) then return nil end
  _ENV=sb_env
  local sb_ret={e.pcall(sb_func, ...)}
  _ENV=sb_orig_env
  return e.table.unpack(sb_ret)
end

function verifyTransaction()
    pcall_rc, result_or_err_msg = run_sandbox(sandbox_env, verify)
    return result_or_err_msg
end

function verify()
    if chainTip:height() >= 500 then
        return true
    else
        return false
    end
end

