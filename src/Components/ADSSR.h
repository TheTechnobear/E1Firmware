#pragma once

#include "Envelope.h"

class ADSSR : public Envelope
{
public:
    ADSSR()
    {
        points.push_back(Point()); // Attack
        points.push_back(Point()); // Decay
        points.push_back(Point()); // Break / Slope
        points.push_back(Point()); // Sustain
        points.push_back(Point()); // Release

        values.push_back(Value()); // Attack
        values.push_back(Value()); // Decay
        values.push_back(Value()); // Break point
        values.push_back(Value()); // Slope
        values.push_back(Value()); // Sustain
        values.push_back(Value()); // Release
    }

    virtual ~ADSSR() = default;

    void computePoints(void)
    {
        uint16_t segmentWidth = getSegmentWidth(5);
        float maxY = getHeight() - 1;

        // Find the lowest point of the envelope
        float lowestY =
            std::min(values[breakPoint].value, values[sustain].value);

        // Set the baseline
        baselineY =
            map(0.0f, std::min(0.0f, lowestY), values[sustain].max, maxY, 0.0f);

        int16_t sustainLevel = map(values[sustain].value,
                                   std::min(0.0f, values[sustain].value),
                                   values[sustain].max,
                                   maxY,
                                   0.0f);

        int16_t breakLevel = map(values[breakPoint].value,
                                 std::min(0.0f, values[breakPoint].value),
                                 values[breakPoint].max,
                                 maxY,
                                 0.0f);

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Delay
        points[1].x = segmentWidth * values[delay].value;
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].value;
        points[2].y = 0;

        // Decay
        points[3].x = points[2].x + segmentWidth * values[decay].value;
        points[3].y = breakLevel;

        // Slope
        points[4].x = points[3].x + segmentWidth * values[slope].value;
        points[4].y = sustainLevel;

        // Sustain
        points[5].x = points[4].x + segmentWidth;
        points[5].y = points[4].y;

        // Release
        points[6].x = points[5].x + segmentWidth * values[release].value;
        points[6].y = baselineY;
    }

    static constexpr uint8_t attack = 1;
    static constexpr uint8_t decay = 2;
    static constexpr uint8_t breakPoint = 3;
    static constexpr uint8_t slope = 4;
    static constexpr uint8_t sustain = 5;
    static constexpr uint8_t release = 6;

private:
};
