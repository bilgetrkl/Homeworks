#ifndef _yy_defines_h_
#define _yy_defines_h_

#define KW_AND 257
#define KW_OR 258
#define KW_NOT 259
#define KW_EQUAL 260
#define KW_LESS 261
#define KW_SET 262
#define KW_DEFVAR 263
#define KW_DEFFUN 264
#define KW_FOR 265
#define KW_IF 266
#define KW_WHILE 267
#define KW_LIST 268
#define KW_APPEND 269
#define KW_CONCAT 270
#define KW_NIL 271
#define KW_EXIT 272
#define KW_LOAD 273
#define KW_DISP 274
#define KW_TRUE 275
#define KW_FALSE 276
#define OP_PLUS 277
#define OP_MINUS 278
#define OP_DIV 279
#define OP_MULT 280
#define OP_OP 281
#define OP_CP 282
#define OP_COMMA 283
#define OP_APO 284
#define VALUEI 285
#define VALUEF 286
#define IDENTIFIER 287
#define STRING 288
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
    double dval;
    char *sval;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
