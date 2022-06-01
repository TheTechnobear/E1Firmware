#pragma once

#include "Envelope.h"

class AHDSR : public Envelope
{
public:
    AHDSR()
    {
        points.push_back(Point()); // Attack
        points.push_back(Point()); // Hold
        points.push_back(Point()); // Decay
        points.push_back(Point()); // Sustain
        points.push_back(Point()); // Release

        values.push_back(Value()); // Attack
        values.push_back(Value()); // Hold
        values.push_back(Value()); // Decay
        values.push_back(Value()); // Sustain
        values.push_back(Value()); // Release
    }

    virtual ~AHDSR() = default;

    void computePoints(const Rectangle &bounds)
    {
        uint16_t segmentWidth = getSegmentWidth(bounds.getWidth(), 5);
        float maxY = bounds.getHeight() - 1;

        // Set the baseline
        baselineY = map(0.0f,
                        std::min(0.0f, values[sustain].get()),
                        values[sustain].getMax(),
                        maxY,
                        0.0f);

        int16_t sustainLevel = map(values[sustain].get(),
                                   std::min(0.0f, values[sustain].get()),
                                   values[sustain].getMax(),
                                   maxY,
                                   0.0f);

        // Starting point
        points[0].x = 0;
        points[0].y = baselineY;

        // Delay
        points[1].x = segmentWidth * 0; //values[delay].getRelative();
        points[1].y = baselineY;

        // Attack
        points[2].x = points[1].x + segmentWidth * values[attack].getRelative();
        points[2].y = 0;

        // Hold
        points[3].x = points[2].x + segmentWidth * values[hold].getRelative();
        points[3].y = 0;

        // Decay
        points[4].x = points[3].x + segmentWidth * values[decay].getRelative();
        points[4].y = sustainLevel;

        // Sustain
        points[5].x = points[4].x + segmentWidth;
        points[5].y = points[4].y;

        // Release
        points[6].x =
            points[5].x + segmentWidth * values[release].getRelative();
        points[6].y = baselineY;
    }

    static constexpr uint8_t attack = 0;
    static constexpr uint8_t hold = 1;
    static constexpr uint8_t decay = 2;
    static constexpr uint8_t sustain = 3;
    static constexpr uint8_t release = 4;

private:
};
