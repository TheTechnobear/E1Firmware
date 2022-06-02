#include "TaskReadLCDTouch.h"
#include "Hardware.h"
#include "App.h"

/*
 * Read LCD capacitive touch
 */
void readLCDTouch(void)
{
    //logMessage ("readLCDTouch");

    static Window *previousWindow = System::windowManager.getActiveWindow();
    static bool waitingForStart = false;

    if (Hardware::touch.readTouch() == 0) {
        return;
    }

    Window *originatingWindow = System::windowManager.getActiveWindow();

    // Detect a window change. All events will be thrown away till next start
    // comes in.
    if (originatingWindow != previousWindow) {
        waitingForStart = true;
    }

    previousWindow = originatingWindow;

#ifdef DEBUG
    logMessage("received touch from active window: address=%x, window=%s",
               originatingWindow,
               originatingWindow->getName());
#endif

    while (originatingWindow && Hardware::touch.eventsAvailable()) {
        TouchPoint touchPoint = Hardware::touch.readEvents();

        if (waitingForStart && touchPoint.event != TouchPoint::Start) {
            continue;
        }

        if (touchPoint.event != TouchPoint::None) {
            if (touchPoint.event == TouchPoint::Start) {
                waitingForStart = false;
                if (!Component::isInside(touchPoint.xStart,
                                         touchPoint.yStart,
                                         originatingWindow->getX(),
                                         originatingWindow->getY(),
                                         originatingWindow->getWidth(),
                                         originatingWindow->getHeight())) {
                    originatingWindow->onTouchOutside();
                    return;
                }
            }

            std::vector<TouchEvent> touchEvents =
                originatingWindow->processTouch(touchPoint);

            for (auto &touchEvent : touchEvents) {
                Component *eventComponent = nullptr;
                uint8_t id = touchEvent.getId();

                if (originatingWindow->getActiveComponent(id)) {
                    eventComponent = originatingWindow->getActiveComponent(id);
                    touchEvent.setEventCompoment(eventComponent);
                } else {
                    eventComponent = touchEvent.getEventComponent();
                    originatingWindow->setActiveComponent(eventComponent, id);
                }

                triggerComponentCallbacks(eventComponent, touchEvent);

                if (originatingWindow
                    != System::windowManager.getActiveWindow()) {
                    break;
                }

                if (touchEvent.getEvent() == TouchPoint::End) {
                    originatingWindow->resetActiveComponent(id);
                }
            }
        }
    }
}

void triggerComponentCallbacks(Component *component,
                               const TouchEvent &touchEvent)
{
    TouchPoint::Event state = touchEvent.getEvent();

    for (Component *c = component; c; c = c->getParentComponent()) {
        if (state == TouchPoint::Start) {
            c->onTouchDown(touchEvent);
        } else if (state == TouchPoint::Move) {
            c->onTouchMove(touchEvent);
        } else if (state == TouchPoint::End) {
            c->onTouchUp(touchEvent);
        } else if (state == TouchPoint::LongHold) {
            c->onTouchLongHold(touchEvent);
        } else if (state == TouchPoint::Click) {
            c->onTouchClick(touchEvent);
        } else if (state == TouchPoint::DoubleClick) {
            c->onTouchDoubleClick(touchEvent);
        }
    }
}
