#pragma once

#include <lua.hpp>

class CLuaInterface
{
protected:
    static CLuaInterface *s_instance;
    CLuaInterface();

public:
    static CLuaInterface *GetInstance()
    {
        if (!s_instance)
            s_instance = new CLuaInterface();
        return s_instance;
    }

    static bool DropInstance()
    {
        if (s_instance)
        {
            s_instance->Drop();

            delete s_instance;
            s_instance = NULL;
            return true;
        }

        return false;
    }

    virtual ~CLuaInterface();

    bool Init();
    void Run();
    void Drop();

    int GetIntValue(const char* varName);   // Get an integer value through the Lua Interface Class
    float GetFloatValue(const char* varName);   // Get a float value through the Lua Interace Class

    void SaveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);      // Save a float value through the Lua Interface Class
    void SaveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);    // Save a float value through the Lua Interface Class

    lua_State *theLuaState;
};