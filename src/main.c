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

  set_background_color(1, 1, 1);

  sprite_fixed_t frog;
  frog.ativo = 1;
  frog.coord_x = 100;
  frog.coord_y = 100;
  frog.data_register = 1;
  frog.offset = 0;

  set_fixed_sprite(frog);

  //-----------------------

  sprite_fixed_t frog_back;
  frog_back.ativo = 1;
  frog_back.coord_x = 200;
  frog_back.coord_y = 200;
  frog_back.data_register = 2;
  frog_back.offset = 1;

  set_fixed_sprite(frog_back);

  return 0;
}
