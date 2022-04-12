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
#include "log.h"
#include <stdio.h>


void log_message(const char *source_file, int source_line, const char *msg) {
    if (source_file)
        fprintf(stderr, "%s(%d): %s\n", source_file, source_line, msg);
    else
        fprintf(stderr, "%s\n", msg);
}

