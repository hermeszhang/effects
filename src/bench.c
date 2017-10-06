#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bench.h"

#ifdef __unix__
#include <time.h>
#endif

#ifdef __unix__
struct timespec time_difference(const struct timespec *start, const struct timespec *end)
{
  struct timespec tmp;
	if ((end->tv_nsec - start->tv_nsec) < 0) {
		tmp.tv_sec = end->tv_sec - start->tv_sec - 1;
		tmp.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
	} else {
		tmp.tv_sec = end->tv_sec - start->tv_sec;
		tmp.tv_nsec = end->tv_nsec - start->tv_nsec;
	}
	return tmp;
}
#endif

int main(int argc, const char **argv) {

  int i;
  unsigned int j;
  unsigned long int rate = 48000;
  gnu_guitar_effect_t *effect;
  struct gnu_guitar_packet packet;
  float in[256];
  float out[256];

#ifdef __unix__
  int err;
  struct timespec start_time;
  struct timespec end_time;
  struct timespec diff_time;
#endif

  memset(in, 0, sizeof(in));

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

#ifdef __unix__
  err = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
  if (err != 0) {
    perror("Failed to get start time");
    return EXIT_FAILURE;
  }
#endif

  for (j = 0; j < (rate / packet.len); j++) {
    gnu_guitar_effect_filter(effect, &packet);
  }

#ifdef __unix__
  err = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
  if (err != 0) {
    perror("Failed to get end time");
    return EXIT_FAILURE;
  }
  diff_time = time_difference(&start_time, &end_time);
  printf("Total latency: %lu.%09lu seconds.\n",
         diff_time.tv_sec,
         diff_time.tv_nsec);
#endif

  gnu_guitar_effect_free(effect);

  return EXIT_SUCCESS;
}

