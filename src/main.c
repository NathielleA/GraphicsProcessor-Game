#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "GraphSync.h"

//-------------------------------------------------------------

int main(void) {
  set_game_sprites();

  // clean_background();
  game_screen();
  moving_sprites();

  return 0;
}

void game_screen() {
  set_background_color(0, 5, 6);

  /*FROG FAMILY HOUSES*/
  ground_block_t frog_houses;
  frog_houses.R = 0;
  frog_houses.G = 4;
  frog_houses.B = 1;

  u16_t fh = 0;
  for (fh; fh < 400; fh++) {
    frog_houses.address = fh;

    if (fh >= 320) {
      frog_houses.R = 0;
      frog_houses.G = 6;
      frog_houses.B = 1;
    }

    set_background_block(frog_houses);
  }

  polygon_t squares_1;
  squares_1.type = 0;
  squares_1.size = 2;
  squares_1.R = 0;
  squares_1.G = 5;
  squares_1.B = 6;
  squares_1.data_register = 1;

  squares_1.ref_point_x = 50 + 60;
  squares_1.ref_point_y = 25;
  set_polygon(squares_1);

  polygon_t squares_2;
  squares_2.type = 0;
  squares_2.size = 2;
  squares_2.R = 0;
  squares_2.G = 5;
  squares_2.B = 6;
  squares_2.data_register = 2;

  squares_2.ref_point_x = 150 + 60;
  squares_2.ref_point_y = 25;
  set_polygon(squares_2);

  polygon_t squares_3;
  squares_3.type = 0;
  squares_3.size = 2;
  squares_3.R = 0;
  squares_3.G = 5;
  squares_3.B = 6;
  squares_3.data_register = 3;

  squares_3.ref_point_x = 250 + 60;
  squares_3.ref_point_y = 25;
  set_polygon(squares_3);

  polygon_t squares_4;
  squares_4.type = 0;
  squares_4.size = 2;
  squares_4.R = 0;
  squares_4.G = 5;
  squares_4.B = 6;
  squares_4.data_register = 4;

  squares_4.ref_point_x = 350 + 60;
  squares_4.ref_point_y = 25;
  set_polygon(squares_4);

  polygon_t squares_5;
  squares_5.type = 0;
  squares_5.size = 2;
  squares_5.R = 0;
  squares_5.G = 5;
  squares_5.B = 6;
  squares_5.data_register = 5;

  squares_5.ref_point_x = 450 + 60;
  squares_5.ref_point_y = 25;
  set_polygon(squares_5);

  /* STREETS */
  ground_block_t streets;

  u16_t st_b = 2240;
  u16_t i = 0;
  for (st_b; st_b < 2720; st_b++) {
    streets.address = st_b;

    if ((streets.address % 2) == 0) {
      streets.R = 2;
      streets.G = 0;
      streets.B = 2;
    } else {
      streets.R = 4;
      streets.G = 0;
      streets.B = 4;
    }

    set_background_block(streets);
  }

  u16_t st_e = 4400;
  for (st_e; st_e < 4800; st_e++) {
    streets.address = st_e;

    if ((streets.address % 2) == 0) {
      streets.R = 2;
      streets.G = 0;
      streets.B = 2;
    } else {
      streets.R = 4;
      streets.G = 0;
      streets.B = 4;
    }

    set_background_block(streets);
  }

  /* CARS LANE*/
  ground_block_t cars_lane;
  cars_lane.R = 3;
  cars_lane.G = 3;
  cars_lane.B = 3;

  u16_t st_cars = 2720;
  for (st_cars; st_cars < 4400; st_cars++) {
    cars_lane.address = st_cars;
    set_background_block(cars_lane);
  }
}

void moving_sprites() {
  u32_t counter = 0;

  u16_t beginning = 0;  // Coordinate x -> número para o começo da tela
  u16_t end = 600;      // Coordinate x -> número para o fim da tela

  /* ---------- ROAD SPRITES ---------- */

  /* FIRST ROAD (RED CARS, LEFT DIRECTION) */
  u16_t first_road = 280;  // Coodinate y -> número para a coordenada da primeira pista

  // coord_x, coord_y, direction, offset, data_register, step_x, step_y, speed, ativo, collision
  sprite_t car_1_1 = {end, first_road, 0, 2, 1, 1, 1, 2, 1, 0};

  sprite_t car_1_2 = {end + 60, first_road, 0, 2, 2, 1, 1, 2, 1, 0};

  sprite_t car_1_3 = {end + 120, first_road, 0, 2, 3, 1, 1, 2, 1, 0};

  /* SECOND ROAD (YELLOW CARS, RIGHT DIRECTION) */
  u16_t second_road = first_road + 40;  // Coodinate y -> número para a coordenada da segunda pista

  sprite_t car_2_1 = {beginning, second_road, 1, 3, 4, 1, 1, 4, 1, 0};

  sprite_t car_2_2 = {beginning + 30, second_road, 1, 3, 5, 1, 1, 4, 1, 0};

  sprite_t car_2_3 = {beginning + 60, second_road, 1, 3, 6, 1, 1, 4, 1, 0};

  /* THRID ROAD (TRUCKS, LEFT DIRECTION) */
  u16_t third_road = second_road + 40;  // Coodinate y -> número para a coordenada da terceira pista

  sprite_t truckfront_3_1 = {end, third_road, 0, 6, 7, 1, 1, 3, 1, 0};
  sprite_t truckback_3_1 = {end + 20, third_road, 0, 7, 8, 1, 1, 3, 1, 0};

  sprite_t truckfront_3_2 = {end + 100, third_road, 0, 6, 9, 1, 1, 3, 1, 0};
  sprite_t truckback_3_2 = {end + 120, third_road, 0, 7, 10, 1, 1, 3, 1, 0};

  sprite_t truckfront_3_3 = {end + 200, third_road, 0, 6, 11, 1, 1, 3, 1, 0};
  sprite_t truckback_3_3 = {end + 220, third_road, 0, 7, 12, 1, 1, 3, 1, 0};

  /* FOURTH ROAD (GREEN CARS, RIGHT DIRECTION) */
  u16_t fourth_road = third_road + 40;  // Coodinate y -> número para a coordenada da quarta pista

  sprite_t car_4_1 = {beginning, fourth_road, 1, 3, 13, 1, 1, 1, 1, 0};

  sprite_t car_4_2 = {beginning + 30, fourth_road, 1, 3, 14, 1, 1, 1, 1, 0};

  sprite_t car_4_3 = {beginning + 60, fourth_road, 1, 3, 15, 1, 1, 1, 1, 0};

  /* ---------- WATER SPRITES ---------- */

  /* FIRST WATER WAY (TRUNK TREE, LEFT DIRECTION) */
  u16_t first_waterway = 10;  // Coodinate y -> número para a coordenada da primeira pista na água

  // coord_x, coord_y, direction, offset, data_register, step_x, step_y, speed, ativo, collision
  sprite_t tree_back_1 = {end, first_waterway, 0, 10, 16, 1, 1, 8, 1, 0};
  sprite_t tree_middle_1 = {end + 20, first_waterway, 0, 9, 17, 1, 1, 8, 1, 0};
  sprite_t tree_front_1 = {end + 40, first_waterway, 0, 8, 18, 1, 1, 8, 1, 0};

  /* SECOND WATER WAY (LILYPAD, RIGHT DIRECTION) */
  u16_t second_waterway = first_waterway + 40;  // Coodinate y -> número para a coordenada da segunda pista na água

  sprite_t lilypad_1_1 = {beginning, second_waterway, 1, 4, 19, 1, 1, 6, 1, 0};
  sprite_t lilypad_1_2 = {beginning + 100, second_waterway, 1, 5, 20, 1, 1, 6, 1, 0};

  /* THIRD WATER WAY (TRUNK TREE, LEFT DIRECTION) */
  u16_t third_waterway = second_waterway + 40;  // Coodinate y -> número para a coordenada da terceira pista na água

  sprite_t tree_back_2 = {end, third_waterway, 0, 10, 21, 1, 1, 10, 1, 0};
  sprite_t tree_middle_2 = {end + 20, third_waterway, 0, 9, 22, 1, 1, 10, 1, 0};
  sprite_t tree_front_2 = {end + 40, third_waterway, 0, 8, 23, 1, 1, 10, 1, 0};

  /* FOURTH WATER WAY (LILYPAD, RIGHT DIRECTION) */
  u16_t second_waterway = first_waterway + 40;  // Coodinate y -> número para a coordenada da quarta pista na água

  sprite_t lilypad_2_1 = {beginning, second_waterway, 1, 4, 24, 1, 1, 9, 1, 0};
  sprite_t lilypad_2_2 = {beginning + 100, second_waterway, 1, 5, 25, 1, 1, 9, 1, 0};

  while (1) {
    /*---------- ROAD CONDITIONS ----------*/

    /* FIRST ROAD CONDITIONS */
    if (car_1_1.coord_x == beginning) {
      car_1_1.coord_x = end;
    } else {
      increase_coordinate_sprite(&car_1_1, counter);
      set_dynamic_sprite(car_1_1);
    }

    if (car_1_2.coord_x == beginning) {
      car_1_2.coord_x = end + 60;
    } else {
      increase_coordinate_sprite(&car_1_2, counter);
      set_dynamic_sprite(car_1_2);
    }

    if (car_1_3.coord_x == beginning) {
      car_1_3.coord_x = end + 120;
    } else {
      increase_coordinate_sprite(&car_1_3, counter);
      set_dynamic_sprite(car_1_3);
    }

    /* SECOND ROAD CONDITIONS */
    if (car_2_1.coord_x == end) {
      car_2_1.coord_x = beginning;
    } else {
      increase_coordinate_sprite(&car_2_1, counter);
      set_dynamic_sprite(car_2_1);
    }

    if (car_2_2.coord_x == end) {
      car_2_2.coord_x = beginning - 30;
    } else {
      increase_coordinate_sprite(&car_2_2, counter);
      set_dynamic_sprite(car_2_2);
    }

    if (car_2_3.coord_x == end) {
      car_2_3.coord_x = beginning - 60;
    } else {
      increase_coordinate_sprite(&car_2_3, counter);
      set_dynamic_sprite(car_2_3);
    }

    /* THIRD  ROAD CONDITIONS */
    if (truckfront_3_1.coord_x == beginning) {
      truckfront_3_1.coord_x = end;
      truckback_3_1.coord_x = end + 20;
    } else {
      increase_coordinate_sprite(&truckfront_3_1, counter);
      increase_coordinate_sprite(&truckback_3_1, counter);
      set_dynamic_sprite(truckfront_3_1);
      set_dynamic_sprite(truckback_3_1);
    }

    if (truckfront_3_2.coord_x == beginning) {
      truckfront_3_2.coord_x = end + 100;
      truckback_3_2.coord_x = end + 120;
    } else {
      increase_coordinate_sprite(&truckfront_3_2, counter);
      increase_coordinate_sprite(&truckback_3_2, counter);
      set_dynamic_sprite(truckfront_3_2);
      set_dynamic_sprite(truckback_3_2);
    }

    if (truckfront_3_3.coord_x == beginning) {
      truckfront_3_3.coord_x = end + 200;
      truckback_3_3.coord_x = end + 220;
    } else {
      increase_coordinate_sprite(&truckfront_3_3, counter);
      increase_coordinate_sprite(&truckback_3_3, counter);
      set_dynamic_sprite(truckfront_3_3);
      set_dynamic_sprite(truckback_3_3);
    }

    /* FOURTH ROAD CONDITIONS */
    if (car_4_1.coord_x == end) {
      car_4_1.coord_x = beginning;
    } else {
      increase_coordinate_sprite(&car_4_1, counter);
      set_dynamic_sprite(car_4_1);
    }

    if (car_4_2.coord_x == end) {
      car_4_2.coord_x = beginning - 30;
    } else {
      increase_coordinate_sprite(&car_4_2, counter);
      set_dynamic_sprite(car_4_2);
    }

    if (car_4_3.coord_x == end) {
      car_4_3.coord_x = beginning - 60;
    } else {
      increase_coordinate_sprite(&car_4_3, counter);
      set_dynamic_sprite(car_4_3);
    }

    /*---------- WATER CONDITIONS ----------*/

    /* FIRST WATER WAY CONDITIONS */
    if (tree_back_1.coord_x == beginning) {
      tree_back_1.coord_x = end;
      tree_middle_1.coord_x = end + 20;
      tree_front_1.coord_x = end + 40;
    } else {
      increase_coordinate_sprite(&tree_back_1, counter);
      increase_coordinate_sprite(&tree_middle_1, counter);
      increase_coordinate_sprite(&tree_front_1, counter);
      set_dynamic_sprite(tree_back_1);
      set_dynamic_sprite(tree_middle_1);
      set_dynamic_sprite(tree_front_1);
    }

    /* SECOND WATER WAY CONDITIONS */
    if (lilypad_1_1.coord_x == end) {
      lilypad_1_1.coord_x = beginning;
      lilypad_1_2.coord_x = beginning + 20;
    } else {
      increase_coordinate_sprite(&lilypad_1_1, counter);
      increase_coordinate_sprite(&lilypad_1_2, counter);
      set_dynamic_sprite(lilypad_1_1);
      set_dynamic_sprite(lilypad_1_2);
    }

    /* THIRD WATER WAY CONDITIONS */
    if (tree_back_1.coord_x == beginning) {
      tree_back_1.coord_x = end;
      tree_middle_1.coord_x = end + 20;
      tree_front_1.coord_x = end + 40;
    } else {
      increase_coordinate_sprite(&tree_back_1, counter);
      increase_coordinate_sprite(&tree_middle_1, counter);
      increase_coordinate_sprite(&tree_front_1, counter);
      set_dynamic_sprite(tree_back_1);
      set_dynamic_sprite(tree_middle_1);
      set_dynamic_sprite(tree_front_1);
    }

    /* FOURTH WATER WAY CONDITIONS */
    if (lilypad_2_1.coord_x == end) {
      lilypad_2_1.coord_x = beginning;
      lilypad_2_2.coord_x = beginning + 20;
    } else {
      increase_coordinate_sprite(&lilypad_2_1, counter);
      increase_coordinate_sprite(&lilypad_2_2, counter);
      set_dynamic_sprite(lilypad_2_1);
      set_dynamic_sprite(lilypad_2_2);
    }

    counter += 1;
  }
  return;
}

void matrix() {}
