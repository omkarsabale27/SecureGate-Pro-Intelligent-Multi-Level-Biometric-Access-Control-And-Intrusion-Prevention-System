# SecureGate-Pro-Intelligent-Multi-Level-Biometric-Access-Control-And-Intrusion-Prevention-System
<div align="center">🔐 SecureGate Pro – Smart Biometric Access Control System
A password- and fingerprint-based embedded security system for controlled door access




</div>📌 Overview
SecureGate Pro is an embedded biometric access-control system developed using the NXP LPC2148 ARM7TDMI-S microcontroller. It provides multi-level user authentication using a user ID, password, and fingerprint verification before operating a motorized door.

The system stores user credentials and administrative settings in an AT24C256 I2C EEPROM, while an R305 fingerprint sensor performs biometric enrolment and verification. A 4×4 keypad and character LCD provide the user interface. An L293D motor driver controls the door motor, and the LPC2148's internal RTC supplies timestamps for access logs.

The project also includes an administrator menu, account lockout, system lockdown after repeated invalid attempts, and serial access logging to a PC terminal.
💡 The door is opened only after the entered user ID, password, and fingerprint have all been successfully verified.

✨ Features
Feature	Description
🪪 User ID Authentication	Accepts numeric user IDs of up to 6 digits
🔑 Password Authentication	Each user has a 4–8 digit password
🧬 Fingerprint Verification	R305 sensor captures and matches the user's fingerprint
🚪 Automatic Door Control	L293D drives the motor to open and close the door after successful authentication
🔐 Account Locking	Three wrong passwords lock the user account
🚨 Fingerprint Failure Protection	Three fingerprint mismatches lock the corresponding account
⛔ System Lockdown	Three consecutive invalid IDs or three wrong admin PIN attempts trigger a 30-second lockdown
👨‍💼 Administrator Menu	Allows user management, password updates, fingerprint management, log viewing, and clock setting
💾 EEPROM Storage	Stores user records and the admin PIN in AT24C256 EEPROM
🕒 RTC Timestamping	Adds date and time to access-log entries
🖥️ LCD Interface	Displays prompts, authentication status, and system messages
⌨️ 4×4 Keypad	Used for ID/password entry and menu navigation
📟 PC Serial Logging	Sends timestamped access and security events to a terminal
🔎 R305 Auto Detection	Probes UART0/UART1 at 9600 and 57600 baud to find the fingerprint module
🛠️ I2C Recovery	Uses timeouts and bus-recovery clock pulses to avoid indefinite I2C waits


🔄 System Working Flow
1. System Initialization
Power ON
   ↓
Initialize motor, LCD, keypad, UART, I2C, RTC and EINT0
   ↓
Automatically detect R305 fingerprint sensor
   ↓
Check AT24C256 EEPROM and load administrator PIN
   ↓
Initialize PC access log
   ↓
Display "ENTER ID"
If the EEPROM does not respond, the LCD displays an EEPROM wiring error and the firmware retries the connection instead of remaining stuck in an I2C wait.
2. User Authentication
Enter User ID
     ↓
Check user record in EEPROM
     ↓
Is the ID valid?
  No ───────────────→ Access denied
  Yes
     ↓
Enter password
     ↓
Does password match?
  No ───────────────→ Count failed attempts
  Yes
     ↓
Place finger on R305 sensor
     ↓
Capture fingerprint and search stored templates
     ↓
Does fingerprint match this user's template?
  No ───────────────→ Access denied / mismatch counter
  Yes
     ↓
ACCESS GRANTED
     ↓
Open door → Hold open → Close door
3. Door Operation
- Access granted: The L293D receives the forward motor-control signal and the door opens.
- Hold: The door remains open for the configured hold interval.
- Close: The motor runs in reverse to close the door.
- Access denied / locked account: The motor receives a short reverse pulse as the configured deny response.
- Door switch: Optional active-low door-closed switch support is available on P0.6. It is disabled by default; when enabled, the firmware checks the switch while closing.
Default timings in board_config.h:
Operation	Configured time
Door opening	3000 ms
Door hold	2000 ms
Door closing / timeout	3000 ms
Denied-access reverse pulse	1500 ms
Fingerprint wait timeout	10000 ms


🔐 Security and Lockout Logic
Condition	System response
Three wrong passwords for a user	That user account is locked and saved in EEPROM
Three fingerprint mismatches for a user	That user account is locked and saved in EEPROM
Three consecutive invalid user IDs	30-second system lockdown
Three incorrect administrator PIN attempts	30-second system lockdown
Locked user attempts access	Access denied; administrator must unlock the account


The administrator can unlock a user through the admin menu. The user account and lock state are stored in EEPROM, so account records are retained across power cycles.
👨‍💼 Administrator Menu
Press the EINT0 admin switch and enter the administrator PIN to access the menu.
Key	Function
1	Change a user's password or the admin PIN (0 selects admin)
2	Fingerprint management: enrol, delete one user's fingerprint, or delete all fingerprints
3	Add a new user
4	Delete a user
5	Exit administrator menu
A	Unlock a locked user account
B	View the latest 25 log entries
C	Set RTC time and date


Factory administrator PIN: 1234
Change this PIN from the administrator menu.
User Input Keys
- Numeric keys: enter user ID or password.
- #: submit the entered value.
- *: delete the previous digit (backspace).
Fingerprint Enrolment
Fingerprint enrolment uses a two-scan process:
1. Place the finger on the R305 sensor for the first scan.
2. Remove the finger when prompted.
3. Place the same finger again for the second scan.
4. The module combines both scans into a template and stores it in its fingerprint library.
The template ID is associated with the user's EEPROM record.
🧱 System Architecture
                       ┌─────────────────────┐
                       │    LPC2148 ARM7     │
                       │   Main Controller  │
                       └──────────┬──────────┘
                                  │
          ┌───────────────────────┼────────────────────────┐
          │                       │                        │
       UART0/1                   I2C0                 GPIO / EINT0
          │                       │                        │
   ┌──────▼──────┐       ┌────────▼────────┐       ┌───────▼──────┐
   │ R305 Finger │       │ AT24C256 EEPROM │       │  Admin Switch│
   │ Print Sensor│       │ User Data / PIN │       │   P0.16      │
   └─────────────┘       └─────────────────┘       └──────────────┘

          ┌───────────────────────┼────────────────────────┐
          │                       │                        │
        GPIO                    GPIO                      UART
          │                       │                        │
   ┌──────▼──────┐       ┌────────▼────────┐       ┌───────▼──────┐
   │ 4×4 Keypad  │       │ Character LCD  │       │ PC Terminal  │
   │ User Input  │       │ User Interface │       │ Access Logs  │
   └─────────────┘       └─────────────────┘       └──────────────┘

                                  │
                            GPIO Motor Control
                                  │
                         ┌────────▼────────┐
                         │      L293D      │
                         │   Motor Driver  │
                         └────────┬────────┘
                                  │
                            Door Motor
🔌 Hardware Components
#	Component	Part / Specification	Purpose
1	Microcontroller	NXP LPC2148, ARM7TDMI-S	Main controller
2	Fingerprint Sensor	R305	Fingerprint enrolment and matching
3	External EEPROM	AT24C256, I2C, address 0x50	Stores user records and admin PIN
4	Motor Driver	L293D	Controls door motor direction
5	Motor	DC motor / door mechanism	Opens and closes the door
6	LCD	Character LCD, 8-bit interface	Displays prompts and status
7	Keypad	4×4 matrix keypad	User and admin input
8	Serial Interface	MAX232 (for PC UART connection)	PC terminal logging
9	Push Button	EINT0 admin switch	Opens administrator menu
10	Door Sensor	Optional active-low switch	Detects closed-door position
11	Power Supply	Regulated supplies as required by modules	Powers controller and peripherals


📍 Pin Configuration
The wiring configuration is centralized in src/board_config.h.
GPIO — Port 0
Pin	Signal	Function
P0.0	TXD0	UART0 transmit; PC logging when UART0 is assigned to the PC
P0.1	RXD0	UART0 receive
P0.2	SCL0	I2C clock for AT24C256
P0.3	SDA0	I2C data for AT24C256
P0.4	L293D IN1	Motor direction control
P0.5	L293D IN2	Motor direction control
P0.6	DOOR_SW	Optional active-low door-closed input
P0.7	MOTOR_EN	L293D enable output
P0.8	TXD1	UART1 transmit; default R305 connection
P0.9	RXD1	UART1 receive; default R305 connection
P0.12	LCD_RS	LCD register select
P0.13	LCD_EN	LCD enable
P0.16	EINT0	Administrator menu switch


GPIO — Port 1
Pin	Signal	Function
P1.16–P1.19	ROW0–ROW3	Keypad row lines
P1.20–P1.23	COL0–COL3	Keypad column lines
P1.24–P1.31	LCD D0–D7	LCD 8-bit data bus


Communication Interfaces
Interface	Pins	Connected Device
UART0	P0.0, P0.1	PC terminal or R305, depending on auto-detection
UART1	P0.8, P0.9	R305 or PC terminal, depending on auto-detection
I2C0	P0.2, P0.3	AT24C256 EEPROM
External Interrupt	P0.16 / EINT0	Administrator switch


Hardware note: P1.26–P1.31 may be used by JTAG depending on the board's debug configuration. Check the board's DBGSEL/JTAG configuration before using these pins for the LCD data bus. The I2C bus requires suitable pull-up resistors.
🧠 Communication and Peripheral Details
1. R305 Fingerprint Sensor
- Communicates with the LPC2148 through UART.
- At startup, firmware probes UART0 and UART1 at 9600 and 57600 baud.
- The detected UART and baud rate are shown on the LCD and recorded in the PC log.
- Performs image capture, character generation, template enrolment, fingerprint search, and template deletion.
- Default module address is 0xFFFFFFFF; module settings are defined in board_config.h.
2. AT24C256 EEPROM
- Connected through I2C0.
- Uses 7-bit I2C address 0x50 (A0–A2 tied to GND).
- Stores up to 8 user records and the administrator PIN.
- Retains user records and lock status across power cycles.
- I2C operations use timeouts. Startup bus recovery sends up to 9 clock pulses followed by a STOP condition to release a stuck bus.
- If the EEPROM is unavailable, the firmware displays an error and retries instead of waiting forever.
3. LPC2148 RTC
- Uses the microcontroller's internal RTC.
- Supplies date and time for timestamped access-log entries.
- On a fresh start without a running battery-backed clock, the firmware initializes it to 01/01/2026 00:00:00.
- The administrator can update the clock using menu option C.
4. PC Serial Logging
- Sends a heading: SECUREGATE PRO ACCESS LOG.
- Each event is timestamped using the RTC.
- The PC terminal is configured for 9600 baud, 8 data bits, no parity, 1 stop bit (8N1).
- The firmware keeps recent log entries in a ring buffer for administrator review (latest 25 entries).
Example log entries:
SECUREGATE PRO ACCESS LOG
LOG 001: [01/01/2026 10:15:20] SYSTEM STARTED
LOG 002: [01/01/2026 10:15:21] R305 DETECTED ON UART1 AT 57600 BAUD
LOG 003: [01/01/2026 10:15:22] EEPROM OK
LOG 004: [01/01/2026 10:16:02] USER 243562 - USER ID VERIFIED
LOG 005: [01/01/2026 10:16:05] USER 243562 - PASSWORD VERIFIED
LOG 006: [01/01/2026 10:16:08] USER 243562 - FINGERPRINT VERIFIED
LOG 007: [01/01/2026 10:16:08] USER 243562 - ACCESS GRANTED
The entries above illustrate the log format; timestamps and event sequence depend on actual operation.
📁 Project Structure
SecureGate_Pro/
│
├── Startup.s                  ← LPC2148 startup and vector table
├── SecureGate_Pro.uvproj      ← Keil µVision project
├── SecureGate_Pro.uvopt       ← Keil project options
│
├── src/
│   ├── main.c                 ← Main authentication flow and system initialization
│   ├── board_config.h         ← Centralized pin mapping and hardware configuration
│   ├── types.h                ← Common data types and system constants
│   │
│   ├── admin.c                ← Admin authentication and user-management menu
│   ├── admin.h                ← Admin module declarations
│   ├── interrupt.c            ← EINT0 admin-switch interrupt
│   ├── interrupt.h            ← Interrupt declarations and flags
│   │
│   ├── r305.c                 ← Fingerprint sensor communication and commands
│   ├── r305.h                 ← R305 driver API
│   ├── r305_defines.h         ← R305 packet and command definitions
│   │
│   ├── eeprom.c               ← User/admin data storage and retrieval
│   ├── eeprom.h               ← EEPROM memory map and user record structure
│   ├── i2c.c                  ← I2C communication and bus recovery
│   ├── i2c.h                  ← I2C driver declarations
│   ├── i2c_defines.h          ← I2C register and bit definitions
│   │
│   ├── lcd.c                  ← LCD low-level driver
│   ├── lcd.h                  ← LCD API and pin definitions
│   ├── keypad.c               ← 4×4 keypad scanning and input handling
│   ├── keypad.h               ← Keypad API
│   │
│   ├── motor.c                ← Door motor direction and timing control
│   ├── motor.h                ← Motor driver API
│   ├── rtc.c                  ← Internal RTC initialization and access
│   ├── rtc.h                  ← RTC API
│   ├── delay.c                ← Software delay implementation
│   ├── delay.h                ← Delay declarations
│   │
│   ├── uart0.c                ← UART0 and serial access-log functions
│   ├── uart0.h                ← UART0 API
│   ├── uart0_defines.h        ← UART0 configuration definitions
│   ├── uart1.c                ← UART1 communication functions
│   ├── uart1.h                ← UART1 API
│   └── uart1_defines.h        ← UART1 configuration definitions
│
└── Objects/                   ← Keil build output (generated)
🛠️ Development Environment
Tool / Parameter	Details
IDE	Keil µVision 4/5
Compiler	Keil ARM C Compiler
Target MCU	NXP LPC2148
Processor Core	ARM7TDMI-S
CPU Clock	60 MHz (project configuration)
Peripheral Clock	15 MHz (configured in types.h)
Programming Language	Embedded C
Fingerprint Module	R305
External Memory	AT24C256 I2C EEPROM
Motor Driver	L293D
PC Serial Terminal	HyperTerminal or another serial terminal
PC Interface	MAX232 as required by the hardware setup


🚀 Getting Started
Prerequisites
- Keil µVision IDE installed.
- LPC2148 development board or a compatible simulation setup.
- R305 fingerprint sensor.
- AT24C256 EEPROM with correct I2C wiring and pull-ups.
- L293D motor-driver circuit and compatible motor.
- Character LCD and 4×4 keypad.
- MAX232 serial interface and a PC terminal for viewing logs.
- Suitable regulated power supplies and common ground between connected modules.
Build and Flash
1. Extract the project ZIP file.
2. Open SecureGate_Pro/SecureGate_Pro.uvproj in Keil µVision.
3. Confirm that the target device is LPC2148.
4. Check that all source files under src/ are included in the Keil project.
5. Verify the include paths and the pin configuration in src/board_config.h.
6. Build the project using Project → Build Target or press F7.
7. Locate the generated HEX file in the Objects/ folder.
8. Flash the HEX file to the LPC2148 using a supported programmer or ISP tool.
9. Connect the PC serial terminal at 9600 8N1 to view access logs.
10. Power up the system and follow the LCD prompts.
First-Time Setup
1. Confirm the R305 sensor is detected at startup.
2. Confirm the EEPROM responds and the LCD reaches the ENTER ID screen.
3. Press the EINT0 admin switch.
4. Enter the factory admin PIN 1234.
5. Change the admin PIN.
6. Add a user using admin option 3; the enrolment process stores the user's fingerprint.
7. Test access using the newly created user ID, password, and enrolled finger.
8. Set the correct RTC date and time using admin option C.
⚙️ Configuration
Hardware-dependent settings are grouped in src/board_config.h.
Macro	Default	Purpose
R305_UART	1	Preferred UART for R305
R305_BAUD	57600	Preferred R305 baud rate
LOG_BAUD	9600	PC terminal logging baud rate
R305_ADDR	0xFFFFFFFF	R305 module address
R305_PASS	0x00000000	Configured R305 handshake password
MOTOR_EN_ENABLED	1	Controls L293D enable pin from P0.7
DOOR_SWITCH_ENABLED	0	Enables optional door-closed switch on P0.6
DOOR_OPEN_MS	3000	Door opening motor duration
DOOR_HOLD_MS	2000	Door open hold duration
DOOR_CLOSE_MS	3000	Door closing duration / timeout
DOOR_DENY_MS	1500	Reverse pulse on denied access
FP_WAIT_MS	10000	Fingerprint capture wait timeout


Change these values only after checking the actual circuit wiring and hardware requirements.
⚠️ Important Hardware Notes
- Use the correct voltage levels for the LPC2148 and peripheral modules.
- The R305 UART connection and PC logging UART are dynamically selected so they use different UARTs.
- The PC serial interface requires an appropriate level-shifting circuit such as MAX232 when connecting to a traditional RS-232 port.
- Fit external pull-up resistors on I2C SCL and SDA.
- The optional door-closed switch is disabled by default (DOOR_SWITCH_ENABLED 0). Enable it only after wiring the active-low switch and its external pull-up.
- Ensure the L293D motor supply and logic supply are suitable for the motor and driver; connect grounds appropriately.
- P1.26–P1.31 can be associated with JTAG. Verify the board's debug selection before using them as LCD data pins.
- Test motor direction and mechanical travel safely before connecting the door mechanism.
🔮 Possible Future Enhancements
- Add a dedicated door-position sensor and obstruction detection.
- Add an alarm or notification for repeated unauthorized access.
- Add a larger event-history storage system.
- Add network connectivity for remote monitoring.
- Add an administrator interface for exporting access reports.
<div align="center">Built with ❤️ using ARM7 | LPC2148 | Embedded C | R305 Biometrics
</div>
