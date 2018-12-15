/*************************************************************************
    > File Name: main.cpp
    > Author: nian
    > Blog: https://whoisnian.com
    > Mail: zhuchangbao1998@gmail.com
    > Created Time: 2018年12月14日 星期五 00时06分07秒
 ************************************************************************/
#include<iostream>
#include<algorithm>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#include"LexicalAnalysis.h"
#include"SyntacticAnalysis.h"

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
"<term_tail> := <mulop> <factor>",
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

    Scan scanner("test.txt");
    Syntax syn(scanner);
    
    if(syn.program() == 0&&syn.scanner.next().isEOF())
        printf("=====YES=====\n");
    else
    {
        printf("=====NO=====\n");
        printf("Error at %d %s %s\n", syn.errPos, syn.errMessage.c_str(), syn.scanner.tokens.at(syn.errPos).name.c_str());
    }
    for(int i = 0;i < (int)syn.syntaxProcess.size();i++)
        printf("%d\t%s\n", syn.syntaxProcess.at(i), synText[syn.syntaxProcess.at(i)]);
    return 0;
}
