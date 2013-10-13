#include <repl.h>

static char *
eval (repl_session_t *sess, char *buf);

static void
error (repl_session_t *sess, char *err);

 
int
main (void) {
  int rc;
  repl_session_opts opts;

  opts.prompt = "js>";
  opts.eval_cb = eval;
  opts.error_cb = error;

  repl_session_t *sess = repl_session_new(&opts);

  // main loop happens here
  rc = repl_session_start(sess);

  repl_session_destroy(sess);
  
  return rc > 0? 1 : 0;
}


static char *
eval (repl_session_t *sess, char *buf) {
  int rc = 0;
  char cmd[4096];
  char *out_str = NULL;
  char tmp[2048];
  char *str;
  buffer_t *out = buffer_new();
  FILE *node;
  sprintf(cmd, "/usr/bin/env node -e \"%s\" exit;", buf);
  node = popen(cmd, "w");
  if (NULL == node) {
    repl_session_set_error("Couldn't start node");
    sess->rc = 1;
  } else {
    while (NULL != (str = fgets(tmp, sizeof(tmp), node))) {
      buffer_append(out, tmp);
    }
    
    rc = fclose(node);

    if (-1 == rc) {
      fprintf(sess->stderr, "error: %s\n", "Error closing node process.");
    }

    node = NULL;
    out_str = strdup(buffer_string(out));
  }

  buffer_free(out);

  return out_str;
}

static void
error (repl_session_t *sess, char *err) {
  fprintf(sess->stdout, "error: %s", err);
}
