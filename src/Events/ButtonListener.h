#pragma once

#include <cstdint>
#include "ButtonBroadcaster.h"

/**
 * @brief provides callbacks for handling hardware button events.
 */
class ButtonListener
{
public:
    /**
     * Constructor.
     */
    ButtonListener() = default;

    /**
     * Destructor.
     */
    virtual ~ButtonListener() = default;

    /**
     * Called when the button is pressed.
     *
     * @param buttonId an identifier of the button.
     */
    virtual void onButtonDown(uint8_t buttonId)
    {
    }

    /**
     * Called when the button is released.
     *
     * @param buttonId an identifier of the button.
     */
    virtual void onButtonUp(uint8_t buttonId)
    {
    }

    /**
     * Called when the button is held down for longer time period.
     *
     * @param buttonId an identifier of the button.
     */
    virtual void onButtonLongHold(uint8_t buttonId)
    {
    }

    /**
     * Assigns the button to the listener object.
     *
     * @param an identifier of the button. It is a number in range 0 to 5, or
     *        AllButtons constant for catching events from all buttons.
     */
    void assignButton(uint8_t newButtonId = AllButtons)
    {
        buttonId = newButtonId;
        buttonBroadcaster.addListener(this);
    }

    /**
     * Assigns all buttons to the listener object.
     */
    void assignAllButtons(void)
    {
        buttonId = AllButtons;
        buttonBroadcaster.addListener(this);
    }

    /**
     * Stops listening for assigned buttons.
     */
    void releaseButton(void)
    {
        buttonBroadcaster.removeListener(this);
    }

    static constexpr uint8_t AllButtons = 255;
    static ButtonBroadcaster buttonBroadcaster;
    uint8_t buttonId;
};
