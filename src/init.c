
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "term.h"
#include "repl.h"
#include "loop.h"


// signal handlers

static void
handle_sigterm (int param) {
  int rc = 1;
  switch (param) {
    case SIGINT:
    case SIGTERM:
      rc = 0;
      break;
  }

  printf("\n");
  exit(rc);
}


// default callbacks

static void
default_eval_cb (repl_session_t *sess, char *cmd);

static void
default_print_cb (repl_session_t *sess, char *cmd);

static void
default_error_cb (repl_session_t *sess, char *error);

static char *__serror = NULL;

repl_session_t *
repl_session_new (repl_session_opts *opts) {
  repl_session_t *session = malloc(sizeof(repl_session_t));

  if (NULL == session)
    return NULL;

  if (NULL != opts->prompt)
    session->prompt = strdup(opts->prompt);


  // callbacks
  if (NULL != opts->eval_cb)
    session->eval = opts->eval_cb;
  else session->eval = default_eval_cb;

  if (NULL != opts->error_cb)
    session->error = opts->error_cb;
  else session->error = default_error_cb;

  if (NULL != opts->print_cb)
    session->print = opts->print_cb;
  else session->print = default_print_cb;

  // streams
  if (NULL != opts->in)
    session->in = opts->in;
  else session->in = stdin;

  if (NULL != opts->out)
    session->out = opts->out;
  else session->out = stdout;

  if (NULL != opts->err)
    session->err = opts->err;
  else session->err = stderr;


  return session;
}


int
repl_session_start (repl_session_t *session) {
  int rc = 0;
  void (*prev_sig)(int);

  // set up signal handlers
  prev_sig = signal(SIGINT, handle_sigterm);

  // if ignored then bind SIGTERM to SIG_IGN
  if (SIG_IGN == (prev_sig = signal(SIGTERM, handle_sigterm))) {
    signal(SIGTERM, SIG_IGN);
  }

  term_reset();
  rc = repl_read(session);

  if (1 == rc) {
    session->error(session, repl_session_error());
  }

  return rc;
}


char *
repl_session_error () {
  return __serror;
}

void
repl_session_destroy (repl_session_t *session) {

  free(session);
}



static void
default_eval_cb (repl_session_t *sess, char *cmd) {
  sess->print(sess, cmd);
}

static void
default_print_cb (repl_session_t *sess, char *buf) {
  fprintf(sess->out, "%s", buf);
  repl_loop(sess);
}


void
default_error_cb (repl_session_t *sess, char *error) {
  fprintf(sess->err, "%s", error);
  repl_loop(sess);
}

