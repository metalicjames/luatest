#include <selene.h>
#include <lua5.3/lauxlib.h>

int pc = 0;
const int pcLimit = 500;

void programCounterHook(lua_State* luaState, lua_Debug* event)
{
    pc++;
    std::cout << "pc: " << pc << std::endl;

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
    sel::State state(true);

    lua_sethook(state.getState(), &programCounterHook, LUA_MASKCOUNT, 1);

    state["StringVector"].SetClass<StringVector>("push", &StringVector::push, "get", &StringVector::get);

    //state.loadString("function test()\n    return 4\nend\n");
    state.Load("./test.lua");

    std::cout << (bool)state["sandboxTest"]();

    std::cout << "\nWe got this far...";

    return 0;
}
