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
#include <string.h>
#include <stdio.h>
#include "protocol.h"
#include "log.h"

void protocol_dump(tCommand *cmd) {
    switch (cmd->type) {
        case CMD_TYPE_NONE:
            LOG_INFO("Command: NONE");
            return;
        case CMD_TYPE_CAN_MESSAGE:
            LOG_INFO("Command: CAN message");
            return;
        case CMD_TYPE_VERSION:
            LOG_INFO("Command: version");
            return;
        case CMD_TYPE_UDS: {
            char temp[UDS_MAX_RECORD_LENGTH * 3 + 32], *p;
            strcpy(temp, (cmd->udsRecord.direct) ? "Command: UDS (direct) record [" : "Command: UDS (OBD) record [");
            p = temp + strlen(temp);
            int i;
            for (i = 0; i < cmd->udsRecord.length; i++) {
                sprintf(p, " %02X", cmd->udsRecord.data[i]);
                p += 3;
            }
            strcpy(p, " ]");
            LOG_INFO(temp);
            return;
        }
        default:
            LOG_FAILURE("Unknown command");
            return;
    }
}


bool protocol_parse(const uint8_t *data, unsigned length, tCommand *cmd) {
    if (length && (data[0] == '*' || data[0] == '&')) {
        cmd->type = CMD_TYPE_UDS;
        if (data[0] == '*')
            cmd->udsRecord.direct = true;
        else
            cmd->udsRecord.direct = false;
        cmd->udsRecord.length = 0;

        unsigned i;
        bool high_nibble = true;
        for (i = 1; i < length; i++) {
            register uint8_t value;
            register uint8_t ch = data[i];
            if (ch >= '0' && ch <= '9') {
                value = ch - '0';
            } else if (ch >= 'a' && ch <= 'f') {
                value = ch - 'a' + 10;
            } else if (ch >= 'A' && ch <= 'F') {
                value = ch - 'A' + 10;
            } else if (ch == '\r' || ch == '\n') {
                break;
            } else {
                LOG_FAILURE("Invalid message");
                cmd->type = CMD_TYPE_NONE;
                return false;
            }
            if (high_nibble) {
                cmd->udsRecord.data[cmd->udsRecord.length] = value << 4;
                high_nibble = false;
            } else {
                cmd->udsRecord.data[cmd->udsRecord.length++] |= value;
                high_nibble = true;
            }
        }
        if (!high_nibble) {
            LOG_FAILURE("Invalid message");
            cmd->type = CMD_TYPE_NONE;
            return false;
        }
        return true;
    }
    cmd->type = CMD_TYPE_NONE;
    return false;
}


bool protocol_write(tCommand *cmd, tWriteBuffer *buffer) {
    return false;
}
