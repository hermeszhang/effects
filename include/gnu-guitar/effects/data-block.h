/* Common definitions for audio drivers. (c) 2006 Antti S. Lankila
 * Licensed under GPL.
 * $Id$
 *
 * $Log$
 * Revision 1.7  2006/08/23 08:11:23  alankila
 * - add unlikely to hopefully speed up if() test
 *
 * Revision 1.6  2006/08/22 06:40:05  alankila
 * - inform gcc on the rare case where it may be helpful that
 *   data and data_swap pointers do not alias.
 *
 * Revision 1.5  2006/08/10 16:18:36  alankila
 * - improve const correctness and make gnuitar compile cleanly under
 *   increasingly pedantic warning models.
 *
 * Revision 1.4  2006/08/08 21:05:31  alankila
 * - optimize gnuitar: this breaks dsound, I'll fix it later
 *
 * Revision 1.3  2006/08/07 12:55:30  alankila
 * - construct audio-driver.c to hold globals and provide some utility
 *   functions to its users. This slashes interdependencies somewhat.
 *
 * Revision 1.2  2006/08/06 20:45:52  alankila
 * - use stdint type rather than glib type
 *
 * Revision 1.1  2006/08/06 20:14:54  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 */

#ifndef _AUDIO_DRIVER_H_
#define _AUDIO_DRIVER_H_ 1

#include <stdint.h>

#ifdef _MSC_VER
#define restrict __restrict
#endif

#define MAX_SAMPLE (32767 << 8)
#define MAX_CHANNELS 8
#define MAX_SAMPLE_RATE 48000

#ifdef GNU_GUITAR_USE_INTEGERS
typedef int32_t gnu_guitar_sample_t;
#else
typedef float gnu_guitar_sample_t;
#endif

#define DSP_SAMPLE gnu_guitar_sample_t

typedef struct gnu_guitar_data_block {
  gnu_guitar_sample_t * restrict data;
  gnu_guitar_sample_t * restrict data_swap;
  uint_fast32_t    len;
  uint_fast8_t     channels;
} data_block_t;

#endif
