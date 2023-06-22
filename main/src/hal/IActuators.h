/*
 * IActuators.h
 *
 *  Created on: 12.06.2023
 *      Author: Maik
 */
#pragma once
#include <memory>
#include <mutex>
#include <thread>

#include "configuration/Configuration.h"
#include "events/EventManager.h"
#include "events/events.h"
#include "logger/logger.hpp"

// ENum value to attach to event data for controlling lamps
enum LampState { OFF, ON, FLASHING_SLOW, FLASHING_FAST };

class IActuators : public IEventHandler {
  public:
    virtual ~IActuators() {}

    /**
     * Sets all actuators when Standby mode is entered:
     * - all off
     */
    virtual void standbyMode() = 0;

    /**
     * Sets all actuators when Running mode is entered:
     * - green lamp on
     */
    virtual void runningMode() = 0;

    /**
     * Sets all actuators when Service mode is entered
     * - green lamp flashing
     */
    virtual void serviceMode() = 0;
    /**
     * Sets all actuators when Error mode is entered
     * - red lamp flashing fast
     * - motor stop
     */
    virtual void errorMode() = 0;
    /**
     * Sets all actuators when EStop mode is entered:
     * - all off
     */
    virtual void estopMode() = 0;

    /**
     * Turns the green lamp on
     */
    virtual void greenLampOn() = 0;

    /**
     * Lets the green lamp blink or turns it off
     *
     * @param on Blinking on or off
     */
    virtual void setGreenBlinking(bool on) = 0;

    /**
     * Turns the green lamp off
     */
    virtual void greenLampOff() = 0;

    /**
     * Turns the yellow lamp on
     */
    virtual void yellowLampOn() = 0;

    /**
     * Lets the yellow lamp blink or turns it off
     *
     * @param on Blinking on or off
     */
    virtual void setYellowBlinking(bool on) = 0;

    /**
     * Turns the yellow lamp off
     */
    virtual void yellowLampOff() = 0;

    /**
     * Turns the red lamp on
     */
    virtual void redLampOn() = 0;

    /**
     * Lets the red lamp blink or turns it off
     *
     * @param on Blinking on or off
     * @param fast Blink fast(1Hz) or slow (0,5Hz)
     */
    virtual void setRedBlinking(bool on, bool fast) = 0;

    /**
     * Turns the red lamp off
     */
    virtual void redLampOff() = 0;

    /**
     * Turns the LED at the 'Start' button on
     */
    virtual void startLedOn() = 0;

    /**
     * Turns the LED at the 'Start' button off
     */
    virtual void startLedOff() = 0;

    /**
     * Turns the LED at the 'Reset' button on
     */
    virtual void resetLedOn() = 0;

    /**
     * Turns the LED at the 'Reset' button off
     */
    virtual void resetLedOff() = 0;

    /**
     * Turns the 'Q1' LED on
     */
    virtual void q1LedOn() = 0;

    /**
     * Turns the 'Q1' LED off
     */
    virtual void q1LedOff() = 0;

    /**
     * Turns the 'Q2' LED on
     */
    virtual void q2LedOn() = 0;

    /**
     * Turns the 'Q2' LED off
     */
    virtual void q2LedOff() = 0;

    /**
     * Stops the motor
     */
    virtual void motorStop() = 0;

    /**
     * Lets the motor run in slow speed
     */
    virtual void motorSlow() = 0;

    /**
     * Lets the motor run in fast speed
     */
    virtual void motorFast() = 0;

    /**
     * Opens the switch to let a workpiece pass.
     */
    virtual void openSwitch() = 0;

    /**
     * Closes the switch to let a workpiece fall into the slide.
     */
    virtual void closeSwitch() = 0;

    /**
     * Lets the switch or pusher sort out workpiece to ramp.
     * If switch is mounted: Do nothing, so workpiece will go to ramp
     * If pusher is mounted: activate for {ON_TIME_PUSHER_MS} ms, then
     * deactivate again
     */
    virtual void sortOut() = 0;

    /**
     * Lets the switch or pusher pass a workpiece to the end of the belt.
     * If switch is mounted: Open for {ON_TIME_SWITCH_MS} ms, then close again
     * If pusher is mounted: Do nothing, so workpiece will pass.
     */
    virtual void letPass() = 0;

    virtual void allOff() = 0;

    void handleEvent(Event event) override {
        std::lock_guard<std::mutex> lock(mutex);
        Logger::debug("[Actuators] handleEvent: " + EVENT_TO_STRING(event.type));

        bool handled = true;

        switch (event.type) {
        case EventType::ESTOP_M_PRESSED:
        case EventType::ESTOP_S_PRESSED:
            allOff();
            break;
        case EventType::ERROR_M_SELF_SOLVABLE:
        case EventType::ERROR_S_SELF_SOLVABLE:
        case EventType::ERROR_M_MAN_SOLVABLE:
        case EventType::ERROR_S_MAN_SOLVABLE:
        	errorMode();
        	break;
        case EventType::MOTOR_M_STOP:
        case EventType::MOTOR_S_STOP:
            motorStop();
            break;
        case EventType::MOTOR_M_FAST:
        case EventType::MOTOR_S_FAST:
            motorFast();
            break;
        case EventType::MOTOR_M_SLOW:
        case EventType::MOTOR_S_SLOW:
            motorSlow();
            break;
        case EventType::MODE_STANDBY:
            standbyMode();
            break;
        case EventType::MODE_RUNNING:
            runningMode();
            break;
        case EventType::MODE_SERVICE:
            serviceMode();
            break;
        case EventType::MODE_ESTOP:
            estopMode();
            break;
        case EventType::MODE_ERROR:
            errorMode();
            break;
        case EventType::LAMP_M_GREEN:
        case EventType::LAMP_S_GREEN:
        case EventType::LAMP_M_YELLOW:
        case EventType::LAMP_S_YELLOW:
        case EventType::LAMP_M_RED:
        case EventType::LAMP_S_RED:
        case EventType::LED_M_START:
        case EventType::LED_S_START:
        case EventType::LED_M_RESET:
        case EventType::LED_S_RESET:
        case EventType::LED_M_Q1:
        case EventType::LED_S_Q1:
        case EventType::LED_M_Q2:
        case EventType::LED_S_Q2:
            handled = handleLampEvent(event.type, (LampState) event.data);
            break;
        case EventType::SORT_M_OUT:
        case EventType::SORT_S_OUT:
            event.data == 1 ? sortOut() : letPass();
            break;
        default:
            handled = false;
        }

        if (!handled) {
            Logger::warn(EVENT_TO_STRING(event.type) +
                         " was not handled by actuators");
        } else {
            Logger::debug("[Actuators] Event was handled: " +
                          EVENT_TO_STRING(event.type));
        }
    }

    bool handleLampEvent(EventType event, LampState state) {
        bool handled = true;

        switch (event) {
        case LAMP_M_GREEN:
        case LAMP_S_GREEN:
            setGreenBlinking(false);
            if (state == LampState::OFF)
                greenLampOff();
            else if (state == LampState::ON)
                greenLampOn();
            else if (state == LampState::FLASHING_SLOW)
                setGreenBlinking(true);
            else
                handled = false;
            break;
        case LAMP_M_YELLOW:
        case LAMP_S_YELLOW:
            setYellowBlinking(false);
            if (state == LampState::OFF)
                yellowLampOff();
            else if (state == LampState::ON)
                yellowLampOn();
            else if (state == LampState::FLASHING_SLOW)
                setYellowBlinking(true);
            else
                handled = false;
            break;
        case LAMP_M_RED:
        case LAMP_S_RED:
            setRedBlinking(false, false);
            if (state == LampState::OFF)
                redLampOff();
            else if (state == LampState::ON)
                redLampOn();
            else if (state == LampState::FLASHING_SLOW)
                setRedBlinking(true, false);
            else if (state == LampState::FLASHING_FAST)
                setRedBlinking(true, true);
            else
                handled = false;
            break;
        case LED_M_START:
        case LED_S_START:
            state == LampState::ON ? startLedOn() : startLedOff();
            break;
        case LED_M_RESET:
        case LED_S_RESET:
            state == LampState::ON ? resetLedOn() : resetLedOff();
            break;
        case LED_M_Q1:
        case LED_S_Q1:
            state == LampState::ON ? q1LedOn() : q1LedOff();
            break;
        case LED_M_Q2:
        case LED_S_Q2:
            state == LampState::ON ? q2LedOn() : q2LedOff();
            break;
        default:
            handled = false;
        }

        return handled;
    }

  protected:
    IActuators(std::shared_ptr<EventManager> mngr) : eventManager(mngr) {
        isMaster = Configuration::getInstance().systemIsMaster();
        subscribeToEvents();
    }

    void subscribeToEvents() {
        // Subscribe to lamp events

        // Subscribe to modes
        eventManager->subscribe(
            EventType::MODE_STANDBY,
            std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
        eventManager->subscribe(
            EventType::MODE_RUNNING,
            std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
        eventManager->subscribe(
            EventType::MODE_SERVICE,
            std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
        eventManager->subscribe(
            EventType::MODE_ESTOP,
            std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
        eventManager->subscribe(
            EventType::MODE_ERROR,
            std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
        eventManager->subscribe(
			EventType::ESTOP_M_PRESSED,
			std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
        eventManager->subscribe(
			EventType::ESTOP_S_PRESSED,
			std::bind(&IActuators::handleEvent, this, std::placeholders::_1));


        // System-dependent events
        if (isMaster) {
            eventManager->subscribe(
    			EventType::ERROR_M_MAN_SOLVABLE,
    			std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
            eventManager->subscribe(
    			EventType::ERROR_M_SELF_SOLVABLE,
    			std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
            eventManager->subscribe(EventType::LAMP_M_RED,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LAMP_M_YELLOW,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LAMP_M_GREEN,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_M_START,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_M_RESET,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_M_Q1,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_M_Q2,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::MOTOR_M_STOP,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::MOTOR_M_FAST,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::MOTOR_M_SLOW,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::SORT_M_OUT,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
        } else {
            eventManager->subscribe(
    			EventType::ERROR_S_MAN_SOLVABLE,
    			std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
            eventManager->subscribe(
    			EventType::ERROR_S_SELF_SOLVABLE,
    			std::bind(&IActuators::handleEvent, this, std::placeholders::_1));
            eventManager->subscribe(EventType::LAMP_S_RED,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LAMP_S_YELLOW,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LAMP_S_GREEN,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_S_START,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_S_RESET,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_S_Q1,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::LED_S_Q2,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::MOTOR_S_STOP,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::MOTOR_S_FAST,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::MOTOR_S_SLOW,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
            eventManager->subscribe(EventType::SORT_S_OUT,
                                    std::bind(&IActuators::handleEvent, this,
                                              std::placeholders::_1));
        }
    }

  private:
    std::shared_ptr<EventManager> eventManager;
    std::mutex mutex;
    bool isMaster;
};
