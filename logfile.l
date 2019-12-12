%{

#include <stdio.h>
#include <stdlib.h>

#include "logfile.tab.h"

const int MAX_LEN = 1024;

int my_byte = 0;
int my_col = 0;
int my_line = 1;

enum yytokentype position(enum yytokentype type, const char * val) {
  const int len = strnlen(val, MAX_LEN);
  yylval.sval = strndup(val, len);

  my_col += len;
  my_byte += len;
  char * type_name = "other";

  switch (type) {
    case NL:
      type_name = "newline";
      my_line += 1;
      my_col = 0;
      break;
    case WS:
      type_name = "whitespace";
      break;
    case INT:
      type_name = "integer";
      yylval.ival = atoi(val);
      break;
    case TAG:
      type_name = "tag";
      break;
    case WORD:
      type_name = "word";
      break;
    case TIMESTAMP:
      type_name = "timestamp";
      break;
    case TIMERANGE:
      type_name = "timerange";
      break;
    default:
      break;
  }
  // fprintf(stderr, "\t%d:%-5d (%d) %s\n", my_line, my_col, my_byte, type_name);
  return type;
}

%}

%option noyywrap

NEWLINE                                       [\r]?[\n]
WHITESPACE                                    [ \t]
DIGIT                                         [0-9]
LETTER                                        [a-zA-Z]
SYMBOL                                        [!"#$%&'()*+,./:;<=>?@\[\\\]^_`{|}-]
YEAR                                          [0-9][0-9][0-9][0-9]
MONTH                                         ([0][1-9]|[1][0-2])
DAY                                           ([0][1-9]|[12][0-9]|[3][01])
HOUR                                          ([01][0-9]|[2][0-3])
MINUTE                                        [0-5][0-9]
SECOND                                        [0-5][0-9]

%%

^"Date: "{YEAR}-{MONTH}-{DAY}{WHITESPACE}*$   { return position(DATE, yytext); }
{NEWLINE}                                     { return position(NL, yytext); }
{WHITESPACE}+                                 { return position(WS, yytext); }
{DIGIT}+                                      { return position(INT, yytext); }
#{LETTER}+                                    { return position(TAG, yytext); }
\[{HOUR}:{MINUTE}:{SECOND}]                   { return position(TIMESTAMP, yytext); }
{HOUR}{MINUTE}-{HOUR}{MINUTE}                 { return position(TIMERANGE, yytext); }
{LETTER}({LETTER}|{DIGIT}|{SYMBOL})*          { return position(WORD, yytext); }
{SYMBOL}                                      { return position(SYMBOL, yytext); }

%%

