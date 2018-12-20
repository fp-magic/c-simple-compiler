#include<cstdio>
#include<vector>
#include"LexicalAnalysis.h"
#include"SyntacticAnalysis.h"
#include"paramtable.h"

using namespace std;

int main(void)
{
    char synText[100][100] = {
"UNKNOWN ERROR",
"<program> := <declaration_list>",
"<declaration_list> := <declaration> <declaration_list_tail>",
"<declaration_list_tail> := <declaration> <deliaration_list_tail>",
"<declaration_list_tail> := <empty>",
"<declaration> := <var_declaration>",
"<declaration> := <fun_declaration>",
"<var_declaration> := <type_specifier> ID ;",
"<var_declaration> := <type_specifier> ID [ NUM ] ;",
"<type_specifier> := int",
"<type_specifier> := void",
"<fun_declaration> := <type_specifier> ID ( <params> ) <compound_stmt>",
"<params> := <param_list>",
"<params> := void",
"<param_list> := <parm> <parm_list_tail>",
"<parm_list_tail> := , <param> <parm_list_tail>",
"<parm_list_tail> := <empty>",
"<param> := <type_specifier> ID",
"<param> := <type_specifier> ID [ ]",
"<compound_stmt> := { <local_declarations> <statement_list> }",
"<local_declarations> := <var_declaration> <local_declarations_tail>",
"<local_declarations> := <empty>",
"<local_declarations_tail> := <var_declaration> <local_declarations_tail>",
"<local_declarations_tail> := <empty>",
"<statement_list> := <statement> <statement_list_tail>",
"<statement_list> := <empty>",
"<statement_list_tail> := <statement> <statement_list_tail>",
"<statement_list_tail> := <empty>",
"<statement> := <expression_stmt>",
"<statement> := <compound_stmt>",
"<statement> := <selection_stmt>",
"<statement> := <iteration_stmt>",
"<statement> := <return_stmt>",
"<expression_stmt> := <expression> ;",
"<expression_stmt> := ;",
"<selection_stmt> := if ( <expression> ) <statement>",
"<selection_stmt> := if ( <expression> ) <statement> else <statement>",
"<iteration_stmt> := while ( <expression> ) <statement>",
"<return_stmt> := return ;",
"<return_stmt> := return <expression> ;",
"<expression> := <var> = <expression>",
"<expression> := <simple_expression>",
"<var> := ID",
"<var> := ID [ <expression> ]",
"<simple_expression> := <additive_expression> <relop> <additive_expression>",
"<simple_expression> := <additive_expression>",
"<relop> := <=",
"<relop> := <",
"<relop> := >",
"<relop> := >=",
"<relop> := ==",
"<relop> := !=",
"<additive_expression> := <term> <additive_expression_tail>",
"<additive_expression_tail> := <addop> <term> <additive_expression_tail>",
"<additive_expression_tail> := <empty>",
"<addop> := +",
"<addop> := -",
"<term> := <factor> <term_tail>",
"<term_tail> := <mulop> <factor> <term_tail>",
"<term_tail> := <empty>",
"<mulop> := *",
"<mulop> := /",
"<factor> := ( <expression> )",
"<factor> := <var>",
"<factor> := <call>",
"<factor> := NUM",
"<call> := ID ( <args> )",
"<args> := <arg_list>",
"<args> := <empty>",
"<arg_list> := <expression> <arg_list_tail>",
"<arg_list_tail> := , <expression> <arg_list_tail>",
"<arg_list_tail> := <empty>",
    };

    // 构造时进行词法分析
    Scan scanner("test.txt");
    Syntax syn(scanner);
    
    // syn.program() 进行语法分析，记录过程
    if(syn.program() == 0&&syn.scanner.next().isEOF())
    {
        printf("=====YES=====\n");
        // 输出保存的语法分析过程
        for(int i = 0;i < (int)syn.syntaxProcess.size();i++)
            printf("%d\t%s\n", syn.syntaxProcess.at(i), synText[syn.syntaxProcess.at(i)]);

        printf("\n\n\n");
        
        // 输出四元式
        try{
            // 重置下个 Token 位置，在已确定的语法分析过程中为生成四元式做准备
            syn.scanner.curIndex = 0;
            syn.prepare_for_4elem();
            syn.gen4elem();
        }catch(...){}

        if(syn.haveErr)
        {
            printf("%s\n", syn.errMessagePT);
            return 0;
        }

        syn.genValls();
        syn.outputParam();
        syn.genAssembly();
    }
    else
    {
        printf("=====NO=====\n");
        printf("Error at %d %s %s\n", syn.errPos, syn.errMessage.c_str(), syn.scanner.tokens.at(syn.errPos).name.c_str());
    }
    return 0;
}
