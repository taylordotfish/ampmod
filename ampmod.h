/*
 * Copyright (C) 2017 taylor.fish <contact@taylor.fish>
 *
 * This file is part of Fish AmpMod.
 *
 * Fish AmpMod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fish AmpMod is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fish AmpMod.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AMPMOD_H
#define AMPMOD_H

#include "ports.h"
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    const float *exponent;
    const float *base;
    const float *increment;
    const float *min_multiplier;
    float last_base;
    float base_target;
    bool moving;

    uint32_t samples_since_change;
    uint32_t sample_rate;
    float samples_per_ms;
    const float *inputs[2];
    float *outputs[2];
} AmpMod;

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index);

#endif
