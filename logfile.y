%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdarg.h>

  #include "journal.h"

  extern int yylex();
  extern int yyparse();
  extern int my_byte;
  extern int my_line;
  extern int my_col;

  void yyerror(const char *s);

  void lgf(const char *, ...);

  struct journal * root = 0;
  struct day * current_day = 0;

%}


%union {
  int ival;
  char *sval;
}

%token <sval> NL
%token <sval> WS
%token <sval> INT
%token <sval> WORD
%token <sval> SYMBOL
%token <sval> TAG
%token <sval> TIMESTAMP
%token <sval> TIMERANGE
%token <sval> DATE

%%

logfile:
  lines { lgf("LOGFILE\n"); }
  ;

lines:
  lines line
  | line
  ;

line:
  dateline nl                       { lgf("dateline\n"); }
  | timestamp ws timerange text nl  { lgf("timestamped logline\n"); }
  | timestamp text nl               { lgf("timestamped line\n"); }
  | timerange text nl               { lgf("logline\n"); }
  | text nl                         { lgf("noteline\n"); }
  | nl                              { lgf("empty line\n"); }
  ;

text:
  text any
  | any
  |
  ;

any:
  ws
  | integer
  | word
  | symbol
  | tag
  ;

nl:         NL
ws:         WS
integer:    INT
word:       WORD
symbol:     SYMBOL
tag:        TAG
dateline:   DATE
timestamp:  TIMESTAMP
timerange:  TIMERANGE

%%

int main(int argc, char ** argv) {
  root = add_day(root, 0);
  current_day = root->data;

  yyparse();

  fprintf(stdout, "\ndone\n");
}

void yyerror(const char *s) {
  lgf("parse error on line %d column %d (byte %d): %s\n", my_line, my_col, my_byte, s);
  exit(-1);
}

void lgf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}
