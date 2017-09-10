#include <gnu-guitar/effects/echo.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {

  float in[256];
  float out[256];
  struct gnu_guitar_packet packet;
  gnu_guitar_effect_t *effect;
  unsigned int i;

  packet.data = in;
  packet.data_swap = out;
  packet.len = 128;
  packet.channels = 2;

  effect = echo_create();
  if (effect == NULL)
    return EXIT_FAILURE;

  for (i = 0; i < 1000; i++) {
    gnu_guitar_effect_filter(effect, &packet);
  }

  return EXIT_SUCCESS;
}

