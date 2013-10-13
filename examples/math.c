
#include <repl.h>

repl_session_opts opts;

static char *
eval (repl_session_t *sess, char *buf);

static void
error (repl_session_t *sess, char *err);

int
main (void) {
  int rc;
  // opts
  opts.prompt = "math>";
  opts.eval_cb = eval;
  opts.error_cb = error;

  // initiate
  repl_session_t *sess = repl_session_new(&opts);

  // start
  rc = repl_session_start(sess);

  // destroy
  repl_session_destroy(sess);
  printf("\n");
  return (-1 == rc || 0 == rc)? 0 : rc;
}


static char *
eval (repl_session_t *sess, char *buf) {
  // got nothing
  if (feof(sess->stdin)) {
    sess->rc = 0;
    return NULL;
  }

  char op[1];
  char *result = NULL;
  char tmp[4096];
  int lval = 0;
  int rval = 0;

  sscanf(buf, "%d %s %d", &lval, op, &rval);
  
  if (NULL == op) {
    sess->rc = 1;
    return repl_session_set_error("Missing operator\n");
  }

  switch (op[0]) {
    case '+':
      sprintf(tmp, "%d", (lval + rval));
      break;

    case '-':
      sprintf(tmp, "%d", (lval - rval));
      break;
    
    case '*':
      sprintf(tmp, "%d", (lval * rval));
      break;
    
    case '/':
      sprintf(tmp, "%d", (lval / rval));
      break;
    
    case '%':
      sprintf(tmp, "%d", (lval % rval));
      break;

    default:
      sess->rc = 1;
      return repl_session_set_error("Invalid operator\n");
  }

  if (NULL == tmp) return NULL;

  result = tmp;
  result[strlen(tmp)] = '\0';

  return result;
}


static void
error (repl_session_t *sess, char *err) {
  fprintf(sess->stderr, "error: '%s'\n", err);
  repl_loop(sess);
}
