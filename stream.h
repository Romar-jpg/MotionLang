#ifndef STREAM_H
#define STREAM_H

#include <stdio.h>

// Structure to track file stream, coordinates, and buffering
typedef struct {
    FILE *file;
    int current_line;
    int current_column;
    char peek_buffer;
    int has_peeked;
} SourceStream;

// Function Prototypes for Module 1
SourceStream* init_stream(const char *filename);
char next_char(SourceStream *stream);
char peek_char(SourceStream *stream);
void close_stream(SourceStream *stream);

#endif