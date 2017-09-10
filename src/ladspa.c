#include "ladspa.h"

#include <gnu-guitar/effects/echo.h>
#include <gnu-guitar/effects/effect.h>

#include <stdlib.h>

/** @brief Contains the data that is created
 * every time a ladspa handle is instantiated.
 * */
struct ladspa_data {
  /** @brief Contains the effect functions. */
  effect_t *effect;
  /** @brief Contains audio input and
   * output pointers and such. */
  data_block_t db;
  /** @brief The sample rate of the
   * audio. */
  unsigned long int sample_rate;
  /** @brief The number of echos. */
  float *echo_count;
};

static void echo_connect_port(LADSPA_Handle instance,
                              unsigned long int port,
                              LADSPA_Data *data_location) {
  struct ladspa_data *data;

  data = (struct ladspa_data *) instance;

  if (port == 0) {
    /* Input */
    data->db.data = data_location;
  } else if (port == 1) {
    /* Output */
    data->db.data_swap = data_location;
  } else if (port == 2) {
    /* Echo count */
    data->echo_count = data_location;
  } else if (port == 3) {
    /* Decay */
  }
}
/* echo descriptor */

extern const LADSPA_Descriptor echo_descriptor;

static LADSPA_Handle gnu_guitar_instantiate(const LADSPA_Descriptor *descriptor, unsigned long int sample_rate) {
  struct ladspa_data *data = NULL;
  if (descriptor == &echo_descriptor) {
    data = malloc(sizeof(*data));
    data->echo_count = NULL;
    data->sample_rate = sample_rate;
    data->db.len = 0;
    data->db.channels = 1;
    data->effect = echo_create();
    return data;
  } else {
    return NULL;
  }
}

const char *echo_port_names[] = {
  "Input",
  "Output",
  "Echo Count",
  NULL
};

LADSPA_PortDescriptor echo_port_descriptors[] = {
  LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT /* Input */,
  LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT /* Output */,
  LADSPA_PORT_CONTROL /* Echo Count */,
};

LADSPA_PortRangeHint echo_port_range_hints[] = {
  /* Input */ {
    LADSPA_HINT_DEFAULT_NONE,
    0 /* LowerBound */,
    0 /* UpperBound */
  },
  /* Output */{
    LADSPA_HINT_DEFAULT_NONE,
    0 /* LowerBound */,
    0 /* UpperBound */
  },
  /* Echo Count */ {
    LADSPA_HINT_INTEGER | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE,
    0 /* LowerBound */,
    4 /* UpperBound */
  }
};

static void gnu_guitar_activate(LADSPA_Handle instance) {
  (void) instance;
}

static void gnu_guitar_run(LADSPA_Handle instance, unsigned long int sample_count) {

  struct ladspa_data *data;
  struct echo_params *echo_params;

  data = (struct ladspa_data *) instance;
  if (data == NULL)
    return;

  if (data->echo_count != NULL) {
    echo_params = (struct echo_params *) data->effect->params;
    printf("before: %u\n", echo_params->echoes);
    echo_params->echoes = (int) *data->echo_count;
    printf("after: %u\n", echo_params->echoes);
  }

  data->db.len = sample_count / data->db.channels;

  data->effect->proc_filter(data->effect, &data->db);
}

const LADSPA_Descriptor echo_descriptor = {
  0 /* UniqueID */,
  "GNU Guitar Echo" /* Label */,
  0 /* Properties */,
  "GNU Guitar Echo" /* Name */,
  "GNU Guitar" /* Maker */,
  "GPLv2" /* Copyright */,
  3 /* PortCount */,
  echo_port_descriptors /* PortDescriptors */,
  echo_port_names /* PortNames */,
  echo_port_range_hints /* PortRangeHints */,
  NULL /* ImplementationData */,
  gnu_guitar_instantiate /* instantiate */,
  echo_connect_port /* connect_port */,
  gnu_guitar_activate /* activate */,
  gnu_guitar_run /* run */,
  NULL /* run_adding */,
  NULL /* run_adding_gain */,
  NULL /* deactivate */,
  NULL /* cleanup */
};

const LADSPA_Descriptor *ladspa_descriptor(unsigned long int index) {
  if (index == 0)
    return &echo_descriptor;
  else
    return NULL;
}

