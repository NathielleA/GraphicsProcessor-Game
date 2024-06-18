#ifndef AUX_H_
#define AUX_H_

#include "types.h"

void reverse(char str[], u32_t length);

char* itoa(u32_t num, char* str, u32_t base);

int clamp(int value, int min, int max)

#endif  // !AUX_H_