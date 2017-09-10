/*
 * GNUitar
 * Reverberation effect
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
 *
 * $Id$
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#    include <io.h>
#else
#    include <unistd.h>
#endif

#include <gnu-guitar/effects/reverb.h>

extern unsigned int sample_rate;

static void reverb_init(struct effect *p) {
  (void) p;
}

/* backbuf-based allpass filter for longer than 1 or 2 sample delays.
 * This */
static double allpass_filter(double input, double factor, int delay, Backbuf_t *history) {
    double tmp, output;
    // delay - 1 because 0th is already the previous, so someone asking
    // for the previous sample is really asking for the 0th sample until
    // add() is performed
    tmp = input - factor * history->get(history, delay - 1);
    output = history->get(history, delay - 1) + factor * tmp;
    history->add(history, tmp);
    return output;
}

static double comb_filter(double input, double factor, int delay, Backbuf_t *history) {
    double output;
    history->add(history, input); // 0th is current, -1 is previous
    output = input + factor * history->get(history, delay);
    return output;
}

static void reverb_filter(struct effect *p, data_block_t *db) {
    struct reverb_params *params = p->params;
    DSP_SAMPLE *s;
    DSP_SAMPLE *outs;
    int count;
    int c = 0; /* curr_channel */
    float input, a, mono, Dry, Wet, Rgn, Delay, fsr;

    s = db->data;
    outs = db->data_swap;
    count = db->len;

    Delay = params->delay / 1000.0 * sample_rate;
    Wet = params->drywet / 100.0;
    Dry = 1 - Wet;
    Rgn = params->regen / 100.0;
    
    /* This is a John Chowning reverberator, explained here:
     * http://www-ccrma.stanford.edu/~jos/waveguide/Schroeder_Reverberator_called_JCRev.html
     *
     * Signal block diagram is as follows:
     *
     * input--[ ap(0.7, 1051) ]--[ ap(0.7, 337)]--[ ap(0.7, 113) ]--a
     *
     *    +--[ comb(0.742, 4799) ]--+
     *    |                         |
     *    +--[ comb(0.733, 4999) ]--+
     * a--+                        (+)--mono
     *    +--[ comb(0.715, 5399) ]--+
     *    |                         |
     *    +--[ comb(0.697, 5801) ]--+
     * 
     * In case multichannel output is desired, the output is further processed
     * with:
     *
     *       +--[ delay(0.046 * sample_rate) ]--ch1
     *       |                                  
     *       +--[ delay(0.057 * sample_rate) ]--ch2
     * mono--+
     *       +--[ delay(0.041 * sample_rate) ]--ch3
     *       |
     *       +--[ delay(0.054 * sample_rate) ]--ch4
     *
     * For stereo output, the channels 1, 2 and 3, 4 are averaged.
     */

    fsr = sample_rate / 44100;
    while (count) {
        input = *s;

        /* change from original: infinite reverb through history */
        input += params->history[c]->get(params->history[c], Delay) * Rgn;
        
        a = allpass_filter(input, 0.7, 1051 * fsr, params->ap[c][0]);
        a = allpass_filter(a,     0.7,  337 * fsr, params->ap[c][1]);
        a = allpass_filter(a,     0.7,  113 * fsr, params->ap[c][2]);
        
        mono  = comb_filter(a, 0.742, 4799 * fsr, params->comb[c]);
        mono += comb_filter(a, 0.733, 4999 * fsr, params->comb[c]);
        mono += comb_filter(a, 0.715, 5399 * fsr, params->comb[c]);
        mono += comb_filter(a, 0.697, 5801 * fsr, params->comb[c]);
        /* change from original: some more reverb terms */
        mono += comb_filter(a, 0.75, 3533 * fsr, params->comb[c]);
        mono += comb_filter(a, 0.65, 6521 * fsr, params->comb[c]);
        
        params->history[c]->add(params->history[c], mono / 6.0);
        
        *outs = *s * Dry + mono * Wet;

        c = (c + 1) % db->channels;
        outs++;
        s++;
        count--;
    }
}

static void reverb_done(struct effect *p) {
    struct reverb_params *dr;
    int             i;
    
    dr = (struct reverb_params *) p->params;

    for (i = 0; i < MAX_CHANNELS; i += 1) {
        del_Backbuf(dr->history[i]);
        del_Backbuf(dr->ap[i][0]);
        del_Backbuf(dr->ap[i][1]);
        del_Backbuf(dr->ap[i][2]);
        del_Backbuf(dr->comb[i]);
    }
    
    free(p->params);
    free(p);
}

effect_t *
reverb_create()
{
    effect_t       *p;
    struct reverb_params *dr;
    int             i;
    
    p = calloc(1, sizeof(effect_t));
    p->params = calloc(1, sizeof(struct reverb_params));

    p->proc_init = reverb_init;
    p->proc_filter = reverb_filter;
    p->toggle = 0;
    p->proc_done = reverb_done;
    dr = (struct reverb_params *) p->params;
    for (i = 0; i < MAX_CHANNELS; i += 1) {
        dr->history[i] = new_Backbuf(MAX_REVERB_SIZE / 1000.0 * MAX_SAMPLE_RATE);
        dr->ap[i][0] = new_Backbuf(2048);
        dr->ap[i][1] = new_Backbuf(2048);
        dr->ap[i][2] = new_Backbuf(2048);
        dr->comb[i] = new_Backbuf(8192);
    }
    dr->delay  = 130;    /* ms */
    dr->drywet = 25.0;
    dr->regen  = 25.0;
    return p;
}
