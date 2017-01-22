#include "LuaInterface.h"
#include <iostream>

CLuaInterface* CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface()
{
}

CLuaInterface::~CLuaInterface()
{
}

bool CLuaInterface::Init()
{
    bool result = false;

    theLuaState = lua_open();
    if (theLuaState)
    {
        luaL_openlibs(theLuaState);
        luaL_dofile(theLuaState, "Script//DM2240.lua");

        result = true;
    }

    return result;
}

void CLuaInterface::Run()
{
    if (theLuaState == NULL)
        return;

    lua_getglobal(theLuaState, "title");
    const char *title = lua_tostring(theLuaState, -1);

    lua_getglobal(theLuaState, "width");
    int width = lua_tointeger(theLuaState, -1);

    lua_getglobal(theLuaState, "height");
    int height = lua_tointeger(theLuaState, -1);

    std::cout << title << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Width of screen : " << width << std::endl;
    std::cout << "Height of screen: " << height << std::endl;
}

void CLuaInterface::Drop()
{
    if (theLuaState)
    {
        lua_close(theLuaState);
    }
}

int CLuaInterface::GetIntValue(const char* varName)
{
    lua_getglobal(theLuaState, varName);
    return lua_tointeger(theLuaState, -1);
}

float CLuaInterface::GetFloatValue(const char* varName)
{
    lua_getglobal(theLuaState, varName);
    return (float)lua_tonumber(theLuaState, -1);
}

void CLuaInterface::SaveIntValue(const char* varName, const int value, const bool bOverwrite)
{
    lua_getglobal(theLuaState, "SaveToLuaFile");
    char outputString[80];
    sprintf(outputString, "%s = %d\n", varName, value);
    lua_pushstring(theLuaState, outputString);
    lua_pushinteger(theLuaState, bOverwrite);
    lua_call(theLuaState, 2, 0);
}

void CLuaInterface::SaveFloatValue(const char* varName, const float value, const bool bOverwrite)
{
    lua_getglobal(theLuaState, "SaveToLuaFile");
    char outputString[80];
    sprintf(outputString, "%s = %6.4f\n", varName, value);  // floating point precision
    lua_pushstring(theLuaState, outputString);
    lua_pushinteger(theLuaState, bOverwrite);
    lua_call(theLuaState, 2, 0);
}
