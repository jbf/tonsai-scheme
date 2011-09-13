#ifndef _T_STREAM_H
#define _T_STREAM_H 1

typedef struct STREAM {
  void *payload;
  int (*fgetc_fun)(struct STREAM *stream);
  int (*ungetc_fun)(int ch, struct STREAM *stream);
  int (*flush_fun)(struct STREAM *stream);
  int (*close_fun)(struct STREAM *stream);
} STREAM;

int stream_getc(STREAM *stream);
int stream_ungetc(int ch, STREAM *stream);
int stream_flush(STREAM *stream);
int stream_close(STREAM *stream);

#endif /* _T_STREAM_H */
