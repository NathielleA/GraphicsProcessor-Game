#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "GameVisualization.h"
#include "GraphSync.h"

#define KEY_BASE 0X0
#define LW_BRIDGE_SPAN 0x00005000
#define LW_BRIDGE_BASE 0xff200000

#define MOUSE_DEVICE_PATH "/dev/input/mice"

#define START 0
#define GAME 1
#define PAUSE 2
#define RESTART 3
#define GAMEOVER 4
#define VICTORY 5

volatile i8_t state_game;
// u8_t game_level;
pthread_mutex_t mutex;
u8_t previous_state;
u32_t counter_state;
sprite_t cursor;

u8_t frogs = 5;
u8_t life = 5;

sprite_t car_1_1;
sprite_t car_1_2;
sprite_t car_1_3;
sprite_t car_2_1;
sprite_t car_2_2;
sprite_t car_2_3;
sprite_t truckfront_3_1;
sprite_t truckback_3_1;
sprite_t truckfront_3_2;
sprite_t truckback_3_2;
sprite_t truckfront_3_3;
sprite_t truckback_3_3;
sprite_t car_4_1;
sprite_t car_4_2;
sprite_t car_4_3;
// sprite_t tree_back_1;
// sprite_t tree_middle_1;
// sprite_t tree_front_1;
// sprite_t tree_back_2;
// sprite_t tree_middle_2;
// sprite_t tree_front_2;
// sprite_t lilypad_1_1;
// sprite_t lilypad_1_2;
// sprite_t lilypad_2_1;
// sprite_t lilypad_2_2;

void change_state(volatile i32_t *KEY_ptr, volatile i8_t edge_capture) {
  switch (state_game) {
    case START:

      if (*KEY_ptr == 0b0111 && edge_capture) {  // primeiro botão da placa, tecla pressionada
        state_game = GAME;
        previous_state = START;
      } else if (*KEY_ptr == 0b1111) {
        state_game = START;
        previous_state = START;
      }
      break;

    case GAME:
      if (*KEY_ptr == 0b0111 && edge_capture) {  // primeiro botão da placa, tecla pressionada
        state_game = PAUSE;
        previous_state = GAME;
      } else if (*KEY_ptr == 0b1111) {
        state_game = GAME;
        previous_state = GAME;
      }
      break;

    case PAUSE:

      if (*KEY_ptr == 0b0111 && edge_capture) {  // primeiro botão da placa, tecla pressionada
        state_game = GAME;
        previous_state = PAUSE;
      } else if (*KEY_ptr == 0b1011 && edge_capture) {  // segundo botão da placa, tecla pressionada
        state_game = START;
        previous_state = PAUSE;
      } else if (*KEY_ptr == 0b1101 && edge_capture) {  // terceiro botão da placa, tecla pressionada
        state_game = START;
        previous_state = PAUSE;
      } else if (*KEY_ptr == 0b1111) {
        state_game = PAUSE;
        previous_state = PAUSE;
      }
      break;

      // case RESTART:
      //   previous_state = RESTART;

      //   state_game = GAME;  // terceiro botão da placa, tecla pressionada
      //   system("clear");

      //   break;

    default:
      break;
  }
}

void *key_thread(void *args) {
  static i32_t fd_map = -1;

  fd_map = open("/dev/mem", (O_RDWR | O_SYNC));
  if (fd_map == -1) {
    perror("Error maping memory");
    exit(EXIT_FAILURE);
  }

  void *LW_virtual = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd_map, LW_BRIDGE_BASE);
  if (LW_virtual == MAP_FAILED) {
    perror("Error maping memory");
    exit(EXIT_FAILURE);
  }

  volatile i32_t *KEY_ptr = (i32_t *)(LW_virtual + KEY_BASE);
  volatile i8_t edge_capture = 1;
  i32_t previous_key_state = 0b1111;
  i32_t current_key_state;

  while (1) {
    current_key_state = *KEY_ptr;
    if (previous_key_state == current_key_state) {
      edge_capture = 0;
    } else {
      edge_capture = 1;
    }

    previous_key_state = current_key_state;

    change_state(KEY_ptr, edge_capture);

    if (previous_state == state_game) {
      counter_state = 1;
    } else {
      counter_state = 0;
    }

    // printf("COUNTER: %d\n", counter_state);
  }

  if (munmap(LW_virtual, LW_BRIDGE_SPAN) == -1) {
    perror("Error ending ");
    exit(EXIT_FAILURE);
  }
  if (close(fd_map) == -1) {
    perror("Error ending device");
    exit(EXIT_FAILURE);
  }

  pthread_exit(args);
}

void *mouse_thread() {
  static i64_t fd_mouse = -1;

  fd_mouse = open(MOUSE_DEVICE_PATH, O_RDONLY);
  if (fd_mouse == -1) {
    perror("Error opening device");
    exit(EXIT_FAILURE);
  }

  struct input_event ev_mouse;

  // cursor.ativo = 1;
  cursor.data_register = 30;
  cursor.offset = 1;
  cursor.coord_x = 320;
  cursor.coord_y = 450;
  cursor.step_x = 1;
  cursor.step_y = 1;
  cursor.speed = 40;
  cursor.collision = 0;

  // Abrindo o dispositivo do mouse (adapte o caminho conforme necessário)
  fd_mouse = open("/dev/input/event0", O_RDONLY);
  if (fd_mouse == -1) {
    perror("Erro ao abrir o dispositivo de entrada");
    exit(EXIT_FAILURE);
  }

  while (1) {
    ssize_t bytes = read(fd_mouse, &ev_mouse, sizeof(ev_mouse));
    if (bytes < sizeof(struct input_event)) {
      perror("Erro ao ler evento do mouse");
      exit(EXIT_FAILURE);
    }

    if (state_game == GAME) {
      cursor.ativo = 1;

      if (ev_mouse.type == EV_REL && ev_mouse.code == REL_X && cursor.coord_y > 200) {
        cursor.coord_x += ev_mouse.value;
      }

      if (ev_mouse.type == EV_KEY && ev_mouse.code == BTN_LEFT) {
        cursor.coord_y -= 10;
      } else if (ev_mouse.type == EV_KEY && ev_mouse.code == BTN_RIGHT) {
        cursor.coord_y += 10;
      }
      printf("%d\n", cursor.coord_y);

      // pthread_mutex_lock(&mutex);
      // set_dynamic_sprite(cursor);
      // pthread_mutex_unlock(&mutex);

    } else {
      cursor.ativo = 0;
      // pthread_mutex_lock(&mutex);
      // clean_sprite();
      // set_dynamic_sprite(cursor);
      // pthread_mutex_unlock(&mutex);
    }
  }

  pthread_exit(NULL);
}

void *visul_thread() {
  u32_t counter = 0;

  u16_t beginning = 0;  // Coordinate x -> número para o começo da tela
  u16_t end = 600;      // Coordinate x -> número para o fim da tela

  /* ---------- ROAD SPRITES ---------- */

  /* FIRST ROAD (RED CARS, LEFT DIRECTION) */
  u16_t first_road = 280;  // Coodinate y -> número para a coordenada da primeira pista

  // coord_x, coord_y, direction, offset, data_register, step_x, step_y, speed, ativo, collision
  car_1_1.coord_x = end;
  car_1_1.coord_y = first_road;
  car_1_1.direction = 0;
  car_1_1.offset = 2;
  car_1_1.data_register = 1;
  car_1_1.step_x = 1;
  car_1_1.step_y = 1;
  car_1_1.speed = 2;
  car_1_1.ativo = 1;
  car_1_1.collision = 0;

  car_1_2.coord_x = end + 60;
  car_1_2.coord_y = first_road;
  car_1_2.direction = 0;
  car_1_2.offset = 2;
  car_1_2.data_register = 2;
  car_1_2.step_x = 1;
  car_1_2.step_y = 1;
  car_1_2.speed = 2;
  car_1_2.ativo = 1;
  car_1_2.collision = 0;

  car_1_3.coord_x = end + 120;
  car_1_3.coord_y = first_road;
  car_1_3.direction = 0;
  car_1_3.offset = 2;
  car_1_3.data_register = 3;
  car_1_3.step_x = 1;
  car_1_3.step_y = 1;
  car_1_3.speed = 2;
  car_1_3.ativo = 1;
  car_1_3.collision = 0;

  /* SECOND ROAD (YELLOW CARS, RIGHT DIRECTION) */
  u16_t second_road = first_road + 40;  // Coodinate y -> número para a coordenada da segunda pista

  car_2_1.coord_x = beginning;
  car_2_1.coord_y = second_road;
  car_2_1.direction = 1;
  car_2_1.offset = 3;
  car_2_1.data_register = 4;
  car_2_1.step_x = 1;
  car_2_1.step_y = 1;
  car_2_1.speed = 4;
  car_2_1.ativo = 1;
  car_2_1.collision = 0;

  car_2_2.coord_x = beginning + 30;
  car_2_2.coord_y = second_road;
  car_2_2.direction = 1;
  car_2_2.offset = 3;
  car_2_2.data_register = 5;
  car_2_2.step_x = 1;
  car_2_2.step_y = 1;
  car_2_2.speed = 4;
  car_2_2.ativo = 1;
  car_2_2.collision = 0;

  car_2_3.coord_x = beginning + 60;
  car_2_3.coord_y = second_road;
  car_2_3.direction = 1;
  car_2_3.offset = 3;
  car_2_3.data_register = 6;
  car_2_3.step_x = 1;
  car_2_3.step_y = 1;
  car_2_3.speed = 4;
  car_2_3.ativo = 1;
  car_2_3.collision = 0;

  /* THRID ROAD (TRUCKS, LEFT DIRECTION) */
  u16_t third_road = second_road + 40;  // Coodinate y -> número para a coordenada da terceira pista

  truckfront_3_1.coord_x = end;
  truckfront_3_1.coord_y = third_road;
  truckfront_3_1.direction = 0;
  truckfront_3_1.offset = 6;
  truckfront_3_1.data_register = 7;
  truckfront_3_1.step_x = 1;
  truckfront_3_1.step_y = 1;
  truckfront_3_1.speed = 3;
  truckfront_3_1.ativo = 1;
  truckfront_3_1.collision = 0;

  truckback_3_1.coord_x = end + 20;
  truckback_3_1.coord_y = third_road;
  truckback_3_1.direction = 0;
  truckback_3_1.offset = 7;
  truckback_3_1.data_register = 8;
  truckback_3_1.step_x = 1;
  truckback_3_1.step_y = 1;
  truckback_3_1.speed = 3;
  truckback_3_1.ativo = 1;
  truckback_3_1.collision = 0;

  //------------------------------------

  truckfront_3_2.coord_x = end + 100;
  truckfront_3_2.coord_y = third_road;
  truckfront_3_2.direction = 0;
  truckfront_3_2.offset = 6;
  truckfront_3_2.data_register = 9;
  truckfront_3_2.step_x = 1;
  truckfront_3_2.step_y = 1;
  truckfront_3_2.speed = 3;
  truckfront_3_2.ativo = 1;
  truckfront_3_2.collision = 0;

  truckback_3_2.coord_x = end + 120;
  truckback_3_2.coord_y = third_road;
  truckback_3_2.direction = 0;
  truckback_3_2.offset = 7;
  truckback_3_2.data_register = 10;
  truckback_3_2.step_x = 1;
  truckback_3_2.step_y = 1;
  truckback_3_2.speed = 3;
  truckback_3_2.ativo = 1;
  truckback_3_2.collision = 0;

  //------------------------------------

  sprite_t truckfront_3_3 = {end + 200, third_road, 0, 6, 11, 1, 1, 3, 1, 0};
  truckfront_3_3.coord_x = end + 200;
  truckfront_3_3.coord_y = third_road;
  truckfront_3_3.direction = 0;
  truckfront_3_3.offset = 6;
  truckfront_3_3.data_register = 11;
  truckfront_3_3.step_x = 1;
  truckfront_3_3.step_y = 1;
  truckfront_3_3.speed = 3;
  truckfront_3_3.ativo = 1;
  truckfront_3_3.collision = 0;

  truckback_3_3.coord_x = end + 220;
  truckback_3_3.coord_y = third_road;
  truckback_3_3.direction = 0;
  truckback_3_3.offset = 7;
  truckback_3_3.data_register = 12;
  truckback_3_3.step_x = 1;
  truckback_3_3.step_y = 1;
  truckback_3_3.speed = 3;
  truckback_3_3.ativo = 1;
  truckback_3_3.collision = 0;

  /* FOURTH ROAD (GREEN CARS, RIGHT DIRECTION) */
  u16_t fourth_road = third_road + 40;  // Coodinate y -> número para a coordenada da quarta pista

  car_4_1.coord_x = beginning;
  car_4_1.coord_y = fourth_road;
  car_4_1.direction = 1;
  car_4_1.offset = 3;
  car_4_1.data_register = 13;
  car_4_1.step_x = 1;
  car_4_1.step_y = 1;
  car_4_1.speed = 1;
  car_4_1.ativo = 1;
  car_4_1.collision = 0;

  car_4_2.coord_x = beginning + 30;
  car_4_2.coord_y = fourth_road;
  car_4_2.direction = 1;
  car_4_2.offset = 3;
  car_4_2.data_register = 14;
  car_4_2.step_x = 1;
  car_4_2.step_y = 1;
  car_4_2.speed = 1;
  car_4_2.ativo = 1;
  car_4_2.collision = 0;

  car_4_3.coord_x = beginning + 60;
  car_4_3.coord_y = fourth_road;
  car_4_3.direction = 1;
  car_4_3.offset = 3;
  car_4_3.data_register = 15;
  car_4_3.step_x = 1;
  car_4_3.step_y = 1;
  car_4_3.speed = 1;
  car_4_3.ativo = 1;
  car_4_3.collision = 0;

  /* ---------- WATER SPRITES ---------- */

  /* FIRST WATER WAY (TRUNK TREE, LEFT DIRECTION) */
  u16_t first_waterway = 60;  // Coodinate y -> número para a coordenada da primeira pista na água

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
  u16_t fourth_waterway = third_waterway + 40;  // Coodinate y -> número para a coordenada da quarta pista na água

  sprite_t lilypad_2_1 = {beginning, fourth_waterway, 1, 4, 24, 1, 1, 9, 1, 0};
  sprite_t lilypad_2_2 = {beginning + 100, fourth_waterway, 1, 5, 25, 1, 1, 9, 1, 0};

  while (1) {
    /*---------- ROAD CONDITIONS ----------*/

    if (previous_state != GAME) {
      usleep(10);
    }

    if (state_game == GAME) {
      car_1_1.ativo = 1;
      car_1_2.ativo = 1;
      car_1_3.ativo = 1;
      car_2_1.ativo = 1;
      car_2_2.ativo = 1;
      car_2_3.ativo = 1;
      car_4_1.ativo = 1;
      car_4_2.ativo = 1;
      car_4_3.ativo = 1;
      truckback_3_1.ativo = 1;
      truckback_3_2.ativo = 1;
      truckback_3_3.ativo = 1;
      truckfront_3_1.ativo = 1;
      truckfront_3_2.ativo = 1;
      truckfront_3_3.ativo = 1;
      tree_back_1.ativo = 1;
      tree_back_2.ativo = 1;
      tree_front_1.ativo = 1;
      tree_front_2.ativo = 1;
      tree_middle_1.ativo = 1;
      tree_middle_2.ativo = 1;
      lilypad_1_1.ativo = 1;
      lilypad_1_2.ativo = 1;
      lilypad_2_1.ativo = 1;
      lilypad_2_2.ativo = 1;

      /* FIRST ROAD CONDITIONS */
      if (car_1_1.coord_x == beginning) {
        car_1_1.coord_x = end;
      } else {
        increase_coordinate_sprite(&car_1_1, counter);
      }

      if (car_1_2.coord_x == beginning) {
        car_1_2.coord_x = end + 60;
      } else {
        increase_coordinate_sprite(&car_1_2, counter);
      }

      if (car_1_3.coord_x == beginning) {
        car_1_3.coord_x = end + 120;
      } else {
        increase_coordinate_sprite(&car_1_3, counter);
      }

      /* SECOND ROAD CONDITIONS */
      if (car_2_1.coord_x == end) {
        car_2_1.coord_x = beginning;
      } else {
        increase_coordinate_sprite(&car_2_1, counter);
      }

      if (car_2_2.coord_x == end) {
        car_2_2.coord_x = beginning - 30;
      } else {
        increase_coordinate_sprite(&car_2_2, counter);
      }

      if (car_2_3.coord_x == end) {
        car_2_3.coord_x = beginning - 60;
      } else {
        increase_coordinate_sprite(&car_2_3, counter);
      }

      /* THIRD  ROAD CONDITIONS */
      if (truckfront_3_1.coord_x == beginning) {
        truckfront_3_1.coord_x = end;
        truckback_3_1.coord_x = end + 20;
      } else {
        increase_coordinate_sprite(&truckfront_3_1, counter);
        increase_coordinate_sprite(&truckback_3_1, counter);
      }

      if (truckfront_3_2.coord_x == beginning) {
        truckfront_3_2.coord_x = end + 100;
        truckback_3_2.coord_x = end + 120;
      } else {
        increase_coordinate_sprite(&truckfront_3_2, counter);
        increase_coordinate_sprite(&truckback_3_2, counter);
      }

      if (truckfront_3_3.coord_x == beginning) {
        truckfront_3_3.coord_x = end + 200;
        truckback_3_3.coord_x = end + 220;
      } else {
        increase_coordinate_sprite(&truckfront_3_3, counter);
        increase_coordinate_sprite(&truckback_3_3, counter);
      }

      /* FOURTH ROAD CONDITIONS */
      if (car_4_1.coord_x == end) {
        car_4_1.coord_x = beginning;
      } else {
        increase_coordinate_sprite(&car_4_1, counter);
      }

      if (car_4_2.coord_x == end) {
        car_4_2.coord_x = beginning - 30;
      } else {
        increase_coordinate_sprite(&car_4_2, counter);
      }

      if (car_4_3.coord_x == end) {
        car_4_3.coord_x = beginning - 60;
      } else {
        increase_coordinate_sprite(&car_4_3, counter);
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
      }

      /* SECOND WATER WAY CONDITIONS */
      if (lilypad_1_1.coord_x == end) {
        lilypad_1_1.coord_x = beginning;
        lilypad_1_2.coord_x = beginning + 20;
      } else {
        increase_coordinate_sprite(&lilypad_1_1, counter);
        increase_coordinate_sprite(&lilypad_1_2, counter);
      }

      /* THIRD WATER WAY CONDITIONS */
      if (tree_back_2.coord_x == beginning) {
        tree_back_2.coord_x = end;
        tree_middle_2.coord_x = end + 20;
        tree_front_2.coord_x = end + 40;
      } else {
        increase_coordinate_sprite(&tree_back_2, counter);
        increase_coordinate_sprite(&tree_middle_2, counter);
        increase_coordinate_sprite(&tree_front_2, counter);
      }

      /* FOURTH WATER WAY CONDITIONS */
      if (lilypad_2_1.coord_x == end) {
        lilypad_2_1.coord_x = beginning;
        lilypad_2_2.coord_x = beginning + 20;
      } else {
        increase_coordinate_sprite(&lilypad_2_1, counter);
        increase_coordinate_sprite(&lilypad_2_2, counter);
      }

      counter += 1;
    } else {
      car_1_1.ativo = 0;
      car_1_2.ativo = 0;
      car_1_3.ativo = 0;
      car_2_1.ativo = 0;
      car_2_2.ativo = 0;
      car_2_3.ativo = 0;
      car_4_1.ativo = 0;
      car_4_2.ativo = 0;
      car_4_3.ativo = 0;
      truckback_3_1.ativo = 0;
      truckback_3_2.ativo = 0;
      truckback_3_3.ativo = 0;
      truckfront_3_1.ativo = 0;
      truckfront_3_2.ativo = 0;
      truckfront_3_3.ativo = 0;
      tree_back_1.ativo = 0;
      tree_back_2.ativo = 0;
      tree_front_1.ativo = 0;
      tree_front_2.ativo = 0;
      tree_middle_1.ativo = 0;
      tree_middle_2.ativo = 0;
      lilypad_1_1.ativo = 0;
      lilypad_1_2.ativo = 0;
      lilypad_2_1.ativo = 0;
      lilypad_2_2.ativo = 0;
    }

    /*Enviando Instruções*/
    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(car_1_1);
    set_dynamic_sprite(car_1_2);
    set_dynamic_sprite(car_1_3);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(car_2_1);
    set_dynamic_sprite(car_2_2);
    set_dynamic_sprite(car_2_3);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(truckfront_3_1);
    set_dynamic_sprite(truckback_3_1);
    set_dynamic_sprite(truckfront_3_2);
    set_dynamic_sprite(truckback_3_2);
    set_dynamic_sprite(truckfront_3_3);
    set_dynamic_sprite(truckback_3_3);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(car_4_1);
    set_dynamic_sprite(car_4_2);
    set_dynamic_sprite(car_4_3);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(tree_back_1);
    set_dynamic_sprite(tree_middle_1);
    set_dynamic_sprite(tree_front_1);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(tree_back_2);
    set_dynamic_sprite(tree_middle_2);
    set_dynamic_sprite(tree_front_2);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(lilypad_1_1);
    set_dynamic_sprite(lilypad_1_2);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    set_dynamic_sprite(lilypad_2_1);
    set_dynamic_sprite(lilypad_2_2);
    pthread_mutex_unlock(&mutex);
  }

  // moving_sprites(active_bit_movement);
  pthread_exit(NULL);
}

void *collision_thread() {
  while (1) {
    collision(&cursor, &car_1_1);
    collision(&cursor, &car_1_2);
    collision(&cursor, &car_1_3);
    collision(&cursor, &car_2_1);
    collision(&cursor, &car_2_2);
    collision(&cursor, &car_2_3);

    collision(&cursor, &truckfront_3_1);
    collision(&cursor, &truckback_3_1);
    collision(&cursor, &truckfront_3_2);
    collision(&cursor, &truckback_3_2);
    collision(&cursor, &truckfront_3_3);
    collision(&cursor, &truckback_3_3);

    collision(&cursor, &car_4_1);
    collision(&cursor, &car_4_2);
    collision(&cursor, &car_4_3);
  }

  pthread_exit(NULL);
}

int main(void) {
  open_data();

  set_game_sprites();

  state_game = START;

  pthread_mutex_init(&mutex, NULL);

  previous_state = GAME;

  pthread_t thread_key_id;
  pthread_t thread_mouse_id;
  pthread_t thread_visu_id;
  pthread_t thread_coll_id;

  if (pthread_create(&thread_key_id, NULL, key_thread, NULL) != 0) {
    perror("Error creating thread");
    return 1;
  }

  if (pthread_create(&thread_mouse_id, NULL, mouse_thread, NULL) != 0) {
    perror("Error creating thread");
    return 1;
  }

  if (pthread_create(&thread_visu_id, NULL, visul_thread, NULL) != 0) {
    perror("Error creating thread");
    return 1;
  }

  if (pthread_create(&thread_coll_id, NULL, collision_thread, NULL) != 0) {
    perror("Error creating thread");
    return 1;
  }

  pthread_mutex_lock(&mutex);
  clean_sprite();
  clean_polygon();
  clean_background();
  init_screen();
  pthread_mutex_unlock(&mutex);

  counter_state = 0;
  // game_level = 0;
  u16_t counter_river = 0;

  while (1) {
    switch (state_game) {
      case START:

        if (previous_state != START) {
          pthread_mutex_lock(&mutex);
          clean_sprite();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          clean_polygon();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          clean_background();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          init_screen();
          pthread_mutex_unlock(&mutex);
        }

        break;
      case GAME:

        if (previous_state != GAME) {
          pthread_mutex_lock(&mutex);
          clean_sprite();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          clean_polygon();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          clean_background();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          game_screen();
          pthread_mutex_unlock(&mutex);
        }

        if (cursor.coord_y <= 200 && cursor.coord_y > 150) {
          cursor.direction = 1;
          counter_river += 1;
          increase_coordinate_sprite(&cursor, counter_river);
        } else if (cursor.coord_y <= 150 && cursor.coord_y > 110) {
          cursor.direction = 0;
          counter_river += 1;
          increase_coordinate_sprite(&cursor, counter_river);
        } else if (cursor.coord_y <= 110 && cursor.coord_y > 70) {
          cursor.direction = 1;
          counter_river += 1;
          increase_coordinate_sprite(&cursor, counter_river);
        } else if (cursor.coord_y <= 70 && cursor.coord_y > 20) {
          cursor.direction = 0;
          counter_river += 1;
          increase_coordinate_sprite(&cursor, counter_river);
        } else if (cursor.coord_y <= 20) {
          frogs -= 1;
          cursor.coord_x = 320;
          cursor.coord_y = 450;
        }

        if (frogs == 0) {
          state_game = VICTORY;
        }

        /*Verifica a colisão*/
        if (life) {
          if (cursor.collision == 1) {
            life -= 1;
            cursor.coord_x = 320;
            cursor.coord_y = 450;
          }
        } else {
          state_game = GAMEOVER;  // Lembrar de ir para restart
        }
        // if (cursor.collision == 1) {
        //   life -= 1;
        // } else if (cursor.collision == 1 && life <= 0) {
        //   state_game = GAMEOVER;
        // }

        pthread_mutex_lock(&mutex);
        set_dynamic_sprite(cursor);
        pthread_mutex_unlock(&mutex);

        break;
      case PAUSE:

        if (previous_state != PAUSE) {
          pthread_mutex_lock(&mutex);
          clean_sprite();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          clean_polygon();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          clean_background();
          pthread_mutex_unlock(&mutex);

          pthread_mutex_lock(&mutex);
          pause_screen();
          pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_lock(&mutex);
        clean_sprite();
        pthread_mutex_unlock(&mutex);

        break;
      case GAMEOVER:
        pthread_mutex_lock(&mutex);
        gameover_screen();
        pthread_mutex_unlock(&mutex);
        sleep(5);

        counter_state = 0;
        // cursor.coord_x = 320;
        // cursor.coord_y = 450;
        state_game = START;

        break;
      case VICTORY:
        pthread_mutex_lock(&mutex);
        victory_screen();
        pthread_mutex_unlock(&mutex);
        sleep(5);

        counter_state = 0;
        cursor.coord_x = 320;
        cursor.coord_y = 450;
        state_game = START;

        break;
      default:
        break;
    }
  }

  close_data();
  return 0;
}
