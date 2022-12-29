#pragma once

#include "Component.h"

class TextGraphics : virtual public Component
{
public:
    TextGraphics()
        : text(nullptr),
          style(TextStyle::smallWhiteOnBlack),
          aligment(TextAlign::left)
    {
    }

    TextGraphics(Rectangle bounds,
                 const char *newText,
                 TextStyle newStyle,
                 TextAlign newAligment,
                 uint16_t newColour = Colours565::white)
        : text(newText),
          style(newStyle),
          aligment(newAligment),
          colour(newColour)
    {
        setBounds(bounds);
    }

    ~TextGraphics()
    {
    }

    void paint(Graphics &g)
    {
        if ((text != nullptr) && (strlen(text) > 0)) {
            g.printText(
                0, getHeight() - 12, text, style, getWidth(), aligment, colour);
        }
    }

private:
    uint16_t colour;
    const char *text;
    TextStyle style;
    TextAlign aligment;
};
