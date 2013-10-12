
#include "repl.h"


int
repl_read (repl_session_t *sess) {
  int rc = 0;
  repl_read_data_t data;
  char buf[REPL_MAX_BUFFER_LENGTH];
  printf("%s ", sess->prompt);
  fgets(buf, REPL_MAX_BUFFER_LENGTH, sess->in);
  if (NULL == buf || '\0' == buf[0]) {
    return -1;
  }
  data.buffer = buffer_new_with_string(buf);
  sess->data = data;
  rc = repl_eval(sess);
  return rc;
}
