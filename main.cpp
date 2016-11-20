#include <selene.h>

int pc = 0;

void programCounterHook(lua_State* luaState, lua_Debug* event)
{
    pc++;
    std::cout << "pc: " << pc << std::endl;
}

int main()
{
    sel::State state;

    lua_sethook(state.getState(), &programCounterHook, LUA_MASKCOUNT, 1);

    //state.loadString("function test()\n    return 4\nend\n");
    state.Load("./test.lua");

    std::cout << (int)state["test"]();

    return 0;
}
