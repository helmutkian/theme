#include "read_stream.h"
#include <stdlib.h>
#include <string.h>

static void _resize_buf(read_stream *rstream);

read_stream *
read_stream_make(FILE *fstream)
{
  read_stream *rstream;

  rstream = malloc(sizeof(read_stream));
  rstream->stream = fstream;
  rstream->max_size = INIT_BUF_SIZE;
  rstream->pos = -1;
  rstream->buf = malloc(sizeof(char) * INIT_BUF_SIZE);

  return rstream;
}

void
read_stream_free(read_stream *rstream)
{
  free(rstream->buf);
  free(rstream);
}

char
read_stream_get_char(read_stream *rstream, char *delims)
{
  char c;
  c = fgetc(rstream->stream);
  if ((c != EOF) && (!delims || !strchr(delims, c))) {
    if (rstream->pos + 1 == rstream->max_size) {
      _resize_buf(rstream);
    }
    rstream->buf[++rstream->pos] = c;
    rstream->buf[rstream->pos + 1] = '\0';
  }

  return c;
}


  
void
_resize_buf(read_stream *rstream)
{
  char *new_buf;
  
  rstream->max_size *= 2;
  new_buf = malloc(sizeof(char) * rstream->max_size);
  memcpy(new_buf, rstream->buf, rstream->pos + 1);
  free(rstream->buf);
  rstream->buf = new_buf;
}
  
  
void
read_stream_unwind(read_stream *rstream)
{
  while(rstream->pos >= 0) {
    ungetc(rstream->buf[rstream->pos--], rstream->stream);
  }
}

char *
read_stream_get_buf(read_stream *rstream)
{
  return rstream->buf;
}

int 
read_stream_get_pos(read_stream *rstream)
{
  return rstream->pos;
}
