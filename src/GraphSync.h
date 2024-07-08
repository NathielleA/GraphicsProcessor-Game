#include <stdio.h>

#include "utils/types.h"

// Structure definition for dynamic sprite
typedef struct sprite_t sprite_t;

struct sprite_t {
  u32_t coord_x;       /**< X-coordinate of the sprite. */
  u32_t coord_y;       /**< Y-coordinate of the sprite. */
  u32_t direction;     /**< Direction of movement. */
  u32_t offset;        /**< Offset used for bitmap selection. */
  u32_t data_register; /**< Register where sprite information is stored. */
  u32_t step_x;        /**< Number of pixels to move in the X direction. */
  u32_t step_y;        /**< Number of pixels to move in the Y direction. */
  u32_t ativo;         /**< Activation status of the sprite. */
  u32_t collision;     /**< Collision status of the sprite. */
};

// Structure definition for fixed sprite
typedef struct sprite_fixed_t sprite_fixed_t;

struct sprite_fixed_t {
  u32_t coord_x;       /**< X-coordinate of the fixed sprite. */
  u32_t coord_y;       /**< Y-coordinate of the fixed sprite. */
  u32_t offset;        /**< Offset used for bitmap selection. */
  u32_t data_register; /**< Register where sprite information is stored. */
  u32_t ativo;         /**< Activation status of the fixed sprite. */
};

// Structure definition for polygon
typedef struct polygon_t polygon_t;

struct polygon_t {
  u32_t ref_point_x;   /**< X-coordinate of the reference point of the polygon. */
  u32_t ref_point_y;   /**< Y-coordinate of the reference point of the polygon. */
  u32_t size;          /**< Size of the polygon. */
  u32_t data_register; /**< Register where polygon information is stored. */
  u32_t type;          /**< Type of the polygon. */
  u32_t R;             /**< Red color component of the polygon. */
  u32_t G;             /**< Green color component of the polygon. */
  u32_t B;             /**< Blue color component of the polygon. */
};

// Structure definition for ground block
typedef struct ground_block_t ground_block_t;

struct ground_block_t {
  u64_t address; /**< Address of the ground block. */
  u32_t R;       /**< Red color component of the ground block. */
  u32_t G;       /**< Green color component of the ground block. */
  u32_t B;       /**< Blue color component of the ground block. */
};

// Function prototypes

/**
 * @brief Writes data to a hardware device.
 *
 * @param data The data to be written.
 */
void write_data(u64_t data);

/**
 * @brief Reads data from a hardware device.
 *
 * @return The read data.
 */
u64_t read_data();

/**
 * @brief Closes a data resource, such as a file.
 *
 * @return A code indicating the success or failure of the closing operation.
 */
u8_t close_data();

/**
 * @brief Sets a ground block in the graphics processor.
 *
 * @param block The ground block to be set.
 * @return The final instruction that will be send to the device hardware.
 */
u64_t set_background_block(ground_block_t block);

/**
 * @brief Sets a fixed sprite in the graphics processor.
 *
 * @param sprite The fixed sprite to be set.
 * @return The final instruction that will be send to the device hardware.
 */
u64_t set_fixed_sprite(sprite_fixed_t sprite);

/**
 * @brief Sets a dynamic sprite in the graphics processor.
 *
 * @param sprite The dynamic sprite to be set.
 * @return The final instruction that will be send to the device hardware.
 */
u64_t set_dynamic_sprite(sprite_t sprite);

/**
 * @brief Sets the background color in the graphics processor.
 *
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 * @return The final instruction that will be send to the device hardware..
 */
u64_t set_background_color(u32_t R, u32_t G, u32_t B);

/**
 * @brief Sets a polygon in the graphics processor.
 *
 * @param polygon The polygon to be set.
 * @return The final instruction that will be send to the device hardware.
 */
u64_t set_polygon(polygon_t polygon);

void clean_sprite();

void clean_polygon();

void clean_background();

void set_game_sprites();

void draw_picture();
