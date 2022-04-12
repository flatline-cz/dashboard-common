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
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "buffer.h"

void buffer_make_nonblocking(tHandle handle) {
    fcntl(handle, F_SETFL, fcntl(handle, F_GETFL) | O_NONBLOCK);
}

void buffer_write_add(tWriteBuffer *context, const char *cmd, unsigned length) {
    if (length + 1 > CMD_LENGTH) {
        fprintf(stderr, "Command too long (%s)\n", context->name);
        exit(1);
    }
    // have enough space?
    if (context->length == BUFFER_COUNT) {
        fprintf(stderr, "Buffer full (%s)\n", context->name);
        exit(1);
    }

    unsigned index = context->position + context->length;
    if (index >= BUFFER_COUNT)
        index -= BUFFER_COUNT;

    context->buffers[index].position = 0;
    context->buffers[index].length = length;
    memcpy(context->buffers[index].buffer, cmd, length);

    context->length++;
}


void buffer_write_send(tWriteBuffer *context, int handle) {
    while (context->length) {
        // get current buffer
        tCMDBuffer *cmd = context->buffers + context->position;

        // write down as much as possible
        int written = (int) write(handle, cmd->buffer + cmd->position, cmd->length - cmd->position);
        if (written < 0) {
            if (errno == EWOULDBLOCK) {
                return;
            }
            perror(__FUNCTION__);
            exit(1);
        }

        // update context
        cmd->position += written;
        if (cmd->position > cmd->length) {
            fprintf(stderr, "Internal error: Write buffer inconsistent (%s)", context->name);
            exit(1);
        }
        if (cmd->position < cmd->length)
            return;
        cmd->position = 0;
        cmd->length = 0;

        // next command
        if (++context->position == BUFFER_COUNT)
            context->position = 0;
        context->length--;
    }
}

static bool extract_command(tReadBuffer *context, hProcessor processor) {
    unsigned position;
    // skip all new lines
    for (position = 0; position < context->length; position++) {
        uint8_t ch = context->buffer[position];
        if (ch != '\n' && ch != '\r')
            break;
    }
    if (position >= context->length)
        return false;

    // start pointer
    uint8_t *cmd = context->buffer + position;

    // try to find end
    unsigned length;
    for (length = 0; position < context->length; position++, length++) {
        uint8_t ch = context->buffer[position];
        if (ch == '\n' || ch == '\r')
            break;
    }
    if (position >= context->length)
        return false;

    // include line terminator
    position++;
    length++;

    // call processor
    processor(cmd, length);

    // shift buffer
    if (++position < context->length) {
        memmove(context->buffer, context->buffer + position, context->length - position);
        context->length -= position;
    } else {
        context->length = 0;
    }

    return true;
}

void buffer_read_process(tReadBuffer *context, int handle, hProcessor processor) {
    while (1) {
        // read data
        unsigned size = CMD_LENGTH - context->length;
        if (size) {
            int length = (int) read(handle, context->buffer + context->length, size);
            if (length < 0) {
                if (errno != EWOULDBLOCK) {
                    perror("Data read");
                    exit(1);
                }
                length = 0;
            }
            if (length == 0)
                return;
            size = length;
        }

        // process buffer
        context->length += size;
        while (!extract_command(context, processor));
    }
}


#define SELECT_FDSET(fds, fd, maxfd) (((fd)>(maxfd))?(fd):(maxfd)); FD_SET(fd, &(fds))

void buffer_process_read(tReadChannel channels[]) {
    fd_set rfds;
    FD_ZERO(&rfds);
    struct timeval timeout = {.tv_sec=(SELECT_TIMEOUT / 1000), .tv_usec=(SELECT_TIMEOUT % 1000) * 1000};

    int maxHandle = -1;

    // fill read selectors
    tReadChannel *channel;
    for (channel = channels; channel->buffer; channel++) {
        maxHandle = SELECT_FDSET(rfds, *(channel->handle), maxHandle);
    }

    // wait for data
    int retval = select(maxHandle + 1, &rfds, NULL, NULL, &timeout);
    if (retval < 0) {
        perror("Waiting for data");
        exit(1);
    }

    // process data
    for (channel = channels; channel->buffer; channel++) {
        if (!FD_ISSET(*(channel->handle), &rfds))
            continue;
        buffer_read_process(channel->buffer, *(channel->handle), channel->processor);
    }
}
