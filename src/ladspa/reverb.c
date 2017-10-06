#include <gnu-guitar/effects/reverb.h>
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
  float *delay;
  /** @brief Volume of each consecutive
   * reverb is reduced by this factor. */
  float *decay;
  /** @brief The duration of the reverb, in
   * terms of milliseconds.  */
  float *dry_wet_balance;
};

static void reverb_connect_port(LADSPA_Handle instance,
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
    /* Delay */
    data->delay = data_location;
  } else if (port == 3) {
    /* Decay */
    data->decay = data_location;
  } else if (port == 4) {
    /* Dry/Wet */
    data->dry_wet_balance = data_location;
  }
}
/* reverb descriptor */

extern const LADSPA_Descriptor reverb_descriptor;

static LADSPA_Handle reverb_instantiate(const LADSPA_Descriptor *descriptor, unsigned long int sample_rate) {
  struct ladspa_data *data = NULL;
  if (descriptor == &reverb_descriptor) {
    data = malloc(sizeof(*data));
    data->delay = NULL;
    data->decay = NULL;
    data->dry_wet_balance = NULL;
    data->db.rate = sample_rate;
    data->db.len = 0;
    data->db.channels = 1;
    data->effect = reverb_create();
    return data;
  } else {
    return NULL;
  }
}

static const char *port_names[] = {
  "Input",
  "Output",
  "Delay",
  "Decay",
  "Dry/Wet",
  NULL
};

static LADSPA_PortDescriptor port_descriptors[] = {
  LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT /* Input */,
  LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT /* Output */,
  LADSPA_PORT_CONTROL /* Delay */,
  LADSPA_PORT_CONTROL /* Decay */,
  LADSPA_PORT_CONTROL /* Dry/Wet */,
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
  /* Delay */ {
    LADSPA_HINT_INTEGER | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE,
    0 /* LowerBound */,
    MAX_REVERB_SIZE /* UpperBound */
  },
  /* Decay */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE,
    0 /* LowerBound */,
    100 /* UpperBound */
  },
  /* Dry/Wet */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE,
    0 /* LowerBound */,
    100 /* UpperBound */
  }
};

static void reverb_activate(LADSPA_Handle instance) {
  (void) instance;
}

static void reverb_run(LADSPA_Handle instance, unsigned long int sample_count) {

  struct ladspa_data *data;
  struct reverb_params *reverb_params;

  data = (struct ladspa_data *) instance;
  if (data == NULL)
    return;

  if (data->delay != NULL) {
    reverb_params = (struct reverb_params *) data->effect->params;
    reverb_params->delay = (double) *data->delay;
  }
  if (data->decay != NULL) {
    reverb_params = (struct reverb_params *) data->effect->params;
    reverb_params->regen = (double) *data->decay;
  }
  if (data->dry_wet_balance != NULL) {
    reverb_params = (struct reverb_params *) data->effect->params;
    reverb_params->drywet = (double) *data->dry_wet_balance;
  }

  data->db.len = sample_count / data->db.channels;

  data->effect->proc_filter(data->effect, &data->db);
}

const LADSPA_Descriptor reverb_descriptor = {
  0 /* UniqueID */,
  "GNU Guitar Reverb" /* Label */,
  0 /* Properties */,
  "GNU Guitar Reverb" /* Name */,
  "GNU Guitar" /* Maker */,
  "GPLv2" /* Copyright */,
  sizeof(port_descriptors) / sizeof(port_descriptors[0]) /* PortCount */,
  port_descriptors /* PortDescriptors */,
  port_names /* PortNames */,
  port_range_hints /* PortRangeHints */,
  NULL /* ImplementationData */,
  reverb_instantiate /* instantiate */,
  reverb_connect_port /* connect_port */,
  reverb_activate /* activate */,
  reverb_run /* run */,
  NULL /* run_adding */,
  NULL /* run_adding_gain */,
  NULL /* deactivate */,
  NULL /* cleanup */
};

