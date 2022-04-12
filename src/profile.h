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
#ifndef DASHBOARD_COMMON_PROFILE_H
#define DASHBOARD_COMMON_PROFILE_H

typedef unsigned long long tTime;

void time_init();

tTime time_get();

void time_profile_start();

unsigned time_profile_end();

void sleep_a_bit();


#endif//DASHBOARD_COMMON_PROFILE_H
