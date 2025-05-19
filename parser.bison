%{
    /* C++ preamble */
    #include <string>
    #include <vector>
    #include <cctype>
    #include <cstring>
    #include "declaration.hpp"
    #include "expression.hpp"
    #include "statement.hpp"
    #include "ast_node_interface.hpp"
    
    extern int yylex();
    extern int yylineno;
    extern char* yytext;
    void yyerror(const char* msg);
    MusicProgram* parser_result = nullptr;

    // Estructura auxiliar para pasar las tres declaraciones juntas
    /*struct Encabezado {
        TempoDeclaration* tempo;
        TimeSignatureDeclaration* compas;
        KeyDeclaration* key;
    };*/
    
    // Función auxiliar para extraer la octava de una nota completa
    int extraer_octava(const char* nota_completa) {
        int len = strlen(nota_completa);
        // La octava siempre es el último carácter
        if (len > 0) {
            char ultimo = nota_completa[len-1];
            if (isdigit(ultimo)) {
                return ultimo - '0';
            }
        }
        yyerror("Error interno: Nota sin octava válida detectada");
        return -1; // 
    }

    // Función auxiliar para extraer solo el nombre de la nota sin la octava
    std::string extraer_nombre_nota(const char* nota_completa) {
        int len = strlen(nota_completa);
        
        // Buscar dónde termina el nombre de la nota y comienza la octava
        // La octava siempre es el último carácter, si es un dígito
        if (len > 0 && isdigit(nota_completa[len-1])) {
            return std::string(nota_completa, len-1);
        }
        
        // Si no hay octava, devolver todo el string
        return std::string(nota_completa);
    }
%}

%union {  /* YYSTYPE */
    int    ival;
    char*  sval;
    NoteExpression*      noteExpr;
    DurationExpression*  durExpr;
    NoteStatement*       stmt;
    TempoDeclaration*    tempoDecl;
    TimeSignatureDeclaration* tsDecl;
    KeyDeclaration*      keyDecl;
    MusicProgram*        program;
    std::string*         strval;
    KeyMode              keyMode;
    Encabezado*          header;
    std::vector<NoteStatement*>* noteList;
}

%token TOKEN_TEMPO TOKEN_COMPAS TOKEN_TONALIDAD
%token TOKEN_SOSTENIDO TOKEN_BEMOL
%token <ival> TOKEN_NUMERO
%token TOKEN_BLANCA TOKEN_NEGRA TOKEN_CORCHEA TOKEN_SEMICORCHEA
%token TOKEN_NOTA_DO TOKEN_NOTA_RE TOKEN_NOTA_MI TOKEN_NOTA_FA
%token TOKEN_NOTA_SOL TOKEN_NOTA_LA TOKEN_NOTA_SI
%token TOKEN_MAYOR TOKEN_MENOR
%token TOKEN_BARRA
%token TOKEN_NOTA_COMPLETA
%token TOKEN_IDENTIFIER

%type <program> programa
%type <tempoDecl> tempo_decl
%type <tsDecl>   compas_decl
%type <keyDecl>  key_decl
%type <noteExpr> pitch_expr
%type <durExpr>  dur_expr
%type <stmt>     nota_stmt
%type <header>   encabezado
%type <noteList> lista_notas
%type <strval>   nota_base
%type <keyMode>  modo

%% /* ---------- grammar ---------- */

programa
    : encabezado lista_notas
      {
        parser_result = new MusicProgram();
        parser_result->add_declaration($1->tempo);
        parser_result->add_declaration($1->compas);
        parser_result->add_declaration($1->key);
        for(NoteStatement* n : *$2) parser_result->add_statement(n);
        $$ = parser_result;
        delete $1; // Liberamos la estructura auxiliar
        delete $2; // Liberamos el vector
      }
    ;

encabezado
    : tempo_decl compas_decl key_decl
      { 
        $$ = new Encabezado{$1, $2, $3};
      }
    ;

tempo_decl
    : TOKEN_TEMPO TOKEN_NUMERO
      { $$ = new TempoDeclaration($2); }
    ;

compas_decl
    : TOKEN_COMPAS TOKEN_NUMERO TOKEN_BARRA TOKEN_NUMERO
      { $$ = new TimeSignatureDeclaration($2, $4); }
    ;

key_decl
    : TOKEN_TONALIDAD nota_base modo
      { 
        $$ = new KeyDeclaration(*$2, $3);
        delete $2; // Liberamos el string
      }
    ;

nota_base
    : TOKEN_NOTA_DO { $$ = new std::string("Do"); }
    | TOKEN_NOTA_RE { $$ = new std::string("Re"); }
    | TOKEN_NOTA_MI { $$ = new std::string("Mi"); }
    | TOKEN_NOTA_FA { $$ = new std::string("Fa"); }
    | TOKEN_NOTA_SOL { $$ = new std::string("Sol"); }
    | TOKEN_NOTA_LA { $$ = new std::string("La"); }
    | TOKEN_NOTA_SI { $$ = new std::string("Si"); }
    ;

modo
    : TOKEN_MAYOR { $$ = KeyMode::MAYOR; }
    | TOKEN_MENOR { $$ = KeyMode::MENOR; }
    ;

lista_notas
    : /* vacío */
      { $$ = new std::vector<NoteStatement*>; }
    | lista_notas nota_stmt
      { 
        $1->push_back($2);
        $$ = $1;
      }
    ;

nota_stmt
    : pitch_expr dur_expr
      { $$ = new NoteStatement($1, $2); }
    ;

pitch_expr
    : TOKEN_NOTA_COMPLETA
      { 
        std::string nombre_nota = extraer_nombre_nota(yytext);
        int octava = extraer_octava(yytext);
        $$ = new NoteExpression(nombre_nota, octava);
      }
    ;

dur_expr
    : TOKEN_CORCHEA     { $$ = new DurationExpression(DurationType::CORCHEA); }
    | TOKEN_NEGRA       { $$ = new DurationExpression(DurationType::NEGRA); }
    | TOKEN_BLANCA      { $$ = new DurationExpression(DurationType::BLANCA); }
    | TOKEN_SEMICORCHEA { $$ = new DurationExpression(DurationType::SEMICORCHEA); }
    ;

%% /* ---------- user code ---------- */

void yyerror(const char* msg) {
    std::cerr << "Error de parser en línea " << yylineno << ": " << msg << std::endl;
} 