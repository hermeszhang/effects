/* GNUitar effect interface.
 * Copyright (C) 2006 Antti S. Lankila. Licensed under GPL.
 *
 * $Id$
 *
 * $Log$
 * Revision 1.3  2006/08/10 16:18:36  alankila
 * - improve const correctness and make gnuitar compile cleanly under
 *   increasingly pedantic warning models.
 *
 * Revision 1.2  2006/08/07 20:01:50  alankila
 * - move all modifications of effect list structures into effect.c.
 *
 * Revision 1.1  2006/08/07 13:20:42  alankila
 * - group all effects through effect.h rather than enumerating them in
 *   pump.c.
 *
 */

#include <gnu-guitar/effects/effect.h>

void gnu_guitar_effect_filter(gnu_guitar_effect_t *effect,
                              struct gnu_guitar_packet *packet) {
  effect->proc_filter(effect, packet);
}

void gnu_guitar_effect_free(gnu_guitar_effect_t *effect) {
  if (effect->proc_done != NULL) {
    effect->proc_done(effect);
  }
}

