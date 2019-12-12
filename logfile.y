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
  struct record * current_record = 0;

%}


%union {
  int ival;
  char *sval;
  struct date * dval;
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

%type <rval> ws
%type <rval> integer
%type <rval> word
%type <rval> symbol
%type <rval> tag
%type <rval> timestamp
%type <rval> timerange

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
dateline:   DATE                    { $1[16] = 0; $$ = set_date(0, $1+6); }
timerange:  TIMERANGE               { $$->begin = set_begin(0, $1); $$->end = set_end(0, $1); }
timestamp:  TIMESTAMP               { $$->recorded_at = set_time(0, $1); }
tag:        TAG                     { $$->tags = add_tag($$->tags, $1); }
integer:    INT                     { $$->notes = append_note($$->notes, $1); }
word:       WORD                    { $$->notes = append_note($$->notes, $1); }
symbol:     SYMBOL                  { $$->notes = append_note($$->notes, $1); }

%%

int main(int argc, char ** argv) {
  root = new_journal();
  current_day = new_day();
  current_record = new_record();

  yyparse();

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
