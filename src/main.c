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

  //clean_background();

  set_background_color(0, 4, 7);

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
  squares_1.G = 4;
  squares_1.B = 7;
  squares_1.data_register = 1;

  squares_1.ref_point_x = 50 + 60;
  squares_1.ref_point_y = 25;
  set_polygon(squares_1);

  polygon_t squares_2;
  squares_2.type = 0;
  squares_2.size = 2;
  squares_2.R = 0;
  squares_2.G = 4;
  squares_2.B = 7;
  squares_2.data_register = 2;

  squares_2.ref_point_x = 150 + 60;
  squares_2.ref_point_y = 25;
  set_polygon(squares_2);

  polygon_t squares_3;
  squares_3.type = 0;
  squares_3.size = 2;
  squares_3.R = 0;
  squares_3.G = 4;
  squares_3.B = 7;
  squares_3.data_register = 3;

  squares_3.ref_point_x = 250 + 60;
  squares_3.ref_point_y = 25;
  set_polygon(squares_3);

  polygon_t squares_4;
  squares_4.type = 0;
  squares_4.size = 2;
  squares_4.R = 0;
  squares_4.G = 4;
  squares_4.B = 7;
  squares_4.data_register = 4;

  squares_4.ref_point_x = 350 + 60;
  squares_4.ref_point_y = 25;
  set_polygon(squares_4);

  polygon_t squares_5;
  squares_5.type = 0;
  squares_5.size = 2;
  squares_5.R = 0;
  squares_5.G = 4;
  squares_5.B = 7;
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

  moving_cars();

  return 0;
}

void moving_cars() {

  /* FIRST ROAD (YELLOW CARS, RIGHT DIRECTION) */
  u16_t first_road = 500; //Coodinate y -> número aleatório para a coordenada da primeira pista
  u16_t beginning = 600; //Coordinate x -> número aleatório para o começo da tela da direita para a esquerda

  sprite_t car_1;
  car_1.coord_x = beginning;
  car_1.coord_y = first_road;
  car_1.direction = 0;
  car_1.offset = 3;        
  car_1.data_register = 1; 
  car_1.step_x = 1;
  car_1.step_y = 1;
  car_1.ativo = 1;

  sprite_t car_2;
  car_2.coord_x = beginning + 30;
  car_2.coord_y = first_road;
  car_2.direction = 0;
  car_2.offset = 3;        
  car_2.data_register = 2; 
  car_2.step_x = 1;
  car_2.step_y = 1;
  car_2.ativo = 1;

  sprite_t car_3;
  car_3.coord_x = beginning + 65;
  car_3.coord_y = first_road;
  car_3.direction = 0;
  car_3.offset = 3;        
  car_3.data_register = 3; 
  car_3.step_x = 1;
  car_3.step_y = 1;
  car_3.ativo = 1;

  while(1) {

    if (car_1.coord_x == 0) {
      car_1.coord_x = beginning;
    } else {
      increase_coordinate_sprite(car_1);
      set_dynamic_sprite(car_1);
    }
    
    if (car_2.coord_x == 0) {
      car_2.coord_x = beginning + 10;
    } else {
      increase_coordinate_sprite(car_2);
      set_dynamic_sprite(car_2);
    }
    
    if (car_3.coord_x == 0) {
      car_3.coord_x = beginning + 15;
    } else {
      increase_coordinate_sprite(car_3);
      set_dynamic_sprite(car_3);
    }
  }
}

void matrix() {

}
