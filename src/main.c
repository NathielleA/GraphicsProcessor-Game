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

volatile i8_t state_game;
pthread_mutex_t mutex;
// i32_t previous_key_state;
// volatile i8_t edge_capture;
u8_t previous_state;
u32_t counter_state;
sprite_t cursor;

sprite_t car_1_1;
sprite_t car_1_2;
sprite_t car_1_3;
// sprite_t car_2_1;
// sprite_t car_2_2;
// sprite_t car_2_3;
// sprite_t truckfront_3_1;
// sprite_t truckback_3_1;
// sprite_t truckfront_3_2;
// sprite_t truckback_3_2;
// sprite_t car_1_1;
// sprite_t car_1_2;
// sprite_t car_1_3;
// sprite_t car_2_1;
// sprite_t car_2_2;
// sprite_t car_2_3;
// sprite_t truckfront_3_1;
// sprite_t truckback_3_1;
// sprite_t truckfront_3_2;
// sprite_t truckback_3_2;
// sprite_t truckfront_3_3;
// sprite_t truckback_3_3;
// sprite_t car_4_1;
// sprite_t car_4_2;
// sprite_t car_4_3;
// sprite_t tree_back_1;
// sprite_t tree_middle_1;
// sprite_t tree_front_1;
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
  // volatile i8_t *state_game = (volatile i8_t *)args;

  static i32_t fd_map = -1;

  // counter_state = 0;

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
      counter_state += 1;
    } else {
      counter_state = 0;
    }

    printf("COUNTER: %d\n", counter_state);
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

      if (ev_mouse.type == EV_REL && ev_mouse.code == REL_X) {
        cursor.coord_x += ev_mouse.value;
      }

      if (ev_mouse.type == EV_KEY && ev_mouse.code == BTN_LEFT) {
        cursor.coord_y -= 10;
      } else if (ev_mouse.type == EV_KEY && ev_mouse.code == BTN_RIGHT) {
        cursor.coord_y += 10;
      }
      pthread_mutex_lock(&mutex);
      set_dynamic_sprite(cursor);
      pthread_mutex_unlock(&mutex);
    } else {
      cursor.ativo = 0;
      pthread_mutex_lock(&mutex);
      set_dynamic_sprite(cursor);
      pthread_mutex_unlock(&mutex);
    }

    // if (state_game == GAME && ev_mouse.type == EV_REL && ev_mouse.code == REL_X) {
    //   cursor.coord_x += ev_mouse.value;
    //   set_dynamic_sprite(cursor);
    // }

    // if (state_game == GAME && ev_mouse.type == EV_KEY && ev_mouse.code == BTN_LEFT) {
    //   cursor.coord_y -= 10;
    //   set_dynamic_sprite(cursor);
    // } else if (state_game == GAME && ev_mouse.type == EV_KEY && ev_mouse.code == BTN_RIGHT) {
    //   cursor.coord_y += 10;
    //   set_dynamic_sprite(cursor);
    // }

    // set_dynamic_sprite(cursor);
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

  sprite_t game_sprites[] = {car_1_1,       car_1_2,       car_1_3,        car_2_1,        car_2_2,
                             car_2_3,       car_4_1,       car_4_2,        car_4_3,        truckback_3_1,
                             truckback_3_2, truckback_3_3, truckfront_3_1, truckfront_3_2, truckfront_3_3,
                             tree_back_1,   tree_back_2,   tree_front_1,   tree_front_2,   tree_middle_1,
                             tree_middle_2, lilypad_1_1,   lilypad_1_2,    lilypad_2_1,    lilypad_2_2};

  u32_t i;

  while (1) {
    /*---------- ROAD CONDITIONS ----------*/

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
    set_dynamic_sprite(car_2_1);
    set_dynamic_sprite(car_2_2);
    set_dynamic_sprite(car_2_3);
    set_dynamic_sprite(truckfront_3_1);
    set_dynamic_sprite(truckback_3_1);
    set_dynamic_sprite(truckfront_3_2);
    set_dynamic_sprite(truckback_3_2);
    set_dynamic_sprite(car_1_1);
    set_dynamic_sprite(car_1_2);
    set_dynamic_sprite(car_1_3);
    set_dynamic_sprite(car_2_1);
    set_dynamic_sprite(car_2_2);
    set_dynamic_sprite(car_2_3);
    set_dynamic_sprite(truckfront_3_1);
    set_dynamic_sprite(truckback_3_1);
    set_dynamic_sprite(truckfront_3_2);
    set_dynamic_sprite(truckback_3_2);
    set_dynamic_sprite(truckfront_3_3);
    set_dynamic_sprite(truckback_3_3);
    set_dynamic_sprite(car_4_1);
    set_dynamic_sprite(car_4_2);
    set_dynamic_sprite(car_4_3);
    set_dynamic_sprite(tree_back_1);
    set_dynamic_sprite(tree_middle_1);
    set_dynamic_sprite(tree_front_1);
    set_dynamic_sprite(lilypad_1_1);
    set_dynamic_sprite(lilypad_1_2);
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
  pthread_mutex_unlock(&mutex);

  pthread_mutex_lock(&mutex);
  clean_polygon();
  pthread_mutex_unlock(&mutex);

  pthread_mutex_lock(&mutex);
  clean_background();
  pthread_mutex_unlock(&mutex);

  counter_state = 0;

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

        /*Verifica a colisão*/
        if (cursor.collision == 1) {
          gameover_screen();
          sleep(1);
          state_game = START;
          counter_state = 0;
          cursor.coord_x = 320;
          cursor.coord_y = 450;
        }

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

        break;

      default:
        break;
    }
  }

  close_data();
  return 0;
}