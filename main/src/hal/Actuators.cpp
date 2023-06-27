/*
 * Actuators.cpp
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */

#include "Actuators.h"

#include "configuration/Configuration.h"
#include "logger/logger.hpp"
#ifdef SIM_ACTIVE
#include "simqnxgpioapi.h"   // must be last include !!!
#include "simqnxirqapi.h"
#endif

Actuators::Actuators(std::shared_ptr<EventManager> mngr) : IActuators(mngr) {
    Configuration &conf = Configuration::getInstance();
    isMaster = conf.systemIsMaster();
    hasPusher = conf.pusherMounted();
    gpio_bank_1 = mmap_device_io(SIZE_4KB, (uint64_t) GPIO_BANK_1);
    gpio_bank_2 = mmap_device_io(SIZE_4KB, (uint64_t) GPIO_BANK_2);

    configurePins();

    greenBlinking = false;
    yellowBlinking = false;
    redBlinking = false;

    standbyMode();

	if(hasPusher) {
        Logger::debug("started with pusher");
		openSwitch();
	} else {
        Logger::debug("started with switch");
		closeSwitch();
	}
}

Actuators::~Actuators() {
    munmap_device_io(gpio_bank_1, SIZE_4KB);
    munmap_device_io(gpio_bank_2, SIZE_4KB);
}

void Actuators::configurePins() {
    ThreadCtl(_NTO_TCTL_IO, 0);

    // Request interrupt and IO abilities.
    int procmgr_status = procmgr_ability(
        0, PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
        PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
        PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
        PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
        PROCMGR_AID_EOL);
    if (procmgr_status != EOK) {
        perror("Requested abilities failed or denied!");
        exit(EXIT_FAILURE);
    }

    uint32_t temp, outputs;

    // Configure GPIOs as outputs
    // Port 1
    outputs = MOTOR_LEFT_PIN | MOTOR_RIGHT_PIN | MOTOR_SLOW_PIN |
              MOTOR_STOP_PIN | LAMP_RED_PIN | LAMP_YELLOW_PIN | LAMP_GREEN_PIN |
              SWITCH_PIN;
    temp = in32(GPIO_OE_REGISTER(gpio_bank_1));
    out32(GPIO_OE_REGISTER(gpio_bank_1), temp & ~outputs);

    // Port 2
    outputs = LED_Q1_PIN | LED_Q2_PIN | LED_RESET_PIN | LED_START_PIN;
    temp = in32(GPIO_OE_REGISTER(gpio_bank_2));
    out32(GPIO_OE_REGISTER(gpio_bank_2), temp & ~outputs);
}

void Actuators::setGreenBlinking(bool on) {
    if (th_GreenBlinking.joinable()) {
        greenBlinking = false;
        th_GreenBlinking.join();
    }

    if (on) {
        th_GreenBlinking =
            std::thread(&Actuators::thGreenLampFlashing, this, false);
    }
}

void Actuators::setYellowBlinking(bool on) {
    if (th_YellowBlinking.joinable()) {
        yellowBlinking = false;
        th_YellowBlinking.join();
    }

    if (on) {
        th_YellowBlinking =
            std::thread(&Actuators::thYellowLampFlashing, this, false);
    }
}

void Actuators::setRedBlinking(bool on, bool fast) {
    if (redBlinking) {
        redBlinking = false;
        if (th_RedBlinking.joinable())
            th_RedBlinking.join();
    }

    if (on) {
        th_RedBlinking = std::thread(&Actuators::thRedLampFlashing, this, fast);
    }
}

void Actuators::standbyMode() {
    motorStop();
    setGreenBlinking(false);
    setYellowBlinking(false);
    setRedBlinking(false, false);
    greenLampOff();
    yellowLampOff();
    redLampOff();
    q1LedOff();
    q2LedOff();
    startLedOn();
    resetLedOff();
    setMotorStop(true);
    setMotorRight(false);
    setMotorLeft(false);
    setMotorSlow(false);
    if(hasPusher){
        openSwitch();
    }else{
        closeSwitch();
    }
}


void Actuators::runningMode() {
    setGreenBlinking(false);
    setRedBlinking(false, false);
    greenLampOn();
    yellowLampOff();
    redLampOff();
    q1LedOff();
    q2LedOff();
    startLedOff();
    resetLedOff();
    setMotorStop(false);
}

void Actuators::serviceMode() {
    redLampOff();
    setGreenBlinking(true);
    q1LedOff();
    q2LedOff();
    startLedOff();
    resetLedOff();
    setMotorStop(false);
    setMotorRight(false);
    setMotorLeft(false);
    setMotorSlow(false);
}

void Actuators::errorMode() {
    setGreenBlinking(false);
    greenLampOff();
    setRedBlinking(true, true);
    setMotorStop(true);
    if(hasPusher){
    openSwitch();
    }else{
    closeSwitch();
    }
}

void Actuators::estopMode() {
    setGreenBlinking(false);
    setYellowBlinking(false);
    setRedBlinking(false, false);
    greenLampOff();
    yellowLampOff();
    redLampOff();
    setMotorStop(true);
    setMotorRight(false);
    setMotorLeft(false);
    setMotorSlow(false);
    if(hasPusher){
    openSwitch();
    }else{
    closeSwitch();
    }
    }
}

void Actuators::greenLampOn() {
    out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_GREEN_PIN);
}

void Actuators::greenLampOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_GREEN_PIN);
}

void Actuators::yellowLampOn() {
    out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_YELLOW_PIN);
}

void Actuators::yellowLampOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_YELLOW_PIN);
}

void Actuators::redLampOn() {
    out32(GPIO_SETDATAOUT(gpio_bank_1), LAMP_RED_PIN);
}

void Actuators::redLampOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_1), LAMP_RED_PIN);
}

void Actuators::thRedLampFlashing(bool fast) {
    int t_ms = fast ? ON_TIME_FAST_MS : ON_TIME_SLOW_MS;
    int t_actual;
    redBlinking = true;
    while (redBlinking) {
        redLampOn();
        t_actual = 0;
        while(t_actual < t_ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(BLINK_POLL_TIME_MS));
			if(!redBlinking)
				return;
			t_actual += 100;
        }
        redLampOff();
        t_actual = 0;
		while(t_actual < t_ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(BLINK_POLL_TIME_MS));
			if(!redBlinking)
				return;
			t_actual += 100;
		}
    }
}

void Actuators::thYellowLampFlashing(bool fast) {
    int t_ms = fast ? ON_TIME_FAST_MS : ON_TIME_SLOW_MS;
    int t_actual;
    yellowBlinking = true;
    while (yellowBlinking) {
        yellowLampOn();
        t_actual = 0;
        while(t_actual < t_ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(BLINK_POLL_TIME_MS));
			if(!yellowBlinking)
				return;
			t_actual += 100;
        }
        yellowLampOff();
        t_actual = 0;
		while(t_actual < t_ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(BLINK_POLL_TIME_MS));
			if(!yellowBlinking)
				return;
			t_actual += 100;
		}
    }
}

void Actuators::thGreenLampFlashing(bool fast) {
    int t_ms = fast ? ON_TIME_FAST_MS : ON_TIME_SLOW_MS;
    int t_actual;
    greenBlinking = true;
    while (greenBlinking) {
    	greenLampOn();
        t_actual = 0;
        while(t_actual < t_ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(BLINK_POLL_TIME_MS));
			if(!greenBlinking)
				return;
			t_actual += 100;
        }
        greenLampOff();
        t_actual = 0;
		while(t_actual < t_ms) {
			std::this_thread::sleep_for(std::chrono::milliseconds(BLINK_POLL_TIME_MS));
			if(!greenBlinking)
				return;
			t_actual += 100;
		}
    }
}

void Actuators::startLedOn() {
    out32(GPIO_SETDATAOUT(gpio_bank_2), LED_START_PIN);
}

void Actuators::startLedOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_START_PIN);
}

void Actuators::resetLedOn() {
    out32(GPIO_SETDATAOUT(gpio_bank_2), LED_RESET_PIN);
}

void Actuators::resetLedOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_RESET_PIN);
}

void Actuators::q1LedOn() { out32(GPIO_SETDATAOUT(gpio_bank_2), LED_Q1_PIN); }

void Actuators::q1LedOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_Q1_PIN);
}

void Actuators::q2LedOn() { out32(GPIO_SETDATAOUT(gpio_bank_2), LED_Q2_PIN); }

void Actuators::q2LedOff() {
    out32(GPIO_CLEARDATAOUT(gpio_bank_2), LED_Q2_PIN);
}

void Actuators::setMotorStop(bool stop) {
    if (stop) {
        out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
    } else {
        out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_STOP_PIN);
    }
    out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
    out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
}

void Actuators::setMotorSlow(bool slow) {
    if (slow)
        out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
    else
        out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_SLOW_PIN);
}

void Actuators::setMotorRight(bool right) {
    if (right)
        out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
    else
        out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_RIGHT_PIN);
}

void Actuators::setMotorLeft(bool left) {
    if (left)
        out32(GPIO_SETDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
    else
        out32(GPIO_CLEARDATAOUT(gpio_bank_1), MOTOR_LEFT_PIN);
}

void Actuators::openSwitch() {
	if(hasPusher) {
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), SWITCH_PIN);
	} else {
		out32(GPIO_SETDATAOUT(gpio_bank_1), SWITCH_PIN);
	}
}

void Actuators::closeSwitch() {
	if(hasPusher) {
		out32(GPIO_SETDATAOUT(gpio_bank_1), SWITCH_PIN);
	} else {
		out32(GPIO_CLEARDATAOUT(gpio_bank_1), SWITCH_PIN);
	}
}

void Actuators::sortOut() {
    // Has pusher -> push out for 500ms, then return to default position
    // No pusher -> nothing to do, workpiece will be sorted out!
    if (hasPusher) {
        Logger::debug("[Actuators] Pusher out for " + std::to_string(ON_TIME_PUSHER_MS) + " ms to sort out workpiece");
        std::thread t([=]() {
            closeSwitch();
            std::this_thread::sleep_for(std::chrono::milliseconds(ON_TIME_PUSHER_MS));
            openSwitch();
        });
        t.detach();
    } else {
        Logger::debug("[Actuators] Let switch sort out workpiece");
        closeSwitch();
    }
}

void Actuators::letPass() {
    // No pusher -> open
    // Has pusher -> nothing to do, workpiece will pass!
    if (!hasPusher) {
        Logger::debug("[Actuators] Open switch to let workpiece pass");
        openSwitch();
    } else {
        Logger::debug("[Actuators] Let pusher pass workpiece");
    }
}

void Actuators::allOff() {
	motorStop();
	setGreenBlinking(false);
	greenLampOff();
	setYellowBlinking(false);
	yellowLampOff();
	setRedBlinking(false, false);
	redLampOff();
	q1LedOff();
	q2LedOff();
	startLedOff();
	resetLedOff();
}

void Actuators::motorStop() {
    Logger::debug("[Actuators] Motor stop");
    setMotorStop(true);
    setMotorSlow(false);
    setMotorRight(false);
    setMotorLeft(false);
}

void Actuators::motorSlow() {
    Logger::debug("[Actuators] Motor slow");
    setMotorStop(false);
    setMotorSlow(true);
    setMotorRight(true);
    setMotorLeft(false);
}

void Actuators::motorFast() {
    Logger::debug("[Actuators] Motor fast");
    setMotorStop(false);
    setMotorSlow(false);
    setMotorRight(true);
    setMotorLeft(false);
}
