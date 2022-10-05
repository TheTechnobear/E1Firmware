#pragma once

#include "MidiMessage.h"
#include "MidiMessageTransport.h"
#include "CircularBuffer.h"

#define SYSEX_START 0xF0
#define SYSEX_END 0xF7
#define SYSEX_MANUFACTURER 0x002145

/*
 * Commands
 */
#define FILE_UPLOAD 0x01
#define FILE_DOWNLOAD 0x02
#define MIDILEARN_SWITCH 0x03
#define UPDATE 0x04
#define REMOVE 0x05
#define SWAP 0x06
#define CREATE 0x07
#define EXECUTE 0x08
#define UPDATE_RUNTIME 0x14
#define DATA_CONTAINER 0x7D
#define EVENT_MESSAGE 0x7E
#define SYSTEM_CALL 0x7F

/*
 * Events
 */
#define NACK 0x00
#define ACK 0x01
#define PRESET_SWITCH 0x02
#define SNAPSHOT_CHANGE 0x03
#define SNAPSHOT_BANK_SWITCH 0x04
#define PRESET_LIST_CHANGE 0x05

/*
 * Object types
 */
#define FILE_PRESET_LEGACY 0x00
#define FILE_PRESET 0x01
#define FILE_CONFIG 0x02
#define FILE_SNAPSHOT 0x03
#define PRESET_LIST 0x04
#define SNAPSHOT_LIST 0x05
#define SNAPSHOT_INFO 0x06
#define CONTROL 0x07
#define FILE_LUA_SCRIPT 0x0C
#define FUNCTION 0x0D
#define FILE_UITOOLKIT 0x70
#define APP_INFO 0x7C
#define LOGGER 0x7D
#define MEMORY_INFO 0x7E
#define ELECTRA_INFO 0x7F

#define USB_MIDI_PORT_CTRL 2

class MidiOutput : public MidiJack
{
public:
    MidiOutput(MidiInterface::Type newInterface, uint8_t newPort);
    MidiOutput(MidiInterface::Type newInterface,
               uint8_t newPort,
               uint8_t newChannel,
               uint16_t newRate);
    virtual ~MidiOutput() = default;

    /*
     * API commands
     */
    static void
        sendSetLogger(MidiInterface::Type interface, uint8_t port, bool status);
    static void sendPresetSlotChanged(MidiInterface::Type interface,
                                      uint8_t port);
    static void sendSnapshotBankChanged(MidiInterface::Type interface,
                                        uint8_t port,
                                        uint8_t bankNumber);
    static void sendSnapshotChanged(MidiInterface::Type interface,
                                    uint8_t port);
    static void sendPresetSwitched(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t bankNumber,
                                   uint8_t slotId);
    static void sendAck(MidiInterface::Type interface, uint8_t port);
    static void sendNack(MidiInterface::Type interface, uint8_t port);
    static void sendAppInfo(MidiInterface::Type interface, uint8_t port);
    static void sendMemoryInfo(MidiInterface::Type interface, uint8_t port);
    static void sendElectraInfo(MidiInterface::Type interface,
                                uint8_t port,
                                const char *electraInfoSerial,
                                uint8_t electraInfoHwRevision);
    static void sendMidiLearn(MidiInterface::Type interface,
                              uint8_t portToTransmitOn,
                              const char *msg,
                              uint8_t port,
                              uint8_t channel,
                              uint16_t parameterId,
                              uint16_t value);
    static void sendMidiLearnSysex(MidiInterface::Type interface,
                                   uint8_t portToTransmitOn,
                                   uint8_t port,
                                   const SysexBlock &sysexBlock);

    static bool sendSysExFile(uint8_t port,
                              const char *filename,
                              ElectraCommand::Object fileType);

    static void
        sendSysExData(const uint8_t *data, uint32_t length, uint8_t port);
    static void sendSysExHeader(uint8_t port);
    // END of API commands

    void setPort(uint8_t newPort);
    uint8_t getPort(void) const;
    void setChannel(uint8_t newChannel);
    uint8_t getChannel(void) const;
    void setRate(uint16_t newRate);
    uint16_t getRate(void) const;
    void setTsLastMessage(void);
    bool isReady(void);

    // General MIDI messaging (outgoing)
    static void sendMessage(MidiInterface::Type interface,
                            uint8_t port,
                            MidiMessage &message);

    static bool isIdenticalChange(const MidiMessageTransport &m1,
                                  const MidiMessageTransport &m2);

    static void addToQueue(MidiInterface::Type interface,
                           uint8_t port,
                           MidiMessage &message);

    static void send(MidiInterface::Type interface,
                     uint8_t port,
                     MidiMessage::Type type,
                     uint8_t channel,
                     uint8_t data1,
                     uint8_t data2);
    static void
        send(MidiInterface::Type interface, uint8_t port, MidiMessage &message);
    static void send(MidiInterface::Type interface,
                     uint8_t port,
                     SysexBlock &sysexBlock);
    static void sendControlChange(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint8_t channel,
                                  uint8_t parameterNumber,
                                  uint8_t value);
    static void sendNoteOn(MidiInterface::Type interface,
                           uint8_t port,
                           uint8_t channel,
                           uint8_t noteNumber,
                           uint8_t velocity);
    static void sendNoteOff(MidiInterface::Type interface,
                            uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t velocity);
    static void sendStart(MidiInterface::Type interface, uint8_t port);
    static void sendStop(MidiInterface::Type interface, uint8_t port);
    static void sendTuneRequest(MidiInterface::Type interface, uint8_t port);
    static void sendProgramChange(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint8_t channel,
                                  uint8_t programNumber);
    static void sendSysEx(MidiInterface::Type interface,
                          uint8_t port,
                          SysexBlock &sysexBlock);
    static void sendSysEx(MidiInterface::Type interface,
                          uint8_t port,
                          uint8_t *data,
                          uint16_t dataLength);
    static void sendSysExPartial(MidiInterface::Type interface,
                                 uint8_t port,
                                 const uint8_t *data,
                                 uint16_t dataLength,
                                 bool complete);
    static void sendPitchBend(MidiInterface::Type interface,
                              uint8_t port,
                              uint8_t channel,
                              uint16_t value);
    static void sendAfterTouchPoly(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t noteNumber,
                                   uint8_t pressure);
    static void sendAfterTouchChannel(MidiInterface::Type interface,
                                      uint8_t port,
                                      uint8_t channel,
                                      uint8_t pressure);
    static void sendClock(MidiInterface::Type interface, uint8_t port);
    static void sendContinue(MidiInterface::Type interface, uint8_t port);
    static void sendActiveSensing(MidiInterface::Type interface, uint8_t port);
    static void sendSystemReset(MidiInterface::Type interface, uint8_t port);
    static void sendSongSelect(MidiInterface::Type interface,
                               uint8_t port,
                               uint8_t song);
    static void sendSongPosition(MidiInterface::Type interface,
                                 uint8_t port,
                                 uint16_t beats);
    static void sendSysEx(MidiInterface::Type interface,
                          uint8_t port,
                          std::vector<uint8_t> data);
    static void sendNrpn(MidiInterface::Type interface,
                         uint8_t port,
                         uint8_t channel,
                         uint16_t parameterNumber,
                         uint16_t midiValue,
                         bool lsbFirst);
    static void sendRpn(MidiInterface::Type interface,
                        uint8_t port,
                        uint8_t channel,
                        uint16_t parameterNumber,
                        uint16_t midiValue);
    static void sendControlChange14Bit(MidiInterface::Type interface,
                                       uint8_t port,
                                       uint8_t channel,
                                       uint16_t parameterNumber,
                                       uint16_t midiValue,
                                       bool lsbFirst);

    static void enableThru(MidiInterface::Type interface,
                           uint8_t port,
                           bool shouldBeEnabled);

    static constexpr uint16_t queueSize = 64;
    static CircularBuffer<MidiMessageTransport, queueSize> outgoingQueue;

private:
    static void indicate(MidiInterface::Type interface,
                         uint8_t port,
                         Direction direction,
                         MidiMessage::Type msgType);
    uint8_t channel;
    uint16_t rate;
    uint32_t tsLastMessage;
};
