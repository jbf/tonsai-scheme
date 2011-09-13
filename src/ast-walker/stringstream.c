#include "stringstream.h"

#include <string.h>
#include <stdio.h>

#include "memory.h"

int stringstream_getc(STREAM *stream);
int stringstream_ungetc(int ch, STREAM *stream);
int stringstream_flush(STREAM *stream);
int stringstream_close(STREAM *stream);

typedef struct string_info {
  char *str;
  size_t len;
  size_t index;
} string_info;

int make_stringstream(STREAM *stream, const char *str, size_t size) {
  char *target;
  string_info *s = new_malloc(string_info);

  /* This will at most trigger a read overflow + pagefault. Sweet */
  if(strlen(str) >= size) { /* str + '\0' must fit in size */
    memset(stream, 0, sizeof(STREAM)/sizeof(char));
    return 0;
  } else if (size < 1) {
    memset(stream, 0, sizeof(STREAM)/sizeof(char));
    return 0;
  }
  
  target = malloc_or_bail(size);
  strcpy(target, str); // we got here, len(target) >= len(str + 1) 
  s->len = size-1;
  s->index = 0;
  s->str = target;

  stream->payload = s;

  stream->fgetc_fun = stringstream_getc;
  stream->ungetc_fun = stringstream_ungetc;
  stream->flush_fun = stringstream_flush;
  stream->close_fun = stringstream_close;
  return 1;
}

int stringstream_getc(STREAM *stream) {
  string_info *s = (string_info *)stream->payload;
  if (s->index >= s->len)
    return EOF;
  return (int)((unsigned char)*(s->str + s->index++));
}

int stringstream_ungetc(int ch, STREAM *stream) {
  string_info *s = (string_info *)stream->payload;
  if (s->index <= 0)
    return EOF;
  
  *(s->str + --(s->index)) = (char)ch;
  return ch;
}

int stringstream_flush(STREAM *stream) {
  return 1;
}

int stringstream_close(STREAM *stream) {
  free_malloced(((string_info *)stream->payload)->str);
  free_malloced(stream->payload);
  stream->payload = NULL;
  return 1;
}
