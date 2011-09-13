#include "../src/ast-walker/stringstream.c"
#include "../src/ast-walker/memory.h"
#include "../src/ast-walker/memory.c"
#include "../src/ast-walker/t_stream.h"
#include "../src/ast-walker/t_stream.c"

#include <string.h>

int main(void) {
  STREAM s;
  const char *tmp = "hej pa dig";
  int ch, i;

  make_stringstream(&s, tmp, strlen(tmp)+1);

  while((ch = stream_getc(&s)) != EOF)
    putchar(ch);

  printf("END\n");
  stream_close(&s);

  printf("%s\n", tmp);

  make_stringstream(&s, tmp, strlen(tmp)+1);
  for (i = 0; i < 4; i++)
    ch = stream_getc(&s);

  stream_ungetc('.', &s);
  stream_ungetc('J', &s);
  stream_ungetc('O', &s);
  stream_ungetc('H', &s);

  ch = stream_ungetc('2', &s);
  if (ch != EOF) {
    printf("Should get EOF\n");
  } else {
    printf("All ok, got EOF\n");
  }

  printf("UNCHANGED: %s\n", tmp);
  
  while((ch = stream_getc(&s)) != EOF)
    putchar(ch);
  printf("END\n");

  stream_close(&s);
}
