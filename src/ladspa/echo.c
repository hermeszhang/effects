#include <gnu-guitar/effects/echo.h>
#include <gnu-guitar/effects/effect.h>

#include <ladspa.h>

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
  /** @brief Volume of each consecutive
   * echo is reduced by this factor. */
  float *echo_decay;
  /** @brief The duration of the echo, in
   * terms of milliseconds.  */
  float *echo_time;
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
    data->echo_decay = data_location;
  } else if (port == 4) {
    /* Time */
    data->echo_time = data_location;
  }
}
/* echo descriptor */

extern const LADSPA_Descriptor echo_descriptor;

static LADSPA_Handle echo_instantiate(const LADSPA_Descriptor *descriptor, unsigned long int sample_rate) {
  struct ladspa_data *data = NULL;
  if (descriptor == &echo_descriptor) {
    data = malloc(sizeof(*data));
    data->echo_count = NULL;
    data->echo_decay = NULL;
    data->echo_time = NULL;
    data->sample_rate = sample_rate;
    data->db.len = 0;
    data->db.channels = 1;
    data->effect = echo_create();
    return data;
  } else {
    return NULL;
  }
}

const char *port_names[] = {
  "Input",
  "Output",
  "Echo Count",
  "Decay",
  "Time",
  NULL
};

LADSPA_PortDescriptor port_descriptors[] = {
  LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT /* Input */,
  LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT /* Output */,
  LADSPA_PORT_CONTROL /* Echo Count */,
  LADSPA_PORT_CONTROL /* Decay */,
  LADSPA_PORT_CONTROL /* Time */,
};

LADSPA_PortRangeHint port_range_hints[] = {
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
  },
  /* Decay */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE,
    0 /* LowerBound */,
    100 /* UpperBound */
  },
  /* Time */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE,
    0 /* LowerBound */,
    MAX_ECHO_LENGTH /* UpperBound */
  }
};

static void echo_activate(LADSPA_Handle instance) {
  (void) instance;
}

static void echo_run(LADSPA_Handle instance, unsigned long int sample_count) {

  struct ladspa_data *data;
  struct echo_params *echo_params;

  data = (struct ladspa_data *) instance;
  if (data == NULL)
    return;

  if (data->echo_count != NULL) {
    echo_params = (struct echo_params *) data->effect->params;
    echo_params->echoes = (unsigned int) *data->echo_count;
  }
  if (data->echo_decay != NULL) {
    echo_params = (struct echo_params *) data->effect->params;
    echo_params->echo_decay = (float) *data->echo_decay;
  }
  if (data->echo_time != NULL) {
    echo_params = (struct echo_params *) data->effect->params;
    echo_params->echo_size = (double) *data->echo_time;
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
  sizeof(port_descriptors) / sizeof(port_descriptors[0]) /* PortCount */,
  port_descriptors /* PortDescriptors */,
  port_names /* PortNames */,
  port_range_hints /* PortRangeHints */,
  NULL /* ImplementationData */,
  echo_instantiate /* instantiate */,
  echo_connect_port /* connect_port */,
  echo_activate /* activate */,
  echo_run /* run */,
  NULL /* run_adding */,
  NULL /* run_adding_gain */,
  NULL /* deactivate */,
  NULL /* cleanup */
};

