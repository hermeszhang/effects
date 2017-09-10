#include "test.h"

#include <gnu-guitar/effects/tubeamp.h>

gnu_guitar_effect_t *gnu_guitar_create_test_effect(void) {
  return tubeamp_create();
}

