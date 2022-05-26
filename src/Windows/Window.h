#pragma once

#include <vector>
#include "TouchEvent.h"
#include "Component.h"

class Window : public Component
{
public:
    Window();
    Window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    virtual ~Window();

    // LCD touch management
    virtual std::vector<TouchEvent> processTouch(const TouchPoint &touchPoint);
    void resetActiveTouch(void);
    void setActiveComponent(Component *component, uint8_t touchId);
    void resetActiveComponent(uint8_t touchId);
    Component *getActiveComponent(uint8_t touchId);

    // Paint releated methods
    void paint(Graphics &g) override;
    void setVisible(bool shouldBeVisible) override;
    void display(void);

    // Event processing
    void setActive(bool shouldBeActive);
    bool getActive(void) const;

    void addChildComponent(Component *component) override;
    void addAndMakeVisible(Component *component) override;

    void setOwnedContent(Component *newComponent);
    Component *getOwnedContent(void);

private:
    static Component *checkHit(Component *component, uint16_t x, uint16_t y);

    static const uint8_t numActiveComponents = 5;

    Component *activeComponent[numActiveComponents];

protected:
    Component *contentComponent;
};
