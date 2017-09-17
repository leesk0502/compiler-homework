%{
#include <stdio.h>
#include <string.h>

#define BACKGROUND_COLOR "#000000"
#define DEFAULT_COLOR "#d1d1d1"
#define IDENTIFIER_COLOR "#d1d1d1"
#define ERROR_COLOR "#ff0000"
#define KEYWORD_COLOR "#e66170"
#define STRING_COLOR "#00c4c4"
#define LITERAL_COLOR "#008c00"
#define SEPARATOR_COLOR "#b060b0"
#define OPERATOR_COLOR "#d2cd86"
#define COMMENT_COLOR "#9999a9"
#define COMMENT_BOLD_COLOR "#8695b8"
%}

%x comment
ONELINE_COMMENT (\/{2}.+\n)

Identifier [a-zA-Z_$][a-zA-Z_$0-9]*
TYPE "int"|"float"|"boolean"|"byte"|"char"|"double"|"long"|"short"
MODIFIER "default"|"public"|"protected"|"private"|"final"|"abstract"|"strictfp"

Keyword abstract|assert|boolean|break|byte|case|catch|char|class|const|continue|default|do|double|else|enum|extends|final|finally|float|for|if|goto|implements|import|instanceof|int|interface|long|native|new|package|private|protected|public|return|short|static|strictfp|super|switch|synchronized|this|throw|thorws|transient|try|void|volatile|while

Literal {IntegerLiteral}|{FloatingPointLiteral}|{BooleanLiteral}|{CharacterLiteral}|{NullLiteral}

IntegerLiteral {DecimalIntegerLiteral}|{HexIntegerLiteral}|{OctalIntegerLiteral}|{BinaryIntegerLiteral}
DecimalIntegerLiteral {DecimalNumeral}{IntegerTypeSuffix}?
HexIntegerLiteral {HexNumeral}{IntegerTypeSuffix}?
OctalIntegerLiteral {OctalNumeral}{IntegerTypeSuffix}?
BinaryIntegerLiteral {BinaryNumeral}{IntegerTypeSuffix}?
IntegerTypeSuffix [lL]

DecimalNumeral 0|{NonZeroDigit}{Digits}?|{NonZeroDigit}{Underscores}{Digits}
Digits {Digit}|{Digit}{DigitsAndUnderscores}?{Digit}
Digit 0|{NonZeroDigit}
NonZeroDigit [1-9]
DigitsAndUnderscores {DigitOrUnderscore}+
DigitOrUnderscore {Digit}|_
Underscores _+

HexNumeral 0x{HexDigits}|0X{HexDigits}
HexDigits {HexDigit}|{HexDigit}{HexDigitsAndUnderscores}?{HexDigit}
HexDigit [0-9a-eA-F]
HexDigitsAndUnderscores {HexDigitOrUnderscore}+
HexDigitOrUnderscore {HexDigit}|_

OctalNumeral 0{OctalDigits}|0{Underscores}{OctalDigits}
OctalDigits {OctalDigit}+|{OctalDigit}{OctalDigitsAndUnderscores}?{OctalDigit}
OctalDigit [0-7]
OctalDigitsAndUnderscores {OctalDigitOrUnderscore}+
OctalDigitOrUnderscore {OctalDigit}|_

BinaryNumeral 0b{BinaryDigits}|0B{BinaryDigits}
BinaryDigits {BinaryDigit}|{BinaryDigit}{BinaryDigitsAndUnderscores}?{BinaryDigit}
BinaryDigit [01]
BinaryDigitsAndUnderscores {BinaryDigitOrUnderscore}+
BinaryDigitOrUnderscore {BinaryDigit}|_

FloatingPointLiteral {DecimalFloatingPointLiteral}|{HexadecimalFloatingPointLiteral}
DecimalFloatingPointLiteral {Digits}.{Digits}?{ExponentPart}?{FloatTypeSuffix}?|.{Digits}{ExponentPart}?{FloatTypeSuffix}?|{Digits}{ExponentPart}{FloatTypeSuffix}?|{Digits}{ExponentPart}?{FloatTypeSuffix}
ExponentPart {ExponentIndicator}{SignedInteger}
ExponentIndicator [eE]
SignedInteger {Sign}?{Digits}
Sign [+-]
FloatTypeSuffix [fFdD]
HexadecimalFloatingPointLiteral {HexSignificand}{BinaryExponent}{FloatTypeSuffix}?
HexSignificand {HexNumeral}|{HexNumeral}.|0x{HexDigits}?.{HexDigits}|0X{HexDigits}?.{HexDigits}
BinaryExponent {BinaryExponentIndicator}{SignedInteger}
BinaryExponentIndicator [pP]

BooleanLiteral true|false

CharacterLiteral "'"{SingleCharacter}"'"|"'"{EscapeSequence}"'"
SingleCharacter [^\'\\]

StringLiteral "\""{StringCharacters}?"\""
StringCharacters {StringCharacter}+
StringCharacter [^\"\\]|{EscapeSequence}

EscapeSequence \\b|\\t|\\n|\\f|\\r|\\\"|\\\'|\\\\
NullLiteral null

Separator [\(\)\{\}\[\]\;\,\.]
Operator "="|">"|"<"|"!"|"~"|"?"|":"|"=="|"<="|">="|"!="|"&&"|"||"|"++"|"--"|"+"|"-"|"*"|"/"|"&"|"|"|"^"|"%"|"<<"|">>"|">>>"|"+="|"-="|"*="|"/="|"&="|"|="|"^="|"%="|"<<="|">>="|">>>="

%%

{Literal}     { fprintf(yyout, "<font color='%s'>%s</font>", LITERAL_COLOR, yytext); }
{Keyword}     { fprintf(yyout, "<font color='%s'>%s</font>", KEYWORD_COLOR, yytext); }
{Identifier}  { fprintf(yyout, "<font color='%s'>%s</font>", IDENTIFIER_COLOR, yytext); }
{Separator}   { fprintf(yyout, "<font color='%s'>%s</font>", SEPARATOR_COLOR, yytext); }
{Operator}    { fprintf(yyout, "<font color='%s'>%s</font>", OPERATOR_COLOR, yytext); }
{StringLiteral}|{CharacterLiteral} { fprintf(yyout, "<font color='%s'>%s</font>", STRING_COLOR, yytext); }
{TYPE}[" "][^a-zA-Z].*[\n] {
  fprintf(yyout, "<font color='%s'><u>%s</u></font>", ERROR_COLOR, yytext);
}

{MODIFIER}?[" "]{TYPE}(\[\])?[" "][^a-zA-Z][0-9a-zA-Z_]*\(({TYPE}(\[\])?[" "][a-zA-Z][0-9a-zA-Z]*)?([,][" "]{TYPE}(\[\])?[" "][a-zA-Z][0-9a-zA-Z_]*)*\)  {
  fprintf(yyout, "<font color='%s'><u>%s</u></font>", ERROR_COLOR, yytext);
}

{ONELINE_COMMENT} {
  fprintf(yyout, "<font color='%s'><strong>%s</strong></font>", COMMENT_COLOR, yytext);
}

"/*" {
  BEGIN(comment);
  fprintf(yyout, "<font color='%s'><strong><xmp>", COMMENT_COLOR);
  ECHO;
}
<comment>\@[^*\n]* {
  fprintf(yyout, "</xmp></strong></font>");
  fprintf(yyout, "<font color='%s'><strong><xmp>%s</xmp></strong></font>", COMMENT_BOLD_COLOR, yytext);
  fprintf(yyout, "<font color='%s'><strong><xmp>", COMMENT_COLOR);
}
<comment>[^*@\n]* ECHO;
<comment>"*"+[^*/@\n]* ECHO;
<comment>\n ECHO;
<comment>"*"+"/" {
  ECHO;
  BEGIN(INITIAL);
  fprintf(yyout, "</xmp></strong></font>");
}

%%
int main( int argc, char * argv[] )
{
  ++argv, --argc; /* skip over program name */
  if ( argc < 1 )
  {
    printf("Usage : ./javaHighlighter <file name>\n");
    return 0;
  }
  yyin = fopen( argv[0], "r");
  yyout = fopen( "index.html", "w");
  
  fprintf(yyout, "<!Doctype><html>");
  fprintf(yyout, "  <head><style>xmp {white-space: pre-wrap; display: inherit; }</style></head>");
  fprintf(yyout, "  <body>");
  fprintf(yyout, "  <pre style='color:%s;background:%s;'>", IDENTIFIER_COLOR, BACKGROUND_COLOR);
  
  while(!feof(yyin))
  {
    yylex();
  }
  fprintf(yyout, "  </pre>");
  fprintf(yyout, "  </body>");
  fprintf(yyout, "</html>");

  fclose(yyin);
  fclose(yyout);
  return 0;
}
