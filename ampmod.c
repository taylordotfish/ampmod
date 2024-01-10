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

#include "ampmod.h"
#include <math.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define URI "https://taylor.fish/plugins/ampmod"
#define FLOAT_CMP_PRECISION 0.00001

static inline int float_cmp(float first, float second) {
    float diff = first - second;
    if (diff < -FLOAT_CMP_PRECISION) return -1;
    if (diff > FLOAT_CMP_PRECISION) return 1;
    return 0;
}

static inline float clampf(float f, float min, float max) {
    #ifdef STRICT_BOUNDS_REQUIRED
        (void)min;
        (void)max;
    #else
        if (f < min) return min;
        if (f > max) return max;
    #endif
    return f;
}

static LV2_Handle instantiate(
        const LV2_Descriptor *descriptor, double rate, const char *bundle_path,
        const LV2_Feature * const *features) {
    AmpMod *am = calloc(1, sizeof(AmpMod));
    am->sample_rate = rate;
    am->samples_per_ms = (float)rate / 1000;
    return (LV2_Handle)am;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data) {
    AmpMod *am = (AmpMod *)instance;
    switch (port) {
        case PORT_INPUT_L:
            am->inputs[0] = data;
            break;
        case PORT_OUTPUT_L:
            am->outputs[0] = data;
            break;
        case PORT_INPUT_R:
            am->inputs[1] = data;
            break;
        case PORT_OUTPUT_R:
            am->outputs[1] = data;
            break;
        case PORT_EXPONENT:
            am->exponent = data;
            break;
        case PORT_BASE:
            am->base = data;
            break;
        case PORT_INCREMENT:
            am->increment = data;
            break;
        case PORT_MIN_MULT:
            am->min_multiplier = data;
            break;
    }
}

static void activate(LV2_Handle instance) {
    AmpMod *am = (AmpMod *)instance;
    am->last_base = 0;
    am->base_target = 0;
    am->moving = false;
    am->samples_since_change = 0;
}

static void handle_base_change(AmpMod *am) {
    const float base = clampf(*am->base, 0, 1);
    if (float_cmp(base, am->base_target) == 0) return;
    const float samples_per_ms = am->samples_per_ms;
    const float increment_per_ms = *am->increment;
    bool smoothing = (increment_per_ms > 0);

    if (am->moving && smoothing) {
        int old_direction = am->base_target < am->last_base ? -1 : 1;
        float new_last_base = (
            am->last_base +
            (am->samples_since_change / samples_per_ms) *
            increment_per_ms * old_direction
        );
        am->last_base = (old_direction < 0 ? fmaxf : fminf)(
            am->base_target, new_last_base
        );
    } else {
        am->last_base = am->base_target;
    }

    am->moving = smoothing;
    am->samples_since_change = 0;
    am->base_target = base;
}

static void run(LV2_Handle instance, uint32_t n_samples) {
    if (n_samples == 0) return;
    AmpMod *am = (AmpMod *)instance;
    const float * const * const inputs = am->inputs;
    float * const * const outputs = am->outputs;

    handle_base_change(am);
    const float base_target = am->base_target;
    const float exponent = clampf(*am->exponent, 0, 3.5);
    float last_base = am->last_base;

    const float samples_per_ms = am->samples_per_ms;
    const float increment_per_ms = clampf(*am->increment, 0, 0.1);
    const float min_multiplier = clampf(*am->min_multiplier, 0, 1);

    uint32_t samples_since_change = am->samples_since_change;
    bool moving = am->moving;
    int direction = base_target < last_base ? -1 : 1;

    for (uint32_t pos = 0; pos < n_samples; pos++) {
        float base = 0;
        if (moving) {
            base = (
                last_base +
                (samples_since_change / samples_per_ms) *
                increment_per_ms * direction
            );
            moving = (
                direction < 0 ?
                (base > base_target) : (base < base_target)
            );
        }

        if (!moving) {
            base = base_target;
        }

        float multiplier = powf(base, exponent);
        multiplier = (1 - min_multiplier) * multiplier + min_multiplier;
        if (multiplier > 1) multiplier = 1;

        for (int i = 0; i < 2; i++) {
            outputs[i][pos] = inputs[i][pos] * multiplier;
        }
        samples_since_change++;
    }

    am->last_base = last_base;
    am->samples_since_change = samples_since_change;
    am->moving = moving;
}

static void deactivate(LV2_Handle instance) {
}

static void cleanup(LV2_Handle instance) {
    AmpMod *am = (AmpMod *)instance;
    free(am);
}

static const void *extension_data(const char *uri) {
    return NULL;
}

static const LV2_Descriptor descriptor = {
    URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index) {
    switch (index) {
        case 0:
            return &descriptor;
        default:
            return NULL;
    }
}
