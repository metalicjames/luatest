#include <selene.h>

int pc = 0;

void programCounterHook(lua_State* luaState, lua_Debug* event)
{
    pc++;
    std::cout << "pc: " << pc << std::endl;
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

    //state.loadString("function test()\n    return 4\nend\n");
    state.Load("./test.lua");

    std::cout << (int)state["test"]();

    return 0;
}
