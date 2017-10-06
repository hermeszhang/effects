#include <gnu-guitar/effects/echo.h>

#include "bench.h"

gnu_guitar_effect_t *gnu_guitar_create_test_effect(void) {
  return echo_create();
}

