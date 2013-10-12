
#include <repl.h>

repl_session_opts opts;

static char *
eval (repl_session_t *sess, char *buf);

static void
print (repl_session_t *sess, char *buf);

static void
error (repl_session_t *sess, char *err);

int
main (void) {
  int rc;
  // opts
  opts.prompt = "repl>";
  opts.eval_cb = eval;
  opts.print_cb = print;
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
  if (feof(sess->stdin)) sess->rc = 0;
  else return buf;
  return NULL;
}

static void
print (repl_session_t *sess, char *buf) {
  fprintf(sess->stdout, "%s\n", buf);
  repl_loop(sess);
}

static void
error (repl_session_t *sess, char *err) {
  fprintf(sess->stderr, "error: '%s'\n", err);
  repl_loop(sess);
}
