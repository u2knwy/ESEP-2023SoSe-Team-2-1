/** 
 * File:   iomasks.h
 * 
 * Bit masks for process manipulation of FESTO system.
 *
 * @author Thomas Lehmann
 * @date 2016-09-17
 * @version 1
 * Initial version
 */

#ifndef SIMMASKS_H
#define	SIMMASKS_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Output Masks, Port A low-byte, Port C high-byte */
#define SIM_DRIVE_DIRECTION_RIGHT   0x0001
#define SIM_DRIVE_DIRECTION_LEFT    0x0002
#define SIM_DRIVE_SLOW              0x0004
#define SIM_DRIVE_STOP              0x0008
#define SIM_FEED_SEPARATOR          0x0080
#define SIM_ALARM_LAMP_GREEN        0x0040
#define SIM_ALARM_LAMP_YELLOW       0x0020
#define SIM_ALARM_LAMP_RED          0x0010

#define SIM_LED_START_BUTTON        0x0100
#define SIM_LED_RESET_BUTTON        0x0200
#define SIM_LED_Q1                  0x0400
#define SIM_LED_Q2                  0x0800

/* Input Masks, Port B low-byte, Port C high-byte, move to lower nibble */
#define SIM_ITEM_DETECTED          (0x0001<<0)
#define SIM_ITEM_AT_HEIGHT_SENSOR  (0x0001<<1)
#define SIM_ITEM_HEIGHT_FITS       (0x0001<<2)
#define SIM_ITEM_AT_JUNCTION       (0x0001<<3)
#define SIM_ITEM_IS_METTAL         (0x0001<<4)
#define SIM_JUNCTION_IS_OPEN       (0x0001<<5)
#define SIM_BUFFER_IS_FULL         (0x0001<<6)
#define SIM_ITEM_AT_END            (0x0001<<7)

#define SIM_BUTTON_START   (0x0001<<8)
#define SIM_BUTTON_STOP    (0x0001<<9)
#define SIM_BUTTON_RESET   (0x0001<<10)
#define SIM_EMERGENCY_STOP (0x0001<<11)

#ifdef	__cplusplus
}
#endif

#endif	/* IOMASKS_H */

