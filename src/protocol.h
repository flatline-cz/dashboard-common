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
#ifndef DASHBOARD_COMMON_PROTOCOL_H
#define DASHBOARD_COMMON_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>
#include "constants.h"
#include "buffer.h"


typedef struct {
    int interface;
    uint32_t id;
    bool extended;
    uint8_t dlc;
    uint8_t data[8];
} tCANMessage;

typedef unsigned tVersion;
typedef struct {
    unsigned length;
    bool direct;
    uint8_t data[UDS_MAX_RECORD_LENGTH];
} tUDSRecord;


#define CMD_TYPE_NONE               0
#define CMD_TYPE_CAN_MESSAGE        1
#define CMD_TYPE_VERSION            2
#define CMD_TYPE_UDS                3

typedef struct {
    unsigned type;
    union {
        tCANMessage can_message;
        tVersion version;
        tUDSRecord udsRecord;
    };

} tCommand;

void protocol_dump(tCommand *cmd);

bool protocol_parse(const uint8_t *data, unsigned length, tCommand *cmd);

bool protocol_write(tCommand *cmd, tWriteBuffer *buffer);

#endif //DASHBOARD_COMMON_PROTOCOL_H
