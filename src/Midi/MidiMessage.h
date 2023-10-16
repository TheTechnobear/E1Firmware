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

/**
 * @file MidiMessage.h
 *
 * @brief A representation of a standard MIDI Message.
 */

#pragma once

#include <vector>
#include "SysexBlock.h"

class MidiMessage
{
public:
    enum Type {
        InvalidType = 0x00,
        NoteOff = 0x80,
        NoteOn = 0x90,
        AfterTouchPoly = 0xA0,
        ControlChange = 0xB0,
        ProgramChange = 0xC0,
        AfterTouchChannel = 0xD0,
        PitchBend = 0xE0,
        SystemExclusive = 0xF0,
        TimeCodeQuarterFrame = 0xF1,
        SongPosition = 0xF2,
        SongSelect = 0xF3,
        TuneRequest = 0xF6,
        Clock = 0xF8,
        Start = 0xFA,
        Continue = 0xFB,
        Stop = 0xFC,
        ActiveSensing = 0xFE,
        SystemReset = 0xFF
    };

    /** Constructor. */
    MidiMessage();

    /** Constructor. */
    MidiMessage(uint8_t newChannel,
                Type newType,
                uint8_t newData1,
                uint8_t newData2);

    /** Constructor */
    MidiMessage(const SysexBlock &sysexBlock);

    /** Destructor. */
    virtual ~MidiMessage() = default;

    uint8_t getChannel(void) const;
    Type getType(void) const;
    uint8_t getData1(void) const;
    uint8_t getData2(void) const;
    const char *getDescription(void) const;

    bool isController(void) const;
    uint8_t getControllerNumber(void) const;
    uint8_t getControllerValue(void) const;
    static MidiMessage
        controllerEvent(uint8_t channel, uint8_t controllerType, uint8_t value);

    bool isNote(void) const;
    bool isNoteOn(void) const;
    static MidiMessage
        noteOn(uint8_t channel, uint8_t noteNumber, uint8_t velocity);
    bool isNoteOff(void) const;
    static MidiMessage
        noteOff(uint8_t channel, uint8_t noteNumber, uint8_t velocity);
    static MidiMessage noteOff(uint8_t channel, uint8_t noteNumber);
    bool isNoteOnOrOff(void) const;
    uint8_t getNoteNumber(void) const;
    void setNoteNumber(uint8_t newNoteNumber);
    uint8_t getVelocity(void) const;
    void setVelocity(uint8_t newVelocity);

    bool isProgramChange() const;
    uint8_t getProgramChangeNumber(void) const;
    static MidiMessage programChange(uint8_t channel, uint8_t programNumber);

    bool isPitchWheel(void) const;
    uint16_t getPitchWheelValue(void) const;
    static MidiMessage pitchWheel(uint8_t channel, uint16_t position);

    bool isAftertouch(void) const;
    uint8_t getAfterTouchNote(void) const;
    uint8_t getAfterTouchValue(void) const;
    static MidiMessage aftertouchChange(uint8_t channel,
                                        uint8_t noteNumber,
                                        uint8_t aftertouchAmount);
    bool isChannelPressure(void) const;
    uint8_t getChannelPressureValue(void) const;
    static MidiMessage channelPressureChange(uint8_t channel, uint8_t pressure);

    bool isMidiStart(void) const;
    static MidiMessage midiStart();

    bool isMidiContinue(void) const;
    static MidiMessage midiContinue();

    bool isMidiStop(void) const;
    static MidiMessage midiStop();

    bool isMidiClock(void) const;
    static MidiMessage midiClock();

    bool isMidiTuneRequest(void) const;
    static MidiMessage midiTuneRequest(void);

    bool isMidiActiveSensing(void) const;
    static MidiMessage midiActiveSensing(void);

    bool isMidiSystemReset(void) const;
    static MidiMessage midiSystemReset(void);

    bool isSongPositionPointer(void) const;
    uint16_t getSongPositionPointerMidiBeat(void) const;

    static MidiMessage songPositionPointer(uint16_t positionInMidiBeats);

    bool isSongSelect(void) const;
    uint16_t getSelectedSong(void) const;
    static MidiMessage songSelect(uint8_t song);

    bool isQuarterFrame(void) const;
    uint8_t getQuarterFrameSequenceNumber(void) const;
    uint8_t getQuarterFrameValue(void) const;
    static MidiMessage quarterFrame(uint8_t sequenceNumber, uint8_t value);

    bool isSysEx(void) const;
    uint32_t getSysExDataSize(void) const;
    const SysexBlock &getSysExBlock(void) const;
    SysexBlock &getSysExBlock(void);
    size_t readSysExData(uint8_t *buffer, size_t length);

    bool isBankSelect(void) const;

    static MidiMessage::Type translateType(const char *typeText);
    static const char *translateTypeToText(MidiMessage::Type messageType);

private:
    Type type;
    uint8_t channel;
    uint8_t data1;
    uint8_t data2;
    SysexBlock sysexBlock;
};
