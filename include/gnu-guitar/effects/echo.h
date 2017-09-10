/*
 * GNUitar
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
 *
 * $Id$
 */

#ifndef _ECHO_H_
#define _ECHO_H_ 1

#include "effect.h"
#include "backbuf.h"

#define MAX_ECHO_COUNT 4
#define MAX_ECHO_LENGTH 500 /* ms */

effect_t * echo_create(void);

struct echo_params {
    Backbuf_t *history[MAX_CHANNELS][MAX_ECHO_COUNT];
    double primes[MAX_ECHO_COUNT];
    double size_factor[MAX_ECHO_COUNT];
    double decay_factor[MAX_ECHO_COUNT];
    /** @brief Length of the echo, in milliseconds. */
    double echo_size;
    /** @brief Each echo reduces in volume. The amount
     * of volume reduced is determined by the decay. This
     * value should be between zero and one hundred. */
    double echo_decay;
    /** @brief The number of echoes that occur. */
    unsigned int echoes;
    /** @deprecated */
    short multichannel;
};

#endif
