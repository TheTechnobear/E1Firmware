#include "luaTransport.h"
#include "luaLE.h"
#include "MidiInputCallback.h"
#include "System.h"

int luaopen_transport(lua_State *L)
{
    luaL_newlib(L, transport_functions);
    return 1;
}

int transport_enable(lua_State *L)
{
    lua_settop(L, 0);

    System::logger.write(LOG_INFO, "lua: transport enabled");

    MidiInputCallback::onMidiClockCallback = &transport_onMidiClock;
    MidiInputCallback::onMidiStartCallback = &transport_onMidiStart;
    MidiInputCallback::onMidiStopCallback = &transport_onMidiStop;
    MidiInputCallback::onMidiContinueCallback = &transport_onMidiContinue;
    MidiInputCallback::onMidiSongSelectCallback = &transport_onMidiSongSelect;
    MidiInputCallback::onMidiSongPositionCallback =
        &transport_onMidiSongPosition;

    return (0);
}

int transport_disable(lua_State *L)
{
    lua_settop(L, 0);

    System::logger.write(LOG_INFO, "lua: transport disabled");

    MidiInputCallback::onMidiClockCallback = nullptr;
    MidiInputCallback::onMidiStartCallback = nullptr;
    MidiInputCallback::onMidiStopCallback = nullptr;
    MidiInputCallback::onMidiContinueCallback = nullptr;
    MidiInputCallback::onMidiSongSelectCallback = nullptr;
    MidiInputCallback::onMidiSongPositionCallback = nullptr;

    return (0);
}

int transport_isEnabled(lua_State *L)
{
    lua_pushboolean(
        L, (MidiInputCallback::onMidiClockCallback == nullptr) ? false : true);
    return (1);
}

void transport_onMidiClock(MidiInput midiInput)
{
    midi_onSingleByte("transport", "onClock", midiInput);
}

void transport_onMidiStart(MidiInput midiInput)
{
    midi_onSingleByte("transport", "onStart", midiInput);
}

void transport_onMidiStop(MidiInput midiInput)
{
    midi_onSingleByte("transport", "onStop", midiInput);
}

void transport_onMidiContinue(MidiInput midiInput)
{
    midi_onSingleByte("transport", "onContinue", midiInput);
}

void transport_onMidiSongSelect(MidiInput midiInput, uint8_t songNumber)
{
    midi_onTwoBytes("transport", "onSongSelect", midiInput, songNumber);
}

void transport_onMidiSongPosition(MidiInput midiInput, int position)
{
    midi_onTwoBytes("transport", "onSongPosition", midiInput, position);
}
