#include <selene.h>

int main()
{
    sel::State state;

    state.loadString("function test()\n    return 4\nend\n");

    std::cout << (int)state["test"]();

    return 0;
}
