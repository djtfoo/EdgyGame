#pragma once

#include <lua.hpp>
#include "Vector3.h"

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
    char GetCharValue(const char* varName); // Get a char value through the Lua Interface Class

    // Get varied number of values through the Lua Interface Class
    int GetVariableValues(const char* varName, int &a, int &b, int &c, int &d);

    float GetDistanceSquareValue(const char* varName, const Vector3& source, const Vector3& destination);

    Vector3 GetVector3Values(const char* varName);  // Get Vector3 values through the Lua Interface Class

    // Extract a field from a table
    float GetField(const char* key);

    // Get error message using an error code
    void GetError(const char* errorCode);

    void SaveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);      // Save a float value through the Lua Interface Class
    void SaveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);    // Save a float value through the Lua Interface Class

    lua_State *theLuaState;
    lua_State *theErrorState;
};