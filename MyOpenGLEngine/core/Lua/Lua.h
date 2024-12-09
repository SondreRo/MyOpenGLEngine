#pragma once
//#include <lua.h>

#include <filesystem>

//extern "C"
//{
//#include "lua.h"
//#include "lauxlib.h"
//#include "lualib.h"
//}

#include "lua.hpp"

class Lua {
public:

	lua_State* L;

	void Init();
	void runInterpreter();
	void RegisterFunction();

	void Cleanup();

	void DoFile(const std::filesystem::path inPath);



};