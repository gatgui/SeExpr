#!/usr/bin/env sh

flex -oSeExprParserLexIn.cpp src/SeExpr/SeExprParserLex.l
sed -e "s/SeExprwrap(n)/SeExprwrap()/g" -e "s/yy/SeExpr/g" -e "s/YY/SeExprYY/g"  SeExprParserLexIn.cpp | tee SeExprParserLex.cpp src/SeExpr/generated/SeExprParserLex.cpp > /dev/null
bison "--defines" "--verbose" "--fixed-output-files" "-p" "SeExpr" "src/SeExpr/SeExprParser.y"
sed -e "s/yy/SeExpr/g" -e "s/YY/SeExprYY/g" y.tab.h | tee SeExprParser.tab.h src/SeExpr/generated/SeExprParser.tab.h > /dev/null
sed -e "s/yy/SeExpr/g" -e "s/YY/SeExprYY/g" y.tab.c | tee SeExprParser.cpp  "src/SeExpr/generated/SeExprParser.cpp" > /dev/null

rm y.*
rm SeExprParser*

