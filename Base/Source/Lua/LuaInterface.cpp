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

    theErrorState = lua_open();
    if (theLuaState && theErrorState)
    {
        luaL_openlibs(theLuaState);
        luaL_dofile(theErrorState, "Image//errorLookup.lua");
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

char CLuaInterface::GetCharValue(const char* varName)
{
    lua_getglobal(theLuaState, varName);

    size_t len;
    const char* cstr = lua_tolstring(theLuaState, -1, &len);
    // if the string is not empty, then return the first char
    if (len > 0)
        return cstr[0];
    else
        return ' ';
}

Vector3 CLuaInterface::GetVector3Values(const char* varName)
{
    lua_getglobal(theLuaState, varName);
    lua_rawgeti(theLuaState, -1, 1);
    
    int x = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);
    lua_rawgeti(theLuaState, -1, 2);
    
    int y = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);
    lua_rawgeti(theLuaState, -1, 3);

    int z = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);

    return Vector3(x, y, z);
}

float CLuaInterface::GetDistanceSquareValue(const char* varName, const Vector3& source, const Vector3& destination)
{
    lua_getglobal(theLuaState, varName);
    lua_pushnumber(theLuaState, source.x);
    lua_pushnumber(theLuaState, source.y);
    lua_pushnumber(theLuaState, source.z);
    lua_pushnumber(theLuaState, destination.x);
    lua_pushnumber(theLuaState, destination.y);
    lua_pushnumber(theLuaState, destination.z);
    lua_call(theLuaState, 6, 1);
    float distanceSquare = (float)lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);

    return distanceSquare;
}

int CLuaInterface::GetVariableValues(const char* varName, int &a, int &b, int &c, int &d)
{
    lua_getglobal(theLuaState, varName);
    lua_pushnumber(theLuaState, 1000);
    lua_pushnumber(theLuaState, 2000);
    lua_pushnumber(theLuaState, 3000);
    lua_pushnumber(theLuaState, 4000);
    lua_call(theLuaState, 4, 4);

    a = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);
    b = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);
    c = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);
    d = lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);

    return true;
}

float CLuaInterface::GetField(const char* key)
{
    int result = false;

    if (!lua_istable(theLuaState, -1))
        GetError("error100");

    lua_pushstring(theLuaState, key);
    lua_gettable(theLuaState, -2);
    if (!lua_isnumber(theLuaState, -1))
        GetError("error101");

    result = (int)lua_tonumber(theLuaState, -1);
    lua_pop(theLuaState, 1);

    return result;
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

void CLuaInterface::GetError(const char* errorCode)
{
    if (theErrorState == NULL)
        return;

    lua_getglobal(theErrorState, errorCode);
    const char* errorMsg = lua_tostring(theErrorState, -1);
    if (errorMsg != NULL)
        std::cout << errorMsg << std::endl;
    else
        std::cout << errorCode << " is not valid.\n*** Please contact the developer ***" << std::endl;
}