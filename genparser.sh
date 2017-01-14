#!/usr/bin/env sh

flex -oExprParserLexIn.cpp src/SeExpr/ExprParserLex.l
sed -e "s/SeExprwrap(n)/SeExprwrap()/g" -e "s/yy/SeExpr2/g" -e "s/YY/SeExprYY/g"  ExprParserLexIn.cpp | tee ExprParserLex.cpp src/SeExpr/generated/ExprParserLex.cpp > /dev/null
bison "--defines" "--verbose" "--fixed-output-files" "-p" "SeExpr2" "src/SeExpr/ExprParser.y"
sed -e "s/yy/SeExpr2/g" -e "s/YY/SeExprYY/g" y.tab.h | tee ExprParser.tab.h src/SeExpr/generated/ExprParser.tab.h > /dev/null
sed -e "s/yy/SeExpr2/g" -e "s/YY/SeExprYY/g" y.tab.c | tee ExprParser.cpp  "src/SeExpr/generated/ExprParser.cpp" > /dev/null

flex -oExprSpecParserLexIn.cpp src/ui/ExprSpecParserLex.l
sed -e "s/SeExprSpecwrap(n)/SeExprSpecwrap()/g" -e "s/yy/SeExpr2Spec/g" -e "s/YY/SeExprSpecYY/g"  ExprSpecParserLexIn.cpp | tee ExprSpecParserLex.cpp src/ui/generated/ExprSpecParserLex.cpp > /dev/null
bison "--defines" "--verbose" "--fixed-output-files" "-p" "SeExpr2Spec" "src/ui/ExprSpecParser.y"
sed -e "s/yy/SeExpr2Spec/g" -e "s/YY/SeExprSpecYY/g" y.tab.h | tee ExprSpecParser.tab.h src/ui/generated/ExprSpecParser.tab.h > /dev/null
sed -e "s/yy/SeExpr2Spec/g" -e "s/YY/SeExprSpecYY/g" y.tab.c | tee ExprSpecParser.cpp  "src/ui/generated/ExprSpecParser.cpp" > /dev/null

rm y.*
rm ExprParser*
rm ExprSpecParser*
