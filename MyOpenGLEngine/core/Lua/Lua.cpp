#include "Lua.h"

#include "Application.h"

//  C Functions that lua can call
int cAdd(lua_State* L)
{
    // Step 1:  extract the parameters from the lua stack:
    double n1 = lua_tonumber(L, 1);
    double n2 = lua_tonumber(L, 2);

    //  Step 2:  Do the actual calculation.  Normally, this will be more interesting than a single sum!
    double sum = n1 + n2;

    // Step 3:  Push the result on the lua stack. 
    lua_pushnumber(L, sum);

    // Return the number of arguments we pushed onto the stack (that is, the number of return values this
    // function has
    return 1;
}

int average(lua_State* L)
{
    // Get the number of parameters
    int n = lua_gettop(L);

    double sum = 0;
    int i;

    // loop through each argument, adding them up
    for (i = 1; i <= n; i++)
    {
        sum += lua_tonumber(L, i);
    }

    // push the average on the lua stack
    lua_pushnumber(L, sum / n);

    // push the sum on the lua stack
    lua_pushnumber(L, sum);

    // return the number of results we pushed on the stack
    return 2;
}

int GetDeltaTime(lua_State* L)
{
    float dt = Application::get().deltaTime;

	lua_pushnumber(L, dt);

	return 1;
}

int SetPosition(lua_State* L)
{
    // Get the number of parameters
    int n = lua_gettop(L);

    std::string name = lua_tostring(L, 1);

	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);
	float z = lua_tonumber(L, 4);

	if (Application::get().mScene.Meshes.find(name) == Application::get().mScene.Meshes.end())
	{
		return 0;
	}
	
	Application::get().mScene.Meshes[name]->transform.SetLocation(glm::vec3(x, y, z));

	return 2;
}

int AddPosition(lua_State* L)
{
    // Get the number of parameters
    int n = lua_gettop(L);

    std::string name = lua_tostring(L, 1);

    float x = lua_tonumber(L, 2);
    float y = lua_tonumber(L, 3);
    float z = lua_tonumber(L, 4);

    if (Application::get().mScene.Meshes.find(name) == Application::get().mScene.Meshes.end())
    {
        return 0;
    }

    Application::get().mScene.Meshes[name]->transform.AddLocation(glm::vec3(x, y, z));


    return 0;
}

int CreateEnemy(lua_State* L)
{
    // Get the number of parameters
    int n = lua_gettop(L);

    float x = lua_tonumber(L, 1);
    float y = lua_tonumber(L, 2);
    float z = lua_tonumber(L, 3);

	Application::get().mScene.ecs_manager.CreateEnemy({ x, y, z });

    return 1;
}

void Lua::Init()
{
    // initialize Lua interpreter
    L = luaL_newstate();

    // load Lua base libraries (print / math / etc)
    luaL_openlibs(L);
}

void Lua::runInterpreter()
{
    char inputBuffer[500];
    while (true)
    {
        printf(">");
        fgets(inputBuffer, sizeof inputBuffer, stdin);
        luaL_dostring(L, inputBuffer);
    }
}

void Lua::RegisterFunction()
{
    lua_register(L, "cAdd", cAdd);
    lua_register(L, "average", average);
    lua_register(L, "GetDeltaTime", GetDeltaTime);
    lua_register(L, "SetPosition", SetPosition);
    lua_register(L, "AddPosition", AddPosition);
    lua_register(L, "CreateEnemy", CreateEnemy);
}

void Lua::Cleanup()
{
    lua_close(L);
}


void Lua::DoFile(const std::filesystem::path inPath)
{
	std::string test = inPath.string();
	luaL_dofile(L, test.c_str());
    
}

