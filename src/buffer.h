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
#ifndef DASHBOARD_COMMON_BUFFER_H
#define DASHBOARD_COMMON_BUFFER_H

#include <stdint.h>
#include "constants.h"

typedef int tHandle;

typedef struct {
    uint8_t buffer[CMD_LENGTH];
    unsigned position;
    unsigned length;
} tCMDBuffer;

typedef struct {
    const char *name;
    tCMDBuffer buffers[BUFFER_COUNT];
    unsigned position;
    unsigned length;
} tWriteBuffer;

void buffer_make_nonblocking(tHandle handle);

void buffer_write_add(tWriteBuffer *context, const char *cmd, unsigned length);

void buffer_write_send(tWriteBuffer *context, tHandle handle);

typedef struct {
    uint8_t buffer[CMD_LENGTH];
    unsigned length;
} tReadBuffer;

typedef void (*hProcessor)(uint8_t *cmd, unsigned length);

typedef void (*hFailureHandler)();

typedef struct {
    tReadBuffer *buffer;
    tHandle *handle;
    hProcessor processor;
    hFailureHandler failureHandler;
} tReadChannel;

void buffer_read_process(tReadBuffer *context, tHandle handle, hProcessor processor);

void buffer_process_read(tReadChannel channels[]);

#endif //DASHBOARD_COMMON_BUFFER_H
