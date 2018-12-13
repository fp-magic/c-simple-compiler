#ifndef SYNTACTICANALYSIS_H_INCLUDED
#define SYNTACTICANALYSIS_H_INCLUDED

#include "LexicalAnalysis.h"
#include "paramtable.h"
class Syntax
{
    public:
        Scan scanner;               // 词法扫描器
        int errPos;                 // 错误位置
        std::string errMessage;     // 错误原因

        Syntax();
        Syntax(Scan scanner);
        
        int program();
        int declaration_list();
        int declaration_list_tail();
        int declaration();
        int var_declaration();
        int fun_declaration();
        int type_specfier();
        int params();
        int param_list();
        int param_list_tail();
        int param();
        int compound_stmt();
        int local_declarations();
        int local_declarations_tail();
        int statement_list();
        int statement_list_tail();
        int statement();
        int expression_stmt();
        int selection_stmt();
        int iteration_stmt();
        int return_stmt();
        int expression();
        int var();
        int simple_expression();
        int additive_expression();
        int additive_expression_tail();
        int relop();
        int term();
        int term_tail();
        int addop();
        int factor();
        int mulop();
        int call();
        int args();
        int arg_list();
        int arg_list_tail();
};

#endif // SYNTACTICANALYSIS_H_INCLUDED
