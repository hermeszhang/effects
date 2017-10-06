/*
 * GNUitar
 * Vibrato effect
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
 *
 * $Log$
 * Revision 1.29  2006/08/06 20:14:55  alankila
 * - split pump.h into several domain-specific headers to reduce file
 *   interdependencies (everyone included pump.h). New files are:
 *   - effect.h for effect definitions
 *   - audio-driver.h for work relating to audio drivers
 *   - audio-midi.h for MIDI interaction.
 *
 * Revision 1.28  2006/08/02 19:11:18  alankila
 * - add missing static declarations
 *
 * Revision 1.27  2006/06/16 12:32:20  alankila
 * - the reasonably trivial vibrato effect is now reinstated as tremolo
 *   in a file with proper name. Old vibrato can be achieved through 1-voice
 *   100% wet chorus.
 *
 * Revision 1.26  2005/09/04 23:05:17  alankila
 * - delete the repeated toggle_foo functions, use one global from gui.c
 *
 * Revision 1.25  2005/09/04 19:45:12  alankila
 * - replace "Speed 1/ms" with "Period ms" which is easier to understand
 *
 * Revision 1.24  2005/09/04 14:40:17  alankila
 * - get rid of effect->id and associated enumeration
 *
 * Revision 1.23  2005/09/04 12:12:36  alankila
 * - make create() and done() symmetric in memory allocation/free
 *
 * Revision 1.22  2005/09/04 11:16:59  alankila
 * - destroy passthru function, move the toggle logic higher up
 *
 * Revision 1.21  2005/09/04 01:51:09  alankila
 * - GKeyFile-based preset load/save
 * - still need locale-immune %lf for printf and sscanf
 *
 * Revision 1.20  2005/09/02 11:58:49  alankila
 * - remove #ifdef HAVE_GTK2 entirely from all effect code
 *
 * Revision 1.19  2005/09/01 23:52:15  alankila
 * - make window delete event do something useful
 *
 * Revision 1.18  2005/09/01 22:36:17  alankila
 * - multichannel-ready vibrato
 *
 * Revision 1.17  2005/09/01 13:36:23  alankila
 * Objectify backbuf, correct naming and make it a typedef.
 *
 * Revision 1.16  2005/08/18 23:54:32  alankila
 * - use GTK_WINDOW_DIALOG instead of TOPLEVEL, however #define them the same
 *   for GTK2.
 *
 * Revision 1.15  2005/08/13 12:06:08  alankila
 * - removed bunch of #ifdef HAVE_GTK/HAVE_GTK2 regarding window type
 *
 * Revision 1.14  2005/08/12 17:56:16  alankila
 * use one global sin lookup table
 *
 * Revision 1.12  2005/08/08 18:34:45  alankila
 * - rename effects:
 *   * vibrato -> tremolo
 *   * tremolo -> vibrato
 *   * distort2 -> overdrive
 *
 * Revision 1.11  2004/08/10 15:21:16  fonin
 * Support processing in float/int - type DSP_SAMPLE
 *
 * Revision 1.9  2004/07/07 19:18:42  fonin
 * GTK2 port
 *
 * Revision 1.8  2003/03/09 20:53:16  fonin
 * Meaningful params "speed" and "amplitude". Structures were redesigned
 * for new feature of on-the-fly change of sampling params.
 *
 * Revision 1.7  2003/02/03 11:39:25  fonin
 * Copyright year changed.
 *
 * Revision 1.6  2003/02/01 19:15:12  fonin
 * Use sizeof(variable) instead sizeof(type) in load/save procedures,
 * when reading/writing from file.
 *
 * Revision 1.5  2003/01/30 21:35:29  fonin
 * Got rid of rnd_window_pos().
 *
 * Revision 1.4  2003/01/29 19:33:42  fonin
 * Fixed array referencing error that caused random crashes.
 * Win32 port.
 *
 * Revision 1.3  2001/06/02 14:05:59  fonin
 * Added GNU disclaimer.
 *
 * Revision 1.2  2001/03/25 12:10:50  fonin
 * Effect window control ignores delete event.
 *
 * Revision 1.1.1.1  2001/01/11 13:22:26  fonin
 * Version 0.1.0 Release 1 beta
 *
 */

#include <gnu-guitar/effects/tremolo.h>

#include <math.h>
#include <stdlib.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

static void
tremolo_init(struct effect *p)
{
    (void) p;
}

static void
tremolo_filter(struct effect *p, data_block_t *db)
{
    struct tremolo_params *tp;
    double vol;
    double speed;
    int count;
    int currchannel = 0;
    DSP_SAMPLE *s;

    tp = p->params;
    s = db->data;
    count = db->len;
    
    speed = tp->tremolo_speed / 1000.0 * db->rate;
    
    while (count) {

        if (tp->tremolo_phase >= speed)
            tp->tremolo_phase = 0;

        vol = 1.0 - tp->tremolo_amplitude / 100.0 *
            (1.0 + sin(tp->tremolo_phase / speed)) / 2.0;

        *s *= vol;

        currchannel = (currchannel + 1) % db->channels;
        if (currchannel == 0)
            tp->tremolo_phase++;

        s++;
        count--;
    }
}

static void
tremolo_done(struct effect *p)
{
    struct tremolo_params *tp;
    tp = (struct tremolo_params *) p->params;
    free(tp);
    free(p);
}

effect_t *
tremolo_create()
{
    effect_t *p;
    struct tremolo_params *ptremolo;

    p = calloc(1, sizeof(effect_t));
    p->params = calloc(1, sizeof(struct tremolo_params));
    p->proc_init = tremolo_init;
    p->proc_filter = tremolo_filter;
    p->toggle = 0;
    p->proc_done = tremolo_done;

    ptremolo = p->params;
    ptremolo->tremolo_amplitude = 25;
    ptremolo->tremolo_speed = 200;
    ptremolo->tremolo_phase = 0;

    return p;
}
