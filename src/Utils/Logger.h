#pragma once

#include <cstdint>

#define ERROR 0
#define WARNING 1
#define INFO 2
#define TRACE 3

/**
 * Logger class
 *
 * Logger class is used to send debugging messages to the USB host over the
 * USB MIDI protocol.
 */
class Logger
{
public:
    Logger();
    ~Logger() = default;

    void setPort(uint8_t newPort);
    void setLevel(uint8_t newLevel);
    void enable(void);
    void disable(void);
    void setStatus(bool newStatus);
    void write(uint8_t level, const char *format, ...);

private:
    static constexpr uint16_t MaxLogMessageSize = 200;
    uint8_t minimumLevel;
    uint8_t port;
    bool enabled;
};
