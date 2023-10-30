/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

#include "luaIntegrationBase.h"
#include "SysexBlock.h"
#include "System.h"

void luaLE_pushTableString(lua_State *L, const char *key, const char *value)
{
    lua_pushstring(L, key);
    lua_pushstring(L, value);
    lua_settable(L, -3);
}

void luaLE_pushTableInteger(lua_State *L, const char *key, int value)
{
    lua_pushstring(L, key);
    lua_pushinteger(L, value);
    lua_settable(L, -3);
}

void luaLE_pushTableObject(lua_State *L,
                           const char *key,
                           const char *objectName,
                           const void *object)
{
    lua_pushstring(L, key);
    luaLE_pushObject(L, objectName, object);
    lua_settable(L, -3);
}

void luaLE_pushArrayInteger(lua_State *L, int key, int value)
{
    lua_pushinteger(L, key);
    lua_pushinteger(L, value);
    lua_settable(L, -3);
}

void luaLE_pushArrayString(lua_State *L, int key, const char *value)
{
    lua_pushinteger(L, key);
    lua_pushstring(L, value);
    lua_settable(L, -3);
}

void luaLE_pushArrayObject(lua_State *L,
                           int key,
                           const char *objectName,
                           const void *object)
{
    lua_pushinteger(L, key);
    luaLE_pushObject(L, objectName, object);
    lua_settable(L, -3);
}

void luaLE_pushObject(lua_State *L, const char *objectName, const void *object)
{
    *reinterpret_cast<const void **>(lua_newuserdata(L, sizeof(void *))) =
        object;

    luaL_getmetatable(L, objectName);
    lua_setmetatable(L, -2);
}

bool luaLE_checkBoolean(lua_State *L, int idx)
{
    return (lua_toboolean(L, idx));
}

void luaLE_getModuleFunction(lua_State *L,
                             const char *module,
                             const char *function)
{
    lua_getglobal(L, module);
    lua_pushstring(L, function);
    lua_gettable(L, -2);
    lua_remove(L, -2);
}

void luaLE_addObjectMethods(lua_State *L, const luaL_Reg *l)
{
    for (; l->name; l++) {
        lua_pushcfunction(L, l->func);
        lua_setfield(L, -(2), l->name);
    }
}

void luaLE_postFunctionCleanUp(lua_State *L)
{
    /** @attention: trigger the garbage collection.
     * For now, after each function call. This needs to be improved.
     */
    lua_gc(L, LUA_GCCOLLECT, 0);
    lua_settop(L, 0);
}

void luaLE_dumpstack(lua_State *L)
{
    int top = lua_gettop(L);

    System::logger.write(LOG_LUA, "stack size: %d", top);

    for (int i = 1; i <= top; i++) {
        System::logger.write(LOG_LUA, "%d -> %s   ", i, luaL_typename(L, i));

        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                System::logger.write(LOG_LUA, "number: %g", lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                System::logger.write(LOG_LUA, "string: %s", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                System::logger.write(LOG_LUA,
                                     "bool: %s",
                                     (lua_toboolean(L, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                System::logger.write(LOG_LUA, "nil: %s", "nil");
                break;
            default:
                System::logger.write(
                    LOG_LUA, "pointer: %p", lua_topointer(L, i));
                break;
        }
    }
}

bool luaLE_functionExists(const char *module, const char *function)
{
    luaLE_getModuleFunction(module, function);

    if (lua_isfunction(L, -1)) {
        return (true);
    }

    return (false);
}

void luaLE_getModuleFunction(const char *module, const char *function)
{
    lua_getglobal(L, module);
    lua_getfield(L, -1, function);
}
