/* GNUitar
 * Echo effect
 * Copyright (C) 2000,2001,2003 Max Rudensky         <fonin@ziet.zhitomir.ua>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <gnu-guitar/effects/echo.h>

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#ifndef _WIN32
#    include <unistd.h>
#else
#    include <io.h>
#endif

#include <stdio.h>

#define ECHO_FIRST_PRIME                    20
#define ECHO_NEXT_PRIME_DISTANCE_FACTOR	    1.6
#define ECHO_CROSSMIX_ATTN                  10.0

unsigned int sample_rate = 48000;

static int
is_prime(int n)
{
    int             i;

    for (i = 2; i < n; i++)
	if (n % i == 0)
	    return 0;

    return 1;
}

static void
echo_init(struct effect *p)
{
  struct echo_params *params;
  params = (struct echo_params *) p->params;
  params->echo_size = 0.5;
  params->echo_decay = 0.5;
  params->echoes = 2;
  params->multichannel = 0;
}

static void
echo_filter(effect_t *p, data_block_t *db)
{
    unsigned int        i, count, curr_channel = 0;
    DSP_SAMPLE          *s, tmp;
    double              in, out, echo_samples, echo_decay;
    struct echo_params  *params;
    int                 delay_lookup[MAX_ECHO_COUNT];
    float               decay_lookup[MAX_ECHO_COUNT];
    float               *outs;

    s = db->data;
    outs = db->data_swap;
    count = db->len;

    params = p->params;

    if (params->echoes > MAX_ECHO_COUNT) {
        gnuitar_printf("Warning: max echo count exceeded: %u\n", params->echoes);
        params->echoes = MAX_ECHO_COUNT;
    }

    echo_samples = params->echo_size / 1000.0 * sample_rate;
    echo_decay = params->echo_decay / 100.0;

    for (i = 0; i < params->echoes; i += 1) {
        delay_lookup[i] = echo_samples * params->size_factor[i];
        decay_lookup[i] = pow(echo_decay, params->decay_factor[i]);
    }

    while (count) {
        /* mix current input into the various echo buffers at their
         * expected delays */
        in = *s;
        out = in;
        for (i = 0; i < params->echoes; i += 1) {
            tmp = params->history[curr_channel][i]->get(params->history[curr_channel][i], delay_lookup[i]) * decay_lookup[i];
            out += tmp;
            if (params->echoes > 1) {
                if (i > 1)
                    tmp += params->history[0][i-1]->get(params->history[0][i-1], delay_lookup[i-1]) * decay_lookup[i-1] / ECHO_CROSSMIX_ATTN;
                else
                    tmp += params->history[0][params->echoes-1]->get(params->history[0][params->echoes-1], delay_lookup[params->echoes-1]) * decay_lookup[params->echoes-1] / ECHO_CROSSMIX_ATTN;
            }
            params->history[curr_channel][i]->add(params->history[curr_channel][i], in + tmp);
        }
        *outs = out;
        
        curr_channel = (curr_channel + 1) % db->channels; 
        s++;
        outs++;
        count--;
    }
}

static void
echo_done(struct effect *p)
{
    struct echo_params *params;
    int             i, j;

    params = p->params;
    for (i = 0; i < MAX_ECHO_COUNT; i += 1) {
        for (j = 0; j < MAX_CHANNELS; j += 1) {
            del_Backbuf(params->history[j][i]);
        }
    }

    free(p);
}

effect_t *
echo_create()
{
    effect_t       *p;
    struct echo_params *params;
    int             i, j, k;

    p = calloc(1, sizeof(effect_t));
    p->params = calloc(1, sizeof(*params));
    p->proc_init = echo_init;
    p->proc_filter = echo_filter;
    p->toggle = 0;
    p->proc_done = echo_done;

    params = p->params;
    params->multichannel = 1;
    params->echo_size = 200;
    params->echo_decay = 30.0;
    params->echoes = MAX_ECHO_COUNT;

    /* find some primes to base echo times on */
    k = ECHO_FIRST_PRIME;
    for (i = 0; i < MAX_ECHO_COUNT; i += 1) {
	while (! is_prime(k))
	    k += 1;
        params->primes[i] = k;
        k *= ECHO_NEXT_PRIME_DISTANCE_FACTOR;
    }
    /* scale primes such that largest value is 1.0 in both */
    for (i = 0; i < MAX_ECHO_COUNT; i += 1) {
        params->size_factor[i] = params->primes[i] / params->primes[0];
        params->decay_factor[i] = params->primes[i] / params->primes[0];
    }
    /* build history buffers, one per channel per echo */
    /* with 20 voices, 0.5 s max buffer, 48 kHz sample rate
     * and 4 bytes per sample we need approx. 1 MB */
    for (i = 0; i < MAX_ECHO_COUNT; i += 1) {
        for (j = 0; j < MAX_CHANNELS; j += 1) {
            params->history[j][i] = new_Backbuf(MAX_ECHO_LENGTH / 1000.0 * MAX_SAMPLE_RATE * params->size_factor[i]);
        }
    }
    return p;
}
