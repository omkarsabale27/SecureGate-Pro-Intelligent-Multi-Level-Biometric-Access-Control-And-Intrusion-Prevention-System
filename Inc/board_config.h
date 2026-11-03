/* board_config.h - every wiring-dependent choice in one place */
#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

/* ---- UART assignment ----
   The R305 and the PC log use different UARTs: whichever UART the R305 is on, the log uses the other.
   At every start-up R305_AutoDetect() probes UART0/UART1 at 9600 and 57600 and uses the first
   combination where the module answers. R305_UART / R305_BAUD are the first guess and the fallback if
   nothing answers. Default = project block diagram: R305 on UART1 (P0.8/P0.9) at the module's factory
   57600 baud, PC log on UART0 (P0.0/P0.1, via MAX232). The result is shown on the LCD and in the PC log. */
#define R305_UART        1
#define R305_BAUD        57600
#define LOG_BAUD         9600
#define R305_PROBE_MS    300        /* per-probe reply timeout during auto-detect */

/* ---- R305 identity (factory address 0xFFFFFFFF, factory password 0xFFFFFFFF) ----
   The module only answers packets that carry its own address. If SYDemo.exe shows a different
   address, put it in R305_ADDR. R305_PASS is tried first at the start-up handshake; the other
   usual values (0x00000000 / 0xFFFFFFFF) are tried automatically if it is rejected. */
#define R305_ADDR        0xFFFFFFFFUL
#define R305_PASS        0x00000000UL

/* ---- LCD, 8-bit mode: D0-D7 on 8 CONTIGUOUS pins of one port, RW tied to GND ----
   D0 = pin LCD_DATA_BASE_PIN, D7 = LCD_DATA_BASE_PIN + 7. Default: P1.24-P1.31, RS = P0.12, EN = P0.13.
   P1.26-P1.31 are JTAG pins if the board's DBGSEL pin is high at reset. */
#define LCD_DATA_PORT      1
#define LCD_DATA_BASE_PIN  24
#define LCD_RS_PIN         12       /* P0.12 */
#define LCD_EN_PIN         13       /* P0.13 */

/* ---- Door motor (L293D) ----
   IN1 = P0.4, IN2 = P0.5, ENABLE (EN1) = P0.7.
   MOTOR_EN_ENABLED 1: P0.7 is driven high only while the motor runs. Leave it at 1 if EN1 is wired
   to P0.7 (a motor that is never "enabled" does not move); set 0 if EN1 is tied to +5 V. */
#define MOTOR_IN1          (1UL << 4)
#define MOTOR_IN2          (1UL << 5)
#define MOTOR_EN_ENABLED   1
#define MOTOR_EN           (1UL << 7)

/* ---- Door-closed limit switch: P0.6, active low (switch to GND when the door is shut) ----
   The LPC2148 has no internal pull-ups: fit an external 10k pull-up to 3.3 V.
   DOOR_SWITCH_ENABLED 0: no switch fitted - the door is closed by time (DOOR_CLOSE_MS) only.
   DOOR_SWITCH_ENABLED 1: reverse until the switch closes; DOOR_CLOSE_MS becomes the time-out. */
#define DOOR_SWITCH_ENABLED 0
#define DOOR_SW             (1UL << 6)

#define DOOR_OPEN_MS     3000       /* forward run time                         */
#define DOOR_HOLD_MS     2000       /* door stays open                          */
#define DOOR_CLOSE_MS    3000       /* reverse run time (or time-out with switch) */
#define DOOR_DENY_MS     1500       /* reverse pulse on denied / locked access  */

/* ---- Fingerprint timing ---- */
#define FP_WAIT_MS       10000      /* how long to wait for a finger */

/* build-time safety: the LCD data bus must not sit on the UART1 pins (P0.8/P0.9) */
#if (LCD_DATA_PORT == 0) && (LCD_DATA_BASE_PIN <= 9) && (LCD_DATA_BASE_PIN + 7 >= 8)
#error LCD data pins overlap P0.8/P0.9 (R305 UART1) - change LCD_DATA_PORT / LCD_DATA_BASE_PIN
#endif

#endif /* __BOARD_CONFIG_H */
