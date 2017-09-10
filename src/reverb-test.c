#include <gnu-guitar/effects/reverb.h>

#include "test.h"

gnu_guitar_effect_t *gnu_guitar_create_test_effect(void) {
  return reverb_create();
}

