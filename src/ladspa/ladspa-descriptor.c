#include <ladspa.h>

#include <stdlib.h>

extern const LADSPA_Descriptor echo_descriptor;

const LADSPA_Descriptor *ladspa_descriptor(unsigned long int index) {
  if (index == 0)
    return &echo_descriptor;
  else
    return NULL;
}

