#include <gnu-guitar/effects/tubeamp.h>

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
  float *stages;
  /* TODO : impulse model */
  float *impulse_quality;
  float *gain;
  float *asymmetry;
  float *biasfactor;
  float *tone_bass;
  float *tone_middle;
  float *tone_treble;
};

static void tubeamp_connect_port(LADSPA_Handle instance,
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
    /* Stages */
    data->stages = data_location;
  } else if (port == 3) {
    /* Impulse Quality */
    data->impulse_quality = data_location;
  } else if (port == 4) {
    /* Gain */
    data->gain = data_location;
  } else if (port == 5) {
    /* Asymmetry */
    data->asymmetry = data_location;
  } else if (port == 6) {
    /* Bias */
    data->biasfactor = data_location;
  } else if (port == 7) {
    /* Bass */
    data->tone_bass = data_location;
  } else if (port == 8) {
    /* Middle */
    data->tone_middle = data_location;
  } else if (port == 9) {
    /* Treble */
    data->tone_treble = data_location;
  }
}
/* tubeamp descriptor */

extern const LADSPA_Descriptor tubeamp_descriptor;

static LADSPA_Handle tubeamp_instantiate(const LADSPA_Descriptor *descriptor, unsigned long int sample_rate) {
  struct ladspa_data *data = NULL;
  if (descriptor == &tubeamp_descriptor) {
    data = malloc(sizeof(*data));
    data->sample_rate = sample_rate;
    data->db.len = 0;
    data->db.channels = 1;
    data->stages = NULL;
    data->impulse_quality = NULL;
    data->gain = NULL;
    data->asymmetry = NULL;
    data->biasfactor = NULL;
    data->tone_bass = NULL;
    data->tone_middle = NULL;
    data->tone_treble = NULL;
    data->effect = tubeamp_create();
    return data;
  } else {
    return NULL;
  }
}

static const char *port_names[] = {
  "Input",
  "Output",
  "Stages",
  "Impulse Quality",
  "Gain",
  "Asymmetry",
  "Bias",
  "Bass",
  "Middle",
  "Treble",
  NULL
};

static LADSPA_PortDescriptor port_descriptors[] = {
  LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT /* Input */,
  LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT /* Output */,
  LADSPA_PORT_CONTROL /* Stages */,
  LADSPA_PORT_CONTROL /* Impulse Quality */,
  LADSPA_PORT_CONTROL /* Gain */,
  LADSPA_PORT_CONTROL /* Asymmetry */,
  LADSPA_PORT_CONTROL /* Bias */,
  LADSPA_PORT_CONTROL /* Bass */,
  LADSPA_PORT_CONTROL /* Middle */,
  LADSPA_PORT_CONTROL /* Treble */,
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
  /* Stages */ {
    LADSPA_HINT_INTEGER | LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MAXIMUM,
    2 /* LowerBound */,
    MAX_STAGES /* UpperBound */
  },
  /* Impulse Quality */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    0 /* LowerBound */,
    3 /* UpperBound */
  },
  /* Gain */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_LOW,
    30 /* LowerBound */,
    50 /* UpperBound */
  },
  /* Asymmetry */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_LOW,
    -5000 /* LowerBound */,
    5000 /* UpperBound */
  },
  /* Bias */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_LOW,
    -25 /* LowerBound */,
    25 /* UpperBound */
  },
  /* Bass */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    -10 /* LowerBound */,
    10 /* UpperBound */
  },
  /* Middle */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    -20 /* LowerBound */,
    0 /* UpperBound */
  },
  /* Treble */ {
    LADSPA_HINT_BOUNDED_BELOW | LADSPA_HINT_BOUNDED_ABOVE | LADSPA_HINT_DEFAULT_MIDDLE,
    -10 /* LowerBound */,
    10 /* UpperBound */
  }
};

static void tubeamp_activate(LADSPA_Handle instance) {
  (void) instance;
}

static void tubeamp_run(LADSPA_Handle instance, unsigned long int sample_count) {

  struct ladspa_data *data;
  struct tubeamp_params *tubeamp_params;

  (void) tubeamp_params;

  data = (struct ladspa_data *) instance;
  if (data == NULL)
    return;

  if (data->stages != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->stages = (unsigned int) *data->stages;
  }

  if (data->impulse_quality != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->impulse_quality = (unsigned int) *data->impulse_quality;
  }

  if (data->gain != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->gain = (float) *data->gain;
  }

  if (data->asymmetry != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->asymmetry = (float) *data->asymmetry;
  }

  if (data->biasfactor != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->biasfactor = (float) *data->biasfactor;
  }

  if (data->tone_bass != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->tone_bass = (float) *data->tone_bass;
  }

  if (data->tone_middle != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->tone_middle = (float) *data->tone_middle;
  }

  if (data->tone_treble != NULL) {
    tubeamp_params = (struct tubeamp_params *) data->effect->params;
    tubeamp_params->tone_treble = (float) *data->tone_treble;
  }

  data->db.len = sample_count / data->db.channels;

  data->effect->proc_filter(data->effect, &data->db);
}

const LADSPA_Descriptor tubeamp_descriptor = {
  0 /* UniqueID */,
  "GNU Guitar Marshall G15R" /* Label */,
  0 /* Properties */,
  "GNU Guitar Marshall G15R" /* Name */,
  "GNU Guitar" /* Maker */,
  "GPLv2" /* Copyright */,
  sizeof(port_descriptors) / sizeof(port_descriptors[0]) /* PortCount */,
  port_descriptors /* PortDescriptors */,
  port_names /* PortNames */,
  port_range_hints /* PortRangeHints */,
  NULL /* ImplementationData */,
  tubeamp_instantiate /* instantiate */,
  tubeamp_connect_port /* connect_port */,
  tubeamp_activate /* activate */,
  tubeamp_run /* run */,
  NULL /* run_adding */,
  NULL /* run_adding_gain */,
  NULL /* deactivate */,
  NULL /* cleanup */
};

