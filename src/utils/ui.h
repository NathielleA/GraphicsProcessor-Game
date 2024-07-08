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
#define FROG_front "frog-front"
#define FROG_back "frog-back"

#define CAR_RED "car-red"
#define CAR_YELLOW "car-yellow"

#define LILYPAD_1 "lilypad1"
#define LILYPAD_2 "lilypad1"

#define TRUCK_BACK "truck-b"
#define TRUCK_FRONT "truck-f"
#define TRUCK_MIDDLE "truck-m"

#define TRUNK_TREE_BACK "truck-b"
#define TRUNK_TREE_FRONT "truck-f"
#define TRUNK_TREE_MIDDLE "truck-m"

#define A_font "a-font"
#define C_font "c-font"
#define H_font "h-font"
#define K_font "k-font"
#define O_font "o-font"
#define R_font "r-font"
#define S_font "s-font"
#define U_font "u-font"

#endif  // !UI_H_