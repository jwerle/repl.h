repl.h
======

Create a repl with eval/print/error hooks with given stdin, stdout, and stderr streams

## install

```sh
$ clib install jwerle/repl.h
```

## usage

The `repl` interface is more or less just an api for defining functions
that answer the "eval", and "print" parts of a REPL routine. The "read"
part is just a read from a defined "stdin" stream that passes a buffer
to the defined "eval" function which returns a `char *` that is passed
to the defined "print" function. The print function if not defined will
print to the defined "stdout" stream.


A simple math repl can be constructed like this:

```c

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
  opts.prompt = "math>";
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

  if (NULL == &lval) {
    sess->rc = 1;
    return repl_session_set_error("Missing left operand\n");
  } else if (NULL == &rval) {
    sess->rc = 1;
    return repl_session_set_error("Missing right operand\n");
  } if (NULL == op) {
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
print (repl_session_t *sess, char *buf) {
  fprintf(sess->stdout, "%s\n", buf);
  repl_loop(sess);
}

static void
error (repl_session_t *sess, char *err) {
  fprintf(sess->stderr, "error: '%s'\n", err);
  repl_loop(sess);
}

```

## license

MIT
