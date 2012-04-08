#include "filestream.h"

int filestream_getc(STREAM *stream);
int filestream_ungetc(int ch, STREAM *stream);
int filestream_flush(STREAM *stream);
int filestream_close(STREAM *stream);

int make_filestream(STREAM *stream, FILE *f) {
  stream->payload = f;
  stream->fgetc_fun = filestream_getc;
  stream->ungetc_fun = filestream_ungetc;
  stream->flush_fun = filestream_flush;
  stream->close_fun = filestream_close;
  return 1;
}

int filestream_getc(STREAM *stream) {
  return fgetc((FILE *)stream->payload);
}

int filestream_ungetc(int ch, STREAM *stream) {
  return ungetc(ch, (FILE *)stream->payload);
}

int filestream_flush(STREAM *stream) {
  return fflush((FILE *)stream->payload);
}

int filestream_close(STREAM *stream) {
  FILE *f = (FILE *)stream->payload;
  stream->payload = NULL;
  return fclose(f);
}
