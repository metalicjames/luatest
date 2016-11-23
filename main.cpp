#include <selene.h>
#include <lua5.3/lauxlib.h>

#include <cryptokernel/blockchain.h>

int pc = 0;
const int pcLimit = 50000;

void programCounterHook(lua_State* luaState, lua_Debug* event)
{
    pc += 1000;
    if(pc > pcLimit)
    {
        luaL_error(luaState, "Script exceeded the instruction limit");
    }
}

class StringVector
{
    public:
        StringVector() {}
        void push(const std::string val)
        {
            vec.push_back(val);
        }
        std::string get(const int index)
        {
            return vec[index];
        }
    private:
        std::vector<std::string> vec;
};

int main()
{
    CryptoKernel::Blockchain::block chainTip;
    chainTip.height = 499;

    sel::State state{true};

    lua_sethook(state.getState(), &programCounterHook, LUA_MASKCOUNT, 1000);

    state["StringVector"].SetClass<StringVector>("push", &StringVector::push, "get", &StringVector::get);

    state["chainTip"].SetObj(chainTip, "height", &CryptoKernel::Blockchain::block::height);

    state.Load("./sandbox.lua");

    const bool result = state["verify"]();
    std::cout << result;

    return 0;
}
