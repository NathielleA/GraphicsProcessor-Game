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
  set_game_sprite();

  sprite_fixed_t frog;
  frog.ativo = 1;
  frog.coord_x = 100;
  frog.coord_y = 100;
  frog.data_register = 1;
  frog.offset = 0;

  return 0;
}
