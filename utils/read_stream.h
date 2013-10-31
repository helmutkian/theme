#ifndef READ_STREAM_H
#define READ_STREAM_H

#include <stdio.h>

#define INIT_BUF_SIZE 256

typedef struct {
  FILE *stream;
  char *buf;
  int pos, max_size;
} read_stream;

read_stream *read_stream_make(FILE *fstream);
void read_stream_free(read_stream *rstream);
char read_stream_get_char(read_stream *rstream, char *delims);
void read_stream_unwind(read_stream *rstream);
char *read_stream_get_buf(read_stream *rstream);
int read_stream_get_pos(read_stream *rstream);


#endif
