#include "stream.h"
#include <stdlib.h>

// 1. Initialize and open the source file (.mtnl)
SourceStream* init_stream(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        return NULL; // File opening failed
    }

    SourceStream *stream = (SourceStream*)malloc(sizeof(SourceStream));
    if (!stream) {
        fclose(f);
        return NULL;
    }

    stream->file = f;
    stream->current_line = 1;
    stream->current_column = 0;
    stream->has_peeked = 0;
    stream->peek_buffer = '\0';

    return stream;
}

// 2. Fetch the next character and update line/column coordinates
char next_char(SourceStream *stream) {
    if (!stream) return EOF;

    // If a character was peeked previously, return it and clear the flag
    if (stream->has_peeked) {
        stream->has_peeked = 0;
        // Update coordinates based on the peeked char before returning
        if (stream->peek_buffer == '\n') {
            stream->current_line++;
            stream->current_column = 0;
        } else {
            stream->current_column++;
        }
        return stream->peek_buffer;
    }

    int c = fgetc(stream->file);
    if (c == EOF) return EOF;

    // Coordinate tracking
    if (c == '\n') {
        stream->current_line++;
        stream->current_column = 0;
    } else {
        stream->current_column++;
    }

    return (char)c;
}

// 3. Look ahead at the next character without consuming it (Buffering)
char peek_char(SourceStream *stream) {
    if (!stream) return EOF;

    if (stream->has_peeked) {
        return stream->peek_buffer;
    }

    int c = fgetc(stream->file);
    if (c == EOF) return EOF;

    stream->peek_buffer = (char)c;
    stream->has_peeked = 1;
    return stream->peek_buffer;
}

// 4. Safely close the file stream and free memory
void close_stream(SourceStream *stream) {
    if (stream) {
        if (stream->file) {
            fclose(stream->file);
        }
        free(stream);
    }
}
