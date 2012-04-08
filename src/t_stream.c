#include "t_stream.h"


int stream_getc(STREAM *stream) {
  return (stream->fgetc_fun)(stream);
}

int stream_ungetc(int ch, STREAM *stream) {
  return (stream->ungetc_fun)(ch, stream);
}

int stream_flush(STREAM *stream) {
  return (stream->flush_fun)(stream);
}

int stream_close(STREAM *stream) {
  return (stream->close_fun)(stream);
}
