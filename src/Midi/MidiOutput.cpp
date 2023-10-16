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

#include "MidiOutput.h"
#include "ArduinoJson.h"
#include "App.h"

/** Constructor
 *
 */
MidiOutput::MidiOutput(MidiInterface::Type newInterface, uint8_t newPort)
    : MidiJack(newInterface, newPort), channel(0), rate(0), tsLastMessage(0)
{
}

MidiOutput::MidiOutput(MidiInterface::Type newInterface,
                       uint8_t newPort,
                       uint8_t newChannel,
                       uint16_t newRate)
    : MidiJack(newInterface, newPort),
      channel(newChannel),
      rate(0),
      tsLastMessage(0)
{
}

void MidiOutput::sendSetLogger(MidiInterface::Type interface,
                               uint8_t port,
                               bool status)
{
    uint8_t data[9] = { 0xF0,   0x00,   0x21, 0x45, SYSTEM_CALL,
                        LOGGER, status, 0x00, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendPresetSlotChanged(MidiInterface::Type interface,
                                       uint8_t port)
{
    uint8_t data[7] = { 0xF0, 0x00,          0x21,
                        0x45, EVENT_MESSAGE, PRESET_LIST_CHANGE,
                        0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendSnapshotBankChanged(MidiInterface::Type interface,
                                         uint8_t port,
                                         uint8_t bankNumber)
{
    uint8_t data[8] = { 0xF0,       0x00,          0x21,
                        0x45,       EVENT_MESSAGE, SNAPSHOT_BANK_SWITCH,
                        bankNumber, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendSnapshotChanged(MidiInterface::Type interface,
                                     uint8_t port)
{
    uint8_t data[7] = { 0xF0, 0x00, 0x21, 0x45, EVENT_MESSAGE, SNAPSHOT_CHANGE,
                        0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendPresetSwitched(MidiInterface::Type interface,
                                    uint8_t port,
                                    uint8_t bankNumber,
                                    uint8_t slotId)
{
    uint8_t data[9] = { 0xF0,          0x00,       0x21,   0x45, EVENT_MESSAGE,
                        PRESET_SWITCH, bankNumber, slotId, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendPageSwitched(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint8_t pageNumber)
{
    uint8_t data[8] = { 0xF0,          0x00,        0x21,       0x45,
                        EVENT_MESSAGE, PAGE_SWITCH, pageNumber, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendPresetBankSwitched(MidiInterface::Type interface,
                                        uint8_t port,
                                        uint8_t bankNumber)
{
    uint8_t data[8] = { 0xF0,       0x00,          0x21,
                        0x45,       EVENT_MESSAGE, PRESET_BANK_SWITCH,
                        bankNumber, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendControlSetSwitched(MidiInterface::Type interface,
                                        uint8_t port,
                                        uint8_t controlSetId)
{
    uint8_t data[8] = { 0xF0,         0x00,          0x21,
                        0x45,         EVENT_MESSAGE, CONTROL_SET_SWITCH,
                        controlSetId, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendPotTouchEvent(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t potId,
                                   uint16_t controlId,
                                   bool touched)
{
    uint8_t data[11] = { 0xF0,
                         0x00,
                         0x21,
                         0x45,
                         EVENT_MESSAGE,
                         POT_TOUCH,
                         potId,
                         (uint8_t)(controlId & 0x7f),
                         (uint8_t)((controlId >> 7) & 0x7f),
                         touched,
                         0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendUsbHostChanged(MidiInterface::Type interface, uint8_t port)
{
    uint8_t data[7] = { 0xF0, 0x00, 0x21, 0x45, EVENT_MESSAGE, USB_HOST_CHANGE,
                        0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendAck(MidiInterface::Type interface, uint8_t port)
{
    uint8_t data[9] = { 0xF0, 0x00, 0x21, 0x45, EVENT_MESSAGE,
                        ACK,  0x00, 0x00, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendNack(MidiInterface::Type interface, uint8_t port)
{
    uint8_t data[9] = { 0xF0, 0x00, 0x21, 0x45, EVENT_MESSAGE,
                        NACK, 0x00, 0x00, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendAvailable(MidiInterface::Type interface, uint8_t port)
{
    uint8_t data[9] = { 0xF0,      0x00, 0x21, 0x45, EVENT_MESSAGE,
                        AVAILABLE, 0x00, 0x00, 0xF7 };

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, sizeof(data));
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendAppInfo(MidiInterface::Type interface, uint8_t port)
{
    uint8_t data[256];
    StaticJsonDocument<256> doc;

    data[0] = 0xF0;
    data[1] = 0x00;
    data[2] = 0x21;
    data[3] = 0x45;
    data[4] = FILE_UPLOAD;
    data[5] = APP_INFO;

    char *jsonStart = (char *)&data[6];
    doc["app"] = App::get()->getApplicationName();
    if (System::context.getPresetName()) {
        doc["preset"] = System::context.getPresetName();
    }
    serializeJson(doc, jsonStart, 250);
    size_t jsonSize = strlen(jsonStart);
    data[jsonSize + 6] = SYSEX_END;

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, jsonSize + 7);
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendRuntimeInfo(MidiInterface::Type interface, uint8_t port)
{
    uint8_t data[128];
    StaticJsonDocument<128> doc;

    data[0] = 0xF0;
    data[1] = 0x00;
    data[2] = 0x21;
    data[3] = 0x45;
    data[4] = FILE_UPLOAD;
    data[5] = MEMORY_INFO;

    char *jsonStart = (char *)&data[6];
    doc["freePercentage"] = Hardware::ram.getFreeRam();
    serializeJson(doc, jsonStart, 120);
    size_t jsonSize = strlen(jsonStart);
    data[jsonSize + 6] = SYSEX_END;

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, jsonSize + 7);
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendElectraInfo(MidiInterface::Type interface,
                                 uint8_t port,
                                 const char *electraInfoSerial,
                                 uint8_t electraInfoHwRevision)
{
    uint8_t data[128];
    char buf[16];
    StaticJsonDocument<128> doc;

    sprintf(buf,
            "%d.%02d",
            electraInfoHwRevision / 100,
            electraInfoHwRevision - 200);

    data[0] = 0xF0;
    data[1] = 0x00;
    data[2] = 0x21;
    data[3] = 0x45;
    data[4] = FILE_UPLOAD;
    data[5] = ELECTRA_INFO;

    char *jsonStart = (char *)&data[6];
    doc["versionText"] = FIRMWARE_VERSION;
    doc["versionSeq"] = FIRMWARE_SEQ;
    doc["serial"] = electraInfoSerial;
    doc["hwRevision"] = buf;
    serializeJson(doc, jsonStart, 120);
    size_t jsonSize = strlen(jsonStart);
    data[jsonSize + 6] = SYSEX_END;

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, jsonSize + 7);
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
}

void MidiOutput::sendMidiLearn(MidiInterface::Type interface,
                               uint8_t portToTransmitOn,
                               const char *msg,
                               uint8_t port,
                               uint8_t channel,
                               uint16_t parameterId,
                               uint16_t value)
{
    uint8_t data[512];
    StaticJsonDocument<512> doc;

#ifdef DEBUG
    System::logger.write(
        ERROR,
        "sendMidiLearn: msg=%s, port=%d, channel=%d, parameter=%d, value=%d",
        msg,
        port,
        channel,
        parameterId,
        value);
#endif

    data[0] = 0xF0;
    data[1] = 0x00;
    data[2] = 0x21;
    data[3] = 0x45;
    data[4] = MIDILEARN_SWITCH;

    char *jsonStart = (char *)&data[5];

    doc["msg"] = msg;
    doc["port"] = port;
    doc["channel"] = channel;
    doc["parameterId"] = parameterId;
    doc["value"] = value;
    serializeJson(doc, jsonStart, 504);
    size_t jsonSize = strlen(jsonStart);
    data[jsonSize + 5] = SYSEX_END;

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());
    sysexBlock.writeBytes(data, jsonSize + 6);
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, portToTransmitOn, message);
}

void MidiOutput::sendMidiLearnSysex(MidiInterface::Type interface,
                                    uint8_t portToTransmitOn,
                                    uint8_t port,
                                    const SysexBlock &capturedSysexBlock)
{
    size_t sysExLength = capturedSysexBlock.getLength();
    char writeBuffer[64];

    SysexBlock sysexBlock = SysexBlock(App::get()->sysexPool.openMemoryBlock());

    writeBuffer[0] = 0xF0;
    writeBuffer[1] = 0x00;
    writeBuffer[2] = 0x21;
    writeBuffer[3] = 0x45;
    writeBuffer[4] = MIDILEARN_SWITCH;
    sysexBlock.writeBytes((const uint8_t *)writeBuffer, 5);

    snprintf(writeBuffer,
             sizeof(writeBuffer),
             "{\"msg\":\"sysex\", \"port\":%d,\"data\":[",
             port);
    sysexBlock.writeBytes((const uint8_t *)writeBuffer, strlen(writeBuffer));

    for (size_t i = 0; i < sysExLength; i++) {
        snprintf(writeBuffer,
                 sizeof(writeBuffer),
                 "\"%02X\"%c",
                 capturedSysexBlock.peek(i),
                 (i == (sysExLength - 1)) ? ' ' : ',');
        sysexBlock.writeBytes((const uint8_t *)writeBuffer,
                              strlen(writeBuffer));
    }

    writeBuffer[0] = ']';
    writeBuffer[1] = '}';
    writeBuffer[2] = 0xF7;

    sysexBlock.writeBytes((const uint8_t *)writeBuffer, 3);
    sysexBlock.close();

    MidiMessage message(sysexBlock);
    sendMessage(interface, portToTransmitOn, message);
}

extern uint8_t stopFlush;

bool MidiOutput::sendSysExFile(uint8_t port,
                               const char *filename,
                               ElectraCommand::Object fileType)
{
    FileIoStream file;
    uint32_t i = 0;
    uint32_t packet = 0;
    uint8_t byteInPacket = 0;
    uint8_t data[4];

    if (!(file = Hardware::sdcard.createInputStream(filename))) {
        System::logger.write(
            LOG_ERROR,
            "sendSysexFile: the file does not exists: filename=%s",
            filename);
        return (false);
    }

    stopFlush = 1;
    MidiOutput::sendSysExHeader(port);

    data[0] = FILE_UPLOAD;
    data[1] = (uint8_t)fileType;
    packet = 2;

    while (file.available()) {
        byteInPacket = packet % 4;
        data[byteInPacket] = file.read();

        if (byteInPacket == 3) {
            MidiOutput::sendSysExData(data, 4, port);
        }

        i++;
        packet++;

        if ((i % 20) == 0) {
            delayMicroseconds(50);
        }
    }

    file.close();

    /*
     * send last packet correctly
     */
    byteInPacket = packet % 4;
    data[byteInPacket] = SYSEX_END;
    MidiOutput::sendSysExData(data, byteInPacket + 1, port);

    stopFlush = 0;

    return (true);
}

/*
 * send USB MIDI packets
 */
void MidiOutput::sendSysExData(const uint8_t *data,
                               uint32_t length,
                               uint8_t port)
{
    uint8_t cable = (port & 0x0F) << 4;

    while (length > 3) {
        usb_midi_write_packed(0x04 | cable | (data[0] << 8) | (data[1] << 16)
                              | (data[2] << 24));
        data += 3;
        length -= 3;
    }
    if (length == 3) {
        usb_midi_write_packed(0x07 | cable | (data[0] << 8) | (data[1] << 16)
                              | (data[2] << 24));
    } else if (length == 2) {
        usb_midi_write_packed(0x06 | cable | (data[0] << 8) | (data[1] << 16));
    } else if (length == 1) {
        usb_midi_write_packed(0x05 | cable | (data[0] << 8));
    }
}

void MidiOutput::sendSysExHeader(uint8_t port)
{
    uint8_t data[4];
    uint32_t manufacturerSysexId = SYSEX_MANUFACTURER;

    data[0] = SYSEX_START;
    data[1] = (manufacturerSysexId >> 16) & 0xFF;
    data[2] = (manufacturerSysexId >> 8) & 0xFF;
    data[3] = manufacturerSysexId & 0xFF;

    MidiOutput::sendSysExData(data, 4, port);
}

void MidiOutput::setPort(uint8_t newPort)
{
    port = newPort;
}

uint8_t MidiOutput::getPort(void) const
{
    return (port);
}

void MidiOutput::setChannel(uint8_t newChannel)
{
    channel = newChannel;
}

uint8_t MidiOutput::getChannel(void) const
{
    return (channel);
}

void MidiOutput::setRate(uint16_t newRate)
{
    rate = newRate;
}

uint16_t MidiOutput::getRate(void) const
{
    return (rate);
}

void MidiOutput::setTsLastMessage(void)
{
    tsLastMessage = millis();
}

bool MidiOutput::isReady(void)
{
    return (((millis() - tsLastMessage) > rate) ? true : false);
}

/** Send Electra message to Midi outputs.
 *
 */
void MidiOutput::sendMessage(MidiInterface::Type interface,
                             uint8_t port,
                             MidiMessage &message)
{
    addToQueue(interface, port, message);
}

bool MidiOutput::isIdenticalChange(const MidiMessageTransport &m1,
                                   const MidiMessageTransport &m2)
{
    if (m1.getType() == MidiMessage::Type::ControlChange) {
        // This is to prevent issue of the rpn reset
        if ((m1.getInterfaceType() == m2.getInterfaceType())
            && (m1.getPort() == m2.getPort()) && (m1.getType() == m2.getType())
            && (m1.getChannel() == m2.getChannel())
            && ((m1.getData1() == 100) || (m1.getData1() == 101))) {
            return false;
        } else if ((m1.getInterfaceType() == m2.getInterfaceType())
                   && (m1.getPort() == m2.getPort())
                   && (m1.getType() == m2.getType())
                   && (m1.getChannel() == m2.getChannel())
                   && (m1.getData1() == m2.getData1())) {
            return true;
        }
    } else if (m1.getType() == MidiMessage::Type::ProgramChange) {
        if ((m1.getInterfaceType() == m2.getInterfaceType())
            && (m1.getPort() == m2.getPort()) && (m1.getType() == m2.getType())
            && (m1.getChannel() == m2.getChannel())) {
            return true;
        }
    } else if (m1.getType() == MidiMessage::Type::SystemExclusive) {
        if ((m1.getInterfaceType() == m2.getInterfaceType())
            && (m1.getPort() == m2.getPort())
            && (m1.getType() == m2.getType())) {
            return true;
        }
    }
    return (false);
}

/** Adds the message to the queue if it is not registered there already
 *
 */
void MidiOutput::addToQueue(MidiInterface::Type interface,
                            uint8_t port,
                            MidiMessage &message)
{
    MidiMessageTransport mmt(interface, port, message);
    /*
    for (int i = 0; i < outgoingQueue.size(); i++) {
        if (MidiOutput::isIdenticalChange(outgoingQueue[i], mmt)) {
            outgoingQueue.getObjectPointer(i)->invalid = true;
        }
    }
*/
    outgoingQueue.push(mmt);
}

/** Send Electra message to Midi outputs.
 *
 */
void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      MidiMessage::Type type,
                      uint8_t channel,
                      uint8_t data1,
                      uint8_t data2)
{
    MidiInterface::get(interface)->send(port, type, channel, data1, data2);
    indicate(interface, port, Direction::out, type);
}

void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      MidiMessage &message)
{
    if (message.getType() == MidiMessage::Type::SystemExclusive) {
        MidiInterface::get(interface)->send(port, message.getSysExBlock());
    } else {
        MidiInterface::get(interface)->send(port,
                                            message.getType(),
                                            message.getChannel(),
                                            message.getData1(),
                                            message.getData2());
    }
    indicate(interface, port, Direction::out, message.getType());
}

void MidiOutput::send(MidiInterface::Type interface,
                      uint8_t port,
                      SysexBlock &sysexBlock)
{
    MidiInterface::get(interface)->sendSysEx(port, sysexBlock);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendControlChange(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t parameterNumber,
                                   uint8_t value)
{
    MidiMessage message(
        channel, MidiMessage::Type::ControlChange, parameterNumber, value);
    sendMessage(interface, port, message);
    indicate(interface, port, Direction::out, MidiMessage::Type::ControlChange);
}

void MidiOutput::sendNoteOn(MidiInterface::Type interface,
                            uint8_t port,
                            uint8_t channel,
                            uint8_t noteNumber,
                            uint8_t velocity)
{
    MidiInterface::get(interface)->sendNoteOn(
        port, channel, noteNumber, velocity);
    indicate(interface, port, Direction::out, MidiMessage::Type::NoteOn);
}

void MidiOutput::sendNoteOff(MidiInterface::Type interface,
                             uint8_t port,
                             uint8_t channel,
                             uint8_t noteNumber,
                             uint8_t velocity)
{
    MidiInterface::get(interface)->sendNoteOff(
        port, channel, noteNumber, velocity);
    indicate(interface, port, Direction::out, MidiMessage::Type::NoteOff);
}

void MidiOutput::sendStart(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendStart(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Start);
}

void MidiOutput::sendStop(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendStop(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Stop);
}

void MidiOutput::sendTuneRequest(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendTuneRequest(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::TuneRequest);
}

void MidiOutput::sendProgramChange(MidiInterface::Type interface,
                                   uint8_t port,
                                   uint8_t channel,
                                   uint8_t programNumber)
{
    MidiMessage message(
        channel, MidiMessage::Type::ProgramChange, programNumber, 0);
    sendMessage(interface, port, message);
    indicate(interface, port, Direction::out, MidiMessage::Type::ProgramChange);
}

void MidiOutput::sendSysEx(MidiInterface::Type interface,
                           uint8_t port,
                           SysexBlock &sysexBlock)
{
    MidiMessage message(sysexBlock);
    sendMessage(interface, port, message);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendSysEx(MidiInterface::Type interface,
                           uint8_t port,
                           uint8_t *data,
                           uint16_t dataLength)
{
    MidiInterface::get(interface)->sendSysEx(port, data, dataLength);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendSysExPartial(MidiInterface::Type interface,
                                  uint8_t port,
                                  const uint8_t *data,
                                  uint16_t dataLength,
                                  bool complete)
{
    MidiInterface::get(interface)->sendSysExPartial(
        port, data, dataLength, complete);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendPitchBend(MidiInterface::Type interface,
                               uint8_t port,
                               uint8_t channel,
                               uint16_t value)
{
    MidiInterface::get(interface)->sendPitchBend(port, channel, value);
    indicate(interface, port, Direction::out, MidiMessage::Type::PitchBend);
}

void MidiOutput::sendAfterTouchPoly(MidiInterface::Type interface,
                                    uint8_t port,
                                    uint8_t channel,
                                    uint8_t noteNumber,
                                    uint8_t pressure)
{
    MidiInterface::get(interface)->sendAfterTouchPoly(
        port, channel, noteNumber, pressure);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::AfterTouchPoly);
}

void MidiOutput::sendAfterTouchChannel(MidiInterface::Type interface,
                                       uint8_t port,
                                       uint8_t channel,
                                       uint8_t pressure)
{
    MidiInterface::get(interface)->sendAfterTouchChannel(
        port, channel, pressure);
    indicate(
        interface, port, Direction::out, MidiMessage::Type::AfterTouchChannel);
}

void MidiOutput::sendClock(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendClock(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Clock);
}

void MidiOutput::sendContinue(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendContinue(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::Continue);
}

void MidiOutput::sendActiveSensing(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendActiveSensing(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::ActiveSensing);
}

void MidiOutput::sendSystemReset(MidiInterface::Type interface, uint8_t port)
{
    MidiInterface::get(interface)->sendSystemReset(port);
    indicate(interface, port, Direction::out, MidiMessage::Type::SystemReset);
}

void MidiOutput::sendSongSelect(MidiInterface::Type interface,
                                uint8_t port,
                                uint8_t song)
{
    MidiInterface::get(interface)->sendSongSelect(port, song);
    indicate(interface, port, Direction::out, MidiMessage::Type::SongSelect);
}

void MidiOutput::sendSongPosition(MidiInterface::Type interface,
                                  uint8_t port,
                                  uint16_t beats)
{
    MidiInterface::get(interface)->sendSongPosition(port, beats);
    indicate(interface, port, Direction::out, MidiMessage::Type::SongPosition);
}

void MidiOutput::sendSysEx(MidiInterface::Type interface,
                           uint8_t port,
                           std::vector<uint8_t> data)
{
    sendSysEx(interface, port, &data[0], data.size());
    indicate(
        interface, port, Direction::out, MidiMessage::Type::SystemExclusive);
}

void MidiOutput::sendNrpn(MidiInterface::Type interface,
                          uint8_t port,
                          uint8_t channel,
                          uint16_t parameterNumber,
                          uint16_t midiValue,
                          bool lsbFirst,
                          bool resetRpn)
{
    sendControlChange(interface, port, channel, 99, parameterNumber >> 7);
    sendControlChange(interface, port, channel, 98, parameterNumber & 0x7F);

    if (lsbFirst) {
        sendControlChange(interface, port, channel, 38, midiValue >> 7);
        sendControlChange(interface, port, channel, 6, midiValue & 0x7F);
    } else {
        sendControlChange(interface, port, channel, 6, midiValue >> 7);
        sendControlChange(interface, port, channel, 38, midiValue & 0x7F);
    }

    if (resetRpn) {
        sendControlChange(interface, port, channel, 101, 127);
        sendControlChange(interface, port, channel, 100, 127);
    }
}

void MidiOutput::sendRpn(MidiInterface::Type interface,
                         uint8_t port,
                         uint8_t channel,
                         uint16_t parameterNumber,
                         uint16_t midiValue)
{
    System::logger.write(LOG_ERROR, "sending RPN: %d", parameterNumber);
    sendControlChange(interface, port, channel, 101, parameterNumber >> 7);
    sendControlChange(interface, port, channel, 100, parameterNumber & 0x7F);
    sendControlChange(interface, port, channel, 6, midiValue >> 7);
    sendControlChange(interface, port, channel, 38, midiValue & 0x7F);
    sendControlChange(interface, port, channel, 101, 127);
    sendControlChange(interface, port, channel, 100, 127);
}

void MidiOutput::sendControlChange14Bit(MidiInterface::Type interface,
                                        uint8_t port,
                                        uint8_t channel,
                                        uint16_t parameterNumber,
                                        uint16_t midiValue,
                                        bool lsbFirst)
{
    if (lsbFirst) {
        sendControlChange(
            interface, port, channel, parameterNumber + 32, midiValue >> 7);
        sendControlChange(
            interface, port, channel, parameterNumber, midiValue & 0x7F);
    } else {
        sendControlChange(
            interface, port, channel, parameterNumber, midiValue >> 7);
        sendControlChange(
            interface, port, channel, parameterNumber + 32, midiValue & 0x7F);
    }
}

void MidiOutput::enableThru(MidiInterface::Type interface,
                            uint8_t port,
                            bool shouldBeEnabled)
{
    if (interface == MidiInterface::Type::MidiIo) {
        MidiInterface::get(interface)->enableThru(port, shouldBeEnabled);
    }
    // other interfaces are not implmented on purpose
}

void MidiOutput::indicate(MidiInterface::Type interface,
                          uint8_t port,
                          Direction direction,
                          MidiMessage::Type msgType)
{
    App::get()->statusBar.indicate(interface, port, direction, msgType);
}

CircularBuffer<MidiMessageTransport, MidiOutput::queueSize>
    MidiOutput::outgoingQueue;
