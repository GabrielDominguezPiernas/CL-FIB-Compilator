//////////////////////////////////////////////////////////////////////
//
//    Asl - Another simple language (grammar)
//
//    Copyright (C) 2017-2022  Universitat Politecnica de Catalunya
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU General Public License
//    as published by the Free Software Foundation; either version 3
//    of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//    contact: José Miguel Rivero (rivero@cs.upc.edu)
//             Computer Science Department
//             Universitat Politecnica de Catalunya
//             despatx Omega.110 - Campus Nord UPC
//             08034 Barcelona.  SPAIN
//
//////////////////////////////////////////////////////////////////////

grammar Asl;

//////////////////////////////////////////////////
/// Parser Rules
//////////////////////////////////////////////////

// A program is a list of functions
program : function+ EOF
        ;

// A function has a name, a list of parameters and a list of statements
function
        : FUNC ID '(' parameters? ')' (':' basic_type)? declarations statements ENDFUNC
        ;

parameters
        : parameter (COMMA parameter)*
        ;

parameter
        : ID ':' type 
        ;

declarations
        : (variable_decl)*
        ;

variable_decl
        : VAR ID (COMMA ID)* ':' type
        ;

type    
        : basic_type | array_type | tuple_type
        ;

basic_type
        : INT | FLOAT | BOOL | CHAR
        ;
array_type
        : ARRAY LSQ INTVAL RSQ OF basic_type
        ;
tuple_type
        : KEYL basic_type (COMMA basic_type)* KEYR
        ;

statements
        : (statement)*
        ;

// The different types of instructions
statement
          // Assignment
        : left_expr ASSIGN expr ';'                             # assignStmt
          // if-then-else statement (else is optional)
        | IF expr THEN statements (ELSE statements)? ENDIF      # ifStmt
          // while-do statement
        | WHILE expr DO statements ENDWHILE                     # whileStmt
          // for statement
        | FOR ident IN RANGE LP (expr (COMMA expr)*) RP DO statements ENDFOR # forStmt
        
        // | PACK expr (COMMA expr)* INTO ident ';'                # packStmt
        // | UNPACK ident INTO expr (COMMA expr)* ';'              # unpackStmt

        // | MAP ident INTO ident USING ident ';'                  # mapStmt 

        | ident '(' (expr (COMMA expr)*)? ')' ';'               # procCall
          // Read a variable
        | READ left_expr ';'                                    # readStmt
          // Write an expression
        | WRITE expr ';'                                        # writeExpr
          // Write a string
        | WRITE STRING ';'                                      # writeString
          // Return statement
        | RETURN (expr)? ';'                                    # returnStmt
        ;

// Grammar for left expressions (l-values in C++)
left_expr
        : ident
        | ident LSQ expr RSQ
        | ident KEYL expr KEYR
        ;

// Grammar for expressions with boolean, relational and aritmetic operators
expr    : LP expr RP                                            # parenthesis
        | ident KEYL expr KEYR                                  # tuple
        | ident LSQ expr RSQ                                    # array
        | ident LP (expr (COMMA expr)*)? RP                     # functionIdent
        //| ident op=MUL ident                                    # prodEscalar
        // | ident op=DOTMAX                                       # maxDot
        // | op=FILTER ident INTO ident USING ident                # filter
        // | expr op=POT expr                                      # potencia
        | op=(PLUS | SUB | NOT) expr                            # unary

        | expr op=(MUL | DIV | MOD) expr                        # arithmetic
        | expr op=(PLUS | SUB) expr                             # arithmetic

        | expr op=(EQUAL | NEG | GT | GE | LE | LT) expr        # relational

        | expr op=AND expr                                      # boolean
        | expr op=OR expr                                       # boolean

        // | MAX LP ((expr (COMMA expr)*)?) RP                     # max
        // | op=SUM LP ((expr (COMMA expr)*)?) RP                  # sum

        | values                                                # value
        
        | ident                                                 # exprIdent
        ;

// Values
values:  INTVAL                                                
        | FLOATVAL                                             
        | BOOLVAL                                            
        | CHARVAL
        ;

INTVAL    : ('0'..'9')+ ;
FLOATVAL  : INTVAL DOT INTVAL;
BOOLVAL   : TRUE | FALSE ;
CHARVAL   : '\'' ( ESC_SEQ | ~('\\'|'\'') ) '\'' ;

// Identifiers
ident   : ID
        ;

//////////////////////////////////////////////////
/// Lexer Rules
//////////////////////////////////////////////////


//////////////////////////////////////////////////
/// Exam Rules
POT       : '**' ;
MAP       : 'map' ;
USING     : 'using' ;
SUM       : 'sum' ;
FILTER    : 'filter' ;
DOTMAX    : '.max' ;
//////////////////////////////////////////////////


ASSIGN    : '=' ;
EQUAL     : '==' ;

NEG       : '!=' ;
GT        : '>' ;
GE        : '>=' ;
LT        : '<' ;
AP        : '"' ;
AT        : '\'' ;
LE        : '<=' ;
LP        : '(' ;
RP        : ')' ;
LSQ       : '[' ;
RSQ       : ']' ;
KEYL      : '{' ;
KEYR      : '}' ;
COLON     : ':' ;
SCOLON    : ';' ;
DOT       : '.' ;
COMMA     : ',' ;
HASH      : '#' ;
PLUS      : '+' ;
SUB       : '-' ;
MUL       : '*' ;
DIV       : '/' ;
MOD       : '%' ;

VAR       : 'var';
INT       : 'int';
FLOAT     : 'float';
BOOL      : 'bool';
CHAR      : 'char';
ARRAY     : 'array' ;

IF        : 'if' ;
THEN      : 'then' ;
ELSE      : 'else' ;
ENDIF     : 'endif' ;
WHILE     : 'while' ;
DO        : 'do' ;
ENDWHILE  : 'endwhile' ;
FUNC      : 'func' ;
ENDFUNC   : 'endfunc' ;
READ      : 'read' ;
WRITE     : 'write' ;
RETURN    : 'return' ;
OF        : 'of' ;
//MAX       : 'max' ;
FOR       : 'for' ;
IN        : 'in' ;
RANGE     : 'range' ;
ENDFOR    : 'endfor' ;
PACK      : 'pack' ;
UNPACK    : 'unpack' ;
INTO      : 'into' ;



TRUE      : 'true' ;
FALSE     : 'false' ;
AND       : 'and' ;
OR        : 'or' ;
NOT       : 'not' ;

ID        : ('a'..'z'|'A'..'Z') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')* ;

// Strings (in quotes) with escape sequences
STRING    : '"' ( ESC_SEQ | ~('\\'|'"') )* '"' ;

fragment
ESC_SEQ   : '\\' ('b'|'t'|'n'|'f'|'r'|'"'|'\''|'\\') ;

// Comments (inline C++-style)
COMMENT   : '//' ~('\n'|'\r')* '\r'? '\n' -> skip ;

// White spaces
WS        : (' '|'\t'|'\r'|'\n')+ -> skip ;
// Alternative description
// WS        : [ \t\r\n]+ -> skip ;
