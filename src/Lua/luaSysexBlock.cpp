#include "luaSysexBlock.h"
#include "MemoryPool.h"
#include "luaLE.h"

int sysexBlock_create(lua_State *L)
{
    lua_settop(L, 0);
    SysexBlock *sysexBlock =
        new SysexBlock(App::get()->sysexPool.openMemoryBlock());

    if (sysexBlock) {
        luaLE_pushObject(L, "SysexBlock", sysexBlock);
        return (1);
    }

    return (luaL_error(L, "failed: sysexBlock %d does not exist"));
}

int sysexBlock_delete(lua_State *L)
{
    lua_settop(L, 1);
    return (0);
}

int sysexBlock_close(lua_State *L)
{
    lua_settop(L, 1);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));

    if (!sysexBlock) {
        return (luaL_error(L, "close: invalid SysexBlock"));
    }

    sysexBlock->close();
    return (0);
}

int sysexBlock_write(lua_State *L)
{
    lua_settop(L, 2);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));
    uint8_t byte = luaL_checkinteger(L, 2);

    if (!sysexBlock) {
        return (luaL_error(L, "seek: invalid SysexBlock"));
    }

    sysexBlock->writeBytes(&byte, 1);
    return (0);
}

int sysexBlock_read(lua_State *L)
{
    lua_settop(L, 1);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));

    if (sysexBlock) {
        lua_pushnumber(L, sysexBlock->read());
        return (1);
    }

    return (luaL_error(L, "read: invalid SysexBlock"));
}

int sysexBlock_seek(lua_State *L)
{
    lua_settop(L, 2);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));
    size_t index = luaL_checkinteger(L, 2);

    if (!sysexBlock) {
        return (luaL_error(L, "seek: invalid SysexBlock"));
    }

    if (index > 0) {
        index--;
    } else {
        return (luaL_error(L, "seek: index cannot be 0 or negative"));
    }

    if (index >= sysexBlock->getLength()) {
        return (luaL_error(L, "seek: index is outside of the SysexBlock"));
    }

    sysexBlock->seek(index);

    return (0);
}

int sysexBlock_peek(lua_State *L)
{
    lua_settop(L, 2);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));
    size_t index = luaL_checkinteger(L, 2);

    if (!sysexBlock) {
        return (luaL_error(L, "peek: invalid SysexBlock"));
    }

    if (index > 0) {
        index--;
    } else {
        return (luaL_error(L, "peek: index cannot be 0 or negative"));
    }

    if (index >= sysexBlock->getLength()) {
        return (luaL_error(L, "peek: index is outside of the SysexBlock"));
    }

    int byte = sysexBlock->peek(index);

    lua_pushnumber(L, byte);

    return (1);
}

int sysexBlock_getLength(lua_State *L)
{
    lua_settop(L, 1);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));

    if (sysexBlock) {
        lua_pushnumber(L, sysexBlock->getLength());
        return (1);
    }

    return (luaL_error(L, "getLength: invalid SysexBlock"));
}

int sysexBlock_getManufacturerSysexId(lua_State *L)
{
    lua_settop(L, 1);

    SysexBlock *sysexBlock =
        (*reinterpret_cast<SysexBlock **>(luaL_checkudata(L, 1, "SysexBlock")));

    if (sysexBlock) {
        lua_pushnumber(L, sysexBlock->getManufacturerSysexId());
        return (1);
    }

    return (luaL_error(L, "getManufacturerSysexId: invalid SysexBlock"));
}

void sysexBlock_register(lua_State *L)
{
    lua_register(L, "SysexBlock", sysexBlock_create);
    luaL_newmetatable(L, "SysexBlock");

    lua_pushcfunction(L, sysexBlock_delete);
    lua_setfield(L, -2, "__gc");

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    luaLE_addObjectMethods(L, sysexBlock_functions);

    lua_pop(L, 1);
}
