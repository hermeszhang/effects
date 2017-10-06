#include <gnu-guitar/effects/tremolo.h>

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
  float *amplitude;
  float *speed;
  float *phase;
};

static void tremolo_connect_port(LADSPA_Handle instance,
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
    /* Amplitude */
    data->amplitude = data_location;
  } else if (port == 3) {
    /* Speed */
    data->speed = data_location;
  } else if (port == 4) {
    /* Phase */
    data->phase = data_location;
  }
}
/* echo descriptor */

extern const LADSPA_Descriptor tremolo_descriptor;

static LADSPA_Handle tremolo_instantiate(const LADSPA_Descriptor *descriptor,
                                         unsigned long int sample_rate) {
  struct ladspa_data *data = NULL;
  if (descriptor == &tremolo_descriptor) {
    data = malloc(sizeof(*data));
    data->amplitude = NULL;
    data->speed = NULL;
    data->phase = NULL;
    data->db.rate = sample_rate;
    data->db.len = 0;
    data->db.channels = 1;
    data->effect = tremolo_create();
    return data;
  } else {
    return NULL;
  }
}

static const char *port_names[] = {
  "Input",
  "Output",
  "Amplitude",
  "Speed",
  "Phase",
  NULL
};

static LADSPA_PortDescriptor port_descriptors[] = {
  LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT /* Input */,
  LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT /* Output */,
  LADSPA_PORT_CONTROL /* Amplitude */,
  LADSPA_PORT_CONTROL /* Speed */,
  LADSPA_PORT_CONTROL /* Phase */,
};

static LADSPA_PortRangeHint port_range_hints[] = {
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
  /* Amplitude */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    0 /* LowerBound */,
    100 /* UpperBound */
  },
  /* Speed */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    20 /* LowerBound */,
    2000 /* UpperBound */
  },
  /* Phase */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    0 /* LowerBound */,
    100 /* UpperBound */
  }
};

static void tremolo_activate(LADSPA_Handle instance) {
  (void) instance;
}

static void tremolo_run(LADSPA_Handle instance, unsigned long int sample_count) {

  struct ladspa_data *data;
  struct tremolo_params *tremolo_params;

  data = (struct ladspa_data *) instance;
  if (data == NULL)
    return;

  tremolo_params = (struct tremolo_params *) data->effect->params;

  if (data->amplitude != NULL)
    tremolo_params->tremolo_amplitude = *data->amplitude;

  if (data->speed != NULL)
    tremolo_params->tremolo_speed = *data->speed;

  if (data->phase != NULL)
    tremolo_params->tremolo_phase = *data->phase;

  data->db.len = sample_count / data->db.channels;

  data->effect->proc_filter(data->effect, &data->db);
}

const LADSPA_Descriptor tremolo_descriptor = {
  0 /* UniqueID */,
  "GNU Guitar Tremolo" /* Label */,
  0 /* Properties */,
  "GNU Guitar Tremolo" /* Name */,
  "GNU Guitar" /* Maker */,
  "GPLv2" /* Copyright */,
  sizeof(port_descriptors) / sizeof(port_descriptors[0]) /* PortCount */,
  port_descriptors /* PortDescriptors */,
  port_names /* PortNames */,
  port_range_hints /* PortRangeHints */,
  NULL /* ImplementationData */,
  tremolo_instantiate /* instantiate */,
  tremolo_connect_port /* connect_port */,
  tremolo_activate /* activate */,
  tremolo_run /* run */,
  NULL /* run_adding */,
  NULL /* run_adding_gain */,
  NULL /* deactivate */,
  NULL /* cleanup */
};

