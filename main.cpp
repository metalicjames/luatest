#include <selene.h>
#include <lua5.3/lauxlib.h>

int pc = 0;
const int pcLimit = 500;

void programCounterHook(lua_State* luaState, lua_Debug* event)
{
    pc++;
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
    sel::State state;

    lua_sethook(state.getState(), &programCounterHook, LUA_MASKCOUNT, 1);

    state["StringVector"].SetClass<StringVector>("push", &StringVector::push, "get", &StringVector::get);

    state.Load("./test.lua");

    const int result = state["test"]();
    std::cout << result;

    return 0;
}
