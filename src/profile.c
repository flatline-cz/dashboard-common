/*
 * Copyright (c) 2022 by flatline.cz@gmail.com
 *
 * This file is a part of DASHBOARD.
 *
 * DASHBOARD is free software: you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. If not,
 * see <https://www.gnu.org/licenses/>
 */
#include "profile.h"
#include <stddef.h>
#include <sys/time.h>
#include <time.h>

static tTime offset;

static tTime get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000ULL) + (t.tv_nsec / 1000000ULL);
}

void time_init() {
    offset = get_time();
}

tTime time_get() {
    return get_time() - offset;
}


static tTime profile;

void time_profile_start() {
    profile = get_time();
}

unsigned time_profile_end() {
    return get_time() - profile;
}

void sleep_a_bit() {
    struct timespec sleep;
    sleep.tv_sec = 0;
    sleep.tv_nsec = 100000;
    nanosleep(&sleep, NULL);
}
