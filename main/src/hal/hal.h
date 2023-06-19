/*
 * hal.h
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */
#pragma once

// For QNX Neutrino
#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#ifdef SIMULATION
#include "simulation/simulationadapterqnx/simqnxgpioapi.h"
#endif

/*---------------------------------------------------------------------------
   GPIO CONFIGURATION
----------------------------------------------------------------------------- */
#define SIZE_4KB                  0x1000
#define SIZE_8KB                  0x2000
#define GPIO_BANK_0               0x44E07000
#define GPIO_BANK_1               0x4804C000
#define GPIO_BANK_2               0x481AC000
#define GPIO_BANK_3               0x481AE000
#define GPIO_CLEARDATAOUT(base)   (uintptr_t) base + 0x190
#define GPIO_SETDATAOUT(base)     (uintptr_t) base + 0x194
#define GPIO_DATAIN(base)         (uintptr_t) base + 0x138
#define GPIO_DEBOUNCENABLE(base)  (uintptr_t) base + 0x150
#define GPIO_DEBOUNCINGTIME(base) (uintptr_t) base + 0x154
// Input Debouncing Value in 31 microsecond steps. Debouncing Value =
// (DEBOUNCETIME + 1) * 31 microseconds (see p. 4875)
#define GPIO_DEBOUNCE_VALUE 321   // ca. 10ms

/* GPIO0 PINS -> INPUTS */
#define LB_START_PIN     (1 << 2)   // Werkstück im Einlauf = low when true
#define LB_HEIGHT_PIN    (1 << 3)   // Werkstück in Höhenmessung = low when true
#define LB_HEIGHT_OK_PIN (1 << 4)    // Werkstück Höhe OK = high when true
#define LB_SWITCH_PIN    (1 << 5)    // Werkstück in Weiche = low when true
#define SE_METAL_PIN     (1 << 7)    // Werkstück Metall = high when true
#define SE_SWITCH_PIN    (1 << 14)   // Weiche offen = high when true
#define LB_RAMP_PIN      (1 << 15)   // Rutsche voll = low when true
#define LB_END_PIN       (1 << 20)   // Werkstück im Auslauf = low when true
#define KEY_START_PIN    (1 << 22)   // Taste Start = high, wenn betätigt
#define KEY_STOP_PIN     (1 << 23)   // Taste Stop = low, wenn betätigt
#define KEY_RESET_PIN    (1 << 26)   // Taste Reset = high, wenn betätigt
#define ESTOP_PIN        (1 << 27)   // E-Stop = low, wenn betätigt

/* GPIO1 PINS -> OUTPUTS */
#define MOTOR_RIGHT_PIN (1 << 12)   // Motor Rechtslauf
#define MOTOR_LEFT_PIN  (1 << 13)   // Motor Linkslauf
#define MOTOR_SLOW_PIN  (1 << 14)   // Motor langsam
#define MOTOR_STOP_PIN  (1 << 15)   // Motor Stop
#define LAMP_RED_PIN    (1 << 16)   // Rote Lampe an
#define LAMP_YELLOW_PIN (1 << 17)   // Gelbe Lampe an
#define LAMP_GREEN_PIN  (1 << 18)   // Grüne Lampe an
#define SWITCH_PIN      (1 << 19)   // Weiche öffnen

/* GPIO2 PINS -> OUTPUTS */
#define LED_START_PIN (1 << 2)   // LED Taste Start
#define LED_RESET_PIN (1 << 3)   // LED Taste Reset
#define LED_Q1_PIN    (1 << 4)   // Signalleuchte Q1
#define LED_Q2_PIN    (1 << 5)   // Signalleuchte Q2

/* Interrupt numbers  (spruh73l.pdf S.465 ff.) */
#define INTR_GPIO_PORT0 97
#define INTR_GPIO_PORT1 99
#define INTR_GPIO_PORT2 33

/* GPIO register offsets (spruh73l.pdf S.4877) */
#define GPIO_OE_REGISTER(base)   (uintptr_t) base + 0x134
#define GPIO_LEVELDETECT0(base)  (uintptr_t) base + 0x140
#define GPIO_LEVELDETECT1(base)  (uintptr_t) base + 0x144
#define GPIO_RISINGDETECT(base)  (uintptr_t) base + 0x148
#define GPIO_FALLINGDETECT(base) (uintptr_t) base + 0x14C

#define GPIO_IRQSTATUS_0(base)     (uintptr_t) base + 0x2C
#define GPIO_IRQSTATUS_1(base)     (uintptr_t) base + 0x30
#define GPIO_IRQSTATUS_SET_0(base) (uintptr_t) base + 0x34
#define GPIO_IRQSTATUS_SET_1(base) (uintptr_t) base + 0x38
