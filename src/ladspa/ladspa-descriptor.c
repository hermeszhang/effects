#include <ladspa.h>

#include <stdlib.h>

extern const LADSPA_Descriptor echo_descriptor;
extern const LADSPA_Descriptor reverb_descriptor;
extern const LADSPA_Descriptor tubeamp_descriptor;
extern const LADSPA_Descriptor tremolo_descriptor;

const LADSPA_Descriptor *ladspa_descriptor(unsigned long int index) {
  if (index == 0)
    return &echo_descriptor;
  else if (index == 1)
    return &reverb_descriptor;
  else if (index == 2)
    return &tubeamp_descriptor;
  else if (index == 3)
    return &tremolo_descriptor;
  else
    return NULL;
}

