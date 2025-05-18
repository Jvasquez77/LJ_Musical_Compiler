%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "declaration.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include "token.hpp"

extern int yyerror(const char* msg);
%}

%option yylineno

ESPACIO     [ \t\n]
OCTAVA      [0-9]
LETRA       [A-Za-z]
DIGITO      [0-9]
ENTERO      -?{DIGITO}+
COMENTARIO  "//".*
NOTA_LAT    "Do"|"Re"|"Mi"|"Fa"|"Sol"|"La"|"Si"
NOTA_ENG    "C"|"D"|"E"|"F"|"G"|"A"|"B"
ALTER       "b"|"#"

%%
{ESPACIO}       { /* Ignorar espacios */ }
{COMENTARIO}    { /* Ignorar comentarios */ }

"Tonalidad"     { return TOKEN_TONALIDAD; }
"Tempo"         { return TOKEN_TEMPO; }
"Compas"        { return TOKEN_COMPAS; }

"Blanca"        { return TOKEN_BLANCA; }
"Negra"         { return TOKEN_NEGRA; }
"Corchea"       { return TOKEN_CORCHEA; }
"Semicorchea"   { return TOKEN_SEMICORCHEA; }

"M"             { return TOKEN_MAYOR; }
"m"             { return TOKEN_MENOR; }

{ENTERO}        { yylval.ival = atoi(yytext); return TOKEN_NUMERO; }
"/"             { return TOKEN_BARRA; }

"Do"|"C"        { return TOKEN_NOTA_DO; }
"Re"|"D"        { return TOKEN_NOTA_RE; }
"Mi"|"E"        { return TOKEN_NOTA_MI; }
"Fa"|"F"        { return TOKEN_NOTA_FA; }
"Sol"|"G"       { return TOKEN_NOTA_SOL; }
"La"|"A"        { return TOKEN_NOTA_LA; }
"Si"|"B"        { return TOKEN_NOTA_SI; }

"#"             { return TOKEN_SOSTENIDO; }
"b"             { return TOKEN_BEMOL; }

("Do"|"Re"|"Mi"|"Fa"|"Sol"|"La"|"Si"|"C"|"D"|"E"|"F"|"G"|"A"|"B")[#b]?[0-9] { return TOKEN_NOTA_COMPLETA; }

[a-zA-Z_][a-zA-Z0-9_]* { return TOKEN_IDENTIFIER; }

.               { 
                  char msg[100];
                  snprintf(msg, sizeof(msg), "Carácter no reconocido: %s", yytext);
                  yyerror(msg); 
                }

%% 

int yywrap() { return 1; } 