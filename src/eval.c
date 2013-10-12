
#include "repl.h"
#include "eval.h"


int
repl_eval (repl_session_t *sess) {
  sess->eval(sess, buffer_string(sess->data.buffer));
  return sess->rc;
}

