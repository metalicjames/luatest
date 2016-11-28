#include <selene.h>
#include <lua5.3/lauxlib.h>

#include <cryptokernel/blockchain.h>
#include <cryptokernel/crypto.h>

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

static void *l_alloc_restricted (void *ud, void *ptr, size_t osize, size_t nsize)
    {
        /* set limit here */
       int* used = (int*)ud;
       const int MAX_SIZE = 102400;

       if(ptr == NULL) {
         /*
          * <http://www.lua.org/manual/5.2/manual.html#lua_Alloc>:
          * When ptr is NULL, osize encodes the kind of object that Lua is
          * allocating.
          *
          * Since we don’t care about that, just mark it as 0.
          */
         osize = 0;
       }

       if (nsize == 0){
         free(ptr);
         *used -= osize; /* substract old size from used memory */
         return NULL;
       }
       else
       {
         if (*used + (nsize - osize) > MAX_SIZE) {/* too much memory in use */
           return NULL;
         }
         ptr = realloc(ptr, nsize);
         if (ptr) {/* reallocation successful? */
           *used += (nsize - osize);
         }
         return ptr;
       }
    }

int main()
{
    CryptoKernel::Blockchain::block chainTip;
    chainTip.height = 500;

    std::unique_ptr<int> ud(new int);
    *(ud.get()) = 0;
    lua_State* _l = lua_newstate(l_alloc_restricted, ud.get());
    luaL_openlibs(_l);

    sel::State state(_l);

    lua_sethook(state.getState(), &programCounterHook, LUA_MASKCOUNT, 1000);

    state["Crypto"].SetClass<CryptoKernel::Crypto, bool>("getPublicKey", &CryptoKernel::Crypto::getPublicKey,
                                                         "getPrivateKey", &CryptoKernel::Crypto::getPrivateKey,
                                                         "setPublicKey", &CryptoKernel::Crypto::setPublicKey,
                                                         "setPrivateKey", &CryptoKernel::Crypto::setPrivateKey,
                                                         "sign", &CryptoKernel::Crypto::sign,
                                                         "verify", &CryptoKernel::Crypto::verify,
                                                         "getStatus", &CryptoKernel::Crypto::getStatus
                                                        );
    state["chainTip"].SetObj(chainTip, "height", &CryptoKernel::Blockchain::block::height);

    state.Load("./sandbox.lua");

    const bool result = state["verifyTransaction"]();
    std::cout << result;

    return 0;
}
