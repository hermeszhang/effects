#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

int main(int argc, const char **argv) {

  int i;
  unsigned int j;
  unsigned long int rate = 48000;
  gnu_guitar_effect_t *effect;
  struct gnu_guitar_packet packet;
  float in[256];
  float out[256];

  for (i = 1; i < argc; i++) {
    if ((strcmp(argv[i], "-h") == 0)
     || (strcmp(argv[i], "--help") == 0)) {
      printf("Usage: %s [options]\n", argv[0]);
      printf("\n");
      printf("Options:\n");
      printf("-r, --rate : Specify sample rate.\n");
      printf("-h, --help : Specify sample rate.\n");
      return EXIT_FAILURE;
    } else if ((strcmp(argv[i], "-r") == 0)
            || (strcmp(argv[i], "--rate") == 0)) {
      if ((i + 1) >= argc) {
        fprintf(stderr, "Missing rate parameter.\n");
        return EXIT_FAILURE;
      } else if (sscanf(argv[i + 1], "%lu", &rate) != 1) {
        fprintf(stderr, "Failed to parse sample rate '%s'.\n", argv[i + 1]);
        return EXIT_FAILURE;
      } else {
        i++;
      }
    }
  }

  packet.data = in;
  packet.data_swap = out;
  packet.channels = 2;
  packet.len = 128;

  effect = gnu_guitar_create_test_effect();
  if (effect == NULL) {
    fprintf(stderr, "Failed to create test effect.\n");
    return EXIT_FAILURE;
  }

  for (j = 0; j < (rate / packet.len); j++) {
    gnu_guitar_effect_filter(effect, &packet);
  }

  gnu_guitar_effect_free(effect);

  return EXIT_SUCCESS;
}

