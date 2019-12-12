%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <stdarg.h>
  #include <string.h>

  #include "journal.h"

  extern int yylex();
  extern int yyparse();
  extern FILE *yyin;
  extern int my_byte;
  extern int my_line;
  extern int my_col;

  void yyerror(const char *s);

  void lgf(const char *, ...);

  struct journal * root = 0;
  struct day * current_day = 0;

%}


%union {
  char *sval;
  struct date * dval;
  struct time * tval;
  struct timerange * trval;
  struct record * rval;
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

%type <dval> dateline

%type <sval> ws
%type <sval> integer
%type <sval> word
%type <sval> symbol
%type <sval> text
%type <sval> any

%type <sval> tag

%type <tval> timestamp

%type <trval> timerange

%%

logfile:
  lines                             { lgf("LOGFILE\n"); }
  ;

lines:
  lines line
  | line
  ;

line:
  dateline nl                       { 
                                        lgf("dateline\n");
                                        current_day->date = $1;
                                        add_day(root, current_day);
                                        current_day = new_day();
                                    }
  | timerange text nl               { 
                                      lgf("logline\n");
                                      printf("TR %p TXT %p\n", $1, $2);
                                    }
  | timestamp ws timerange text nl  {
                                      lgf("timestamped logline\n");
                                    }
  | timestamp text nl               {
                                      lgf("timestamped line\n");
                                    }
  | text nl                         { 
                                      lgf("noteline\n");
                                    }
  | nl                              { 
                                      lgf("empty line\n");
                                    }
  ;

text:
  text any
  | any
  ;

any:
  ws
  | integer
  | word
  | symbol
  | tag
  ;

nl:         NL                      { ; }
ws:         WS                      { ; }
dateline:   DATE                    { printf(" > dateline %p %p\n", $$, $1); $1[16] = 0; $$ = set_date($$, strndup($1+6, 10)); }
timerange:  TIMERANGE               { printf(" > timerange %p %p\n", $$, $1); $$ = set_timerange($$, strndup($1, 10)); }
timestamp:  TIMESTAMP               { printf(" > timestamp %p %p\n", $$, $1); $$ = set_timestamp($$, strndup($1, 10)); }
tag:        TAG                     { printf(" > tag %p %p\n", $$, $1); $$ = $1; }
integer:    INT                     { printf(" > integer %p %p\n", $$, $1); $$ = $1; }
word:       WORD                    { printf(" > word %p %p\n", $$, $1);$$ = $1; }
symbol:     SYMBOL                  { printf(" > symbol %p %p\n",$$, $1); $$ = $1; }

%%

int main(int argc, char ** argv) {
  fprintf(stdout, "\nbegin\n");
  root = new_journal();
  current_day = new_day();

  fprintf(stdout, "\n parse\n");
  yyin = stdin;
  if(argc>1) {
    yyin = fopen(argv[1], "r");
  }
  yyparse();

  fprintf(stdout, "\n print\n");
  print_journal(0, root);
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
