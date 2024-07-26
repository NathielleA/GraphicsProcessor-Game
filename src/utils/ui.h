#ifndef UI_H_
#define UI_H_

#define WRREG 0xc0  /**< Write register command */
#define DATA_B 0x70 /**< Data bus value B */
#define DATA_A 0x80 /**< Data bus value A */

#define LEFT 0         /**< Direction constant: Left */
#define RIGHT 4        /**< Direction constant: Right */
#define UP 2           /**< Direction constant: Up */
#define DOWN 6         /**< Direction constant: Down */
#define UPPER_RIGHT 1  /**< Direction constant: Upper-right */
#define UPPER_LEFT 3   /**< Direction constant: Upper-left */
#define BOTTOM_LEFT 5  /**< Direction constant: Bottom-left */
#define BOTTOM_RIGHT 7 /**< Direction constant: Bottom-right */

/* OPCODES */
#define WBR 0 /**< Write to register opcode */
#define WSM 1 /**< Write to shared memory opcode */
#define WBM 2 /**< Write to bitmap memory opcode */
#define DP 3  /**< Direct programming opcode */

/* START CLOCK PULSE GENERATOR */
#define LOW_LOGIC_LEVEL 0x00000000  /**< Low logic level for clock pulse */
#define HIGH_LOGIC_LEVEL 0x00000001 /**< High logic level for clock pulse */

#define DEVICE_PATH "/dev/gpp_data_bus" /**< Path to device data bus */

/* SPRITES */
#define FROG_FRONT 10
#define FROG_BACK 11

#define CAR_RED 20
#define CAR_YELLOW 21

#define LILYPAD_1 30
#define LILYPAD_2 31

#define TRUCK_FRONT 40
#define TRUCK_BACK 41

#define TRUNK_TREE_FRONT 50
#define TRUNK_TREE_MIDDLE 51
#define TRUNK_TREE_BACK 52

#define A_font 60
#define C_font 61
#define H_font 62
#define K_font 63
#define O_font 64
#define R_font 65
#define S_font 66
#define U_font 67

#endif  // !UI_H_