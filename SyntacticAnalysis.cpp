#include <string>
#include "SyntacticAnalysis.h"

Syntax::Syntax()
{
    errPos = -1;
    errMessage = "";
}

Syntax::Syntax(Scan scanner)
{
    errPos = -1;
    errMessage = "";
    this->scanner = scanner;
}

int Syntax::program()
{
    #ifdef DEBUG
    printf("----------program\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = declaration_list();
    if(res == 0)
    {
        return 0;   // 1 <program> := <declaration_list>
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::declaration_list()
{
    #ifdef DEBUG
    printf("----------declaration_list\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = declaration();
    if(res == 0)
    {
        res = declaration_list_tail();
        if(res == 0)
        {
            return 0;   // 2 <declaration_list> := <declaration> <declaration_list_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::declaration_list_tail()
{
    #ifdef DEBUG
    printf("----------declaration_list_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = declaration();
    if(res == 0)
    {
        res = declaration_list_tail();
        if(res == 0)
        {
            return 0;   // 3 <declaration_list_tail> := <declaration> <deliaration_list_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 4 <declaration_list_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 4 <declaration_list_tail> := <empty>
    }
}

int Syntax::declaration()
{
    #ifdef DEBUG
    printf("----------declaration\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = var_declaration();
    if(res == 0)
    {
        return 0;   // 5 <declaration> := <var_declaration>
    }
    else
    {
        scanner.setIndex(ori_index);
        res = fun_declaration();
        if(res == 0)
        {
            return 0;   // 6 <declaration> := <fun_declaration>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
}

int Syntax::var_declaration()
{
    #ifdef DEBUG
    printf("----------var_declaration\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = type_specfier();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isIdentifier())
        {
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(";"))
            {
                return 0;   // 7 <var_declaration> := <type_specifier> ID ;
            }
            else if(temp.isDelimiter()&&temp.name == std::string("["))
            {
                temp = scanner.next();
                if(temp.isInt())
                {
                    temp = scanner.next();
                    if(temp.isDelimiter()&&temp.name == std::string("]"))
                    {
                        temp = scanner.next();
                        if(temp.isDelimiter()&&temp.name == std::string(";"))
                        {
                            return 0;   // 8 <var_declaration> := <type_specifier> ID [ NUM ] ;
                        }
                        else
                        {
                            if(scanner.curIndex - 1 > errPos)
                            {
                                errPos = scanner.curIndex - 1;
                                errMessage = "need ; before";
                            }
                            scanner.setIndex(ori_index);
                            return -1;
                        }
                    }
                    else
                    {
                        if(scanner.curIndex - 1 > errPos)
                        {
                            errPos = scanner.curIndex - 1;
                            errMessage = "need ] before";
                        }
                        scanner.setIndex(ori_index);
                        return -1;
                    }
                }
                else
                {
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need NUM before";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "need ; or [ before";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ID before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::fun_declaration()
{
    #ifdef DEBUG
    printf("----------fun_declaration\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = type_specfier();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isIdentifier())
        {
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string("("))
            {
                res = params();
                if(res == 0)
                {
                    temp = scanner.next();
                    if(temp.isDelimiter()&&temp.name == std::string(")"))
                    {
                        res = compound_stmt();
                        if(res == 0)
                        {
                            return 0;   // 11 <fun_declaration> := <type_specifier> ID ( <params> ) <compound_stmt>
                        }
                        else
                        {
                            scanner.setIndex(ori_index);
                            return res;
                        }
                    }
                    else
                    {
                        if(scanner.curIndex - 1 > errPos)
                        {
                            errPos = scanner.curIndex - 1;
                            errMessage = "need ) before";
                        }
                        scanner.setIndex(ori_index);
                        return -1;
                    }
                }
                else
                {
                    scanner.setIndex(ori_index);
                    return res;
                }
            }
            else
            {
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "need ( before";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ID before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::type_specfier()
{
    #ifdef DEBUG
    printf("----------type_specfier\n");
    #endif
    int ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("int"))
    {
        return 0;   // 9 <type_specifier> := int
    }
    else if(temp.isKeyWord()&&temp.name == std::string("void"))
    {
        return 0;   // 10 <type_specifier> := void
    }
    else
    {
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::params()
{
    #ifdef DEBUG
    printf("----------params\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = param_list();
    if(res == 0)
    {
        return 0;   // 12 <params> := <param_list>
    }
    else
    {
        scanner.setIndex(ori_index);
        Token temp = scanner.next();
        if(temp.isKeyWord()&&temp.name == std::string("void"))
        {
            return 0;   // 13 <params> := void
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need void before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
}

int Syntax::param_list()
{
    #ifdef DEBUG
    printf("----------param_list\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = param();
    if(res == 0)
    {
        res = param_list_tail();
        if(res == 0)
        {
            return 0;   // 14 <param_list> := <parm> <parm_list_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::param_list_tail()
{
    #ifdef DEBUG
    printf("----------param_list_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string(","))
    {
        res = param();
        if(res == 0)
        {
            res = param_list_tail();
            if(res == 0)
            {
                return 0;   // 15 <parm_list_tail> := , <param> <parm_list_tail>
            }
            else
            {
                scanner.setIndex(ori_index);
                return 0;   // 16 <parm_list_tail> := <empty>
            }
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 16 <parm_list_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 16 <parm_list_tail> := <empty>
    }
}

int Syntax::param()
{
    #ifdef DEBUG
    printf("----------param\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = type_specfier();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isIdentifier())
        {
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string("["))
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string("]"))
                {
                    return 0;   // 18 <param> := <type_specifier> ID [ ]
                }
                else
                {
                    scanner.back();
                    scanner.back();
                    return 0;   // 17 <param> := <type_specifier> ID
                }
            }
            else
            {
                scanner.back();
                return 0;   // 17 <param> := <type_specifier> ID
            }
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ID before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::compound_stmt()
{
    #ifdef DEBUG
    printf("----------compound_stmt\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("{"))
    {
        res = local_declarations();
        if(res == 0)
        {
            res = statement_list();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string("}"))
                {
                    return 0;   // 19 <compound_stmt> := { <local_declarations> <statement_list> }
                }
                else
                {
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need } before";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need { before";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::local_declarations()
{
    #ifdef DEBUG
    printf("----------local_declarations\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = var_declaration();
    if(res == 0)
    {
        res = local_declarations_tail();
        if(res == 0)
        {
            return 0;   // 20 <local_declarations> := <var_declaration> <local_declarations_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 21 <local_declarations> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 21 <local_declarations> := <empty>
    }
}

int Syntax::local_declarations_tail()
{
    #ifdef DEBUG
    printf("----------local_declarations_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = var_declaration();
    if(res == 0)
    {
        res = local_declarations_tail();
        if(res == 0)
        {
            return 0;   // 22 <local_declarations_tail> := <var_declaration> <local_declarations_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 23 <local_declarations_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 23 <local_declarations_tail> := <empty>
    }
}

int Syntax::statement_list()
{
    #ifdef DEBUG
    printf("----------statement_list\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = statement();
    if(res == 0)
    {
        res = statement_list_tail();
        if(res == 0)
        {
            return 0;   // 24 <statement_list> := <statement> <statement_list_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 25 <statement_list> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 25 <statement_list> := <empty>
    }
}

int Syntax::statement_list_tail()
{
    #ifdef DEBUG
    printf("----------statement_list_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = statement();
    if(res == 0)
    {
        res = statement_list_tail();
        if(res == 0)
        {
            return 0;   // 26 <statement_list_tail> := <statement> <statement_list_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 27 <statement_list_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 27 <statement_list_tail> := <empty>
    }
}

int Syntax::statement()
{
    #ifdef DEBUG
    printf("----------statement\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = expression_stmt();
    if(res == 0)
    {
        return 0;   // 28 <statement> := <expression_stmt>
    }
    else
    {
        scanner.setIndex(ori_index);
        res = compound_stmt();
        if(res == 0)
        {
            return 0;   // 29 <statement> := <compound_stmt>
        }
        else
        {
            scanner.setIndex(ori_index);
            res = selection_stmt();
            if(res == 0)
            {
                return 0;   // 30 <statement> := <selection_stmt>
            }
            else
            {
                scanner.setIndex(ori_index);
                res = iteration_stmt();
                if(res == 0)
                {
                    return 0;   // 31 <statement> := <iteration_stmt>
                }
                else
                {
                    scanner.setIndex(ori_index);
                    res = return_stmt();
                    if(res == 0)
                    {
                        return 0;   // 32 <statement> := <return_stmt>
                    }
                    else
                    {
                        scanner.setIndex(ori_index);
                        return res;
                    }
                }
            }
        }
    }
}

int Syntax::expression_stmt()
{
    #ifdef DEBUG
    printf("----------expression_stmt\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = expression();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string(";"))
        {
            return 0;   // 33 <expression_stmt> := <expression> ;
        }
        else
        {
            scanner.setIndex(ori_index);
            Token temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(";"))
            {
                return 0;   // 34 <expression_stmt> := ;
            }
            else
            {
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "need ; before";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        Token temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string(";"))
        {
            return 0;   // 34 <expression_stmt> := ;
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ; before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
}

int Syntax::selection_stmt()
{
    #ifdef DEBUG
    printf("----------selection_stmt\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("if"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    res = statement();
                    if(res == 0)
                    {
                        int another_index = scanner.curIndex;
                        temp = scanner.next();
                        if(temp.isKeyWord()&&temp.name == std::string("else"))
                        {
                            res = statement();
                            if(res == 0)
                            {
                                return 0;   // 36 <selection_stmt> := if ( <expression> ) <statement> else <statement>
                            }
                            else
                            {
                                scanner.setIndex(another_index);
                                return 0;   // 35 <selection_stmt> := if ( <expression> ) <statement>
                            }
                        }
                        else
                        {
                            scanner.back();
                            return 0;   // 35 <selection_stmt> := if ( <expression> ) <statement>
                        }
                    }
                    else
                    {
                        scanner.setIndex(ori_index);
                        return res;
                    }
                }
                else
                {
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need ) before";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ( before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need if before";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::iteration_stmt()
{
    #ifdef DEBUG
    printf("----------iteration_stmt\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("while"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    res = statement();
                    if(res == 0)
                    {
                        return 0;   // 37 <iteration_stmt> := while ( <expression> ) <statement>
                    }
                    else
                    {
                        scanner.setIndex(ori_index);
                        return res;
                    }
                }
                else
                {
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need ) before";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ( before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need while before";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::return_stmt()
{
    #ifdef DEBUG
    printf("----------return_stmt\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isKeyWord()&&temp.name == std::string("return"))
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string(";"))
        {
            return 0;   // 38 <return_stmt> := return ;
        }
        else
        {
            scanner.back();
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(";"))
                {
                    return 0;   // 39 <return_stmt> := return <expression> ;
                }
                else
                {
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need ; before";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need return before";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::expression()
{
    #ifdef DEBUG
    printf("----------expression\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = var();
    if(res == 0)
    {
        Token temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("="))
        {
            res = expression();
            if(res == 0)
            {
                return 0;   // 40 <expression> := <var> = <expression>
            }
            else
            {
                scanner.setIndex(ori_index);
                res = simple_expression();
                if(res == 0)
                {
                    return 0;   // 41 <expression> := <simple_expression>
                }
                else
                {
                    scanner.setIndex(ori_index);
                    return res;
                }
            }
        }
        else
        {
            scanner.setIndex(ori_index);
            res = simple_expression();
            if(res == 0)
            {
                return 0;   // 41 <expression> := <simple_expression>
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        res = simple_expression();
        if(res == 0)
        {
            return 0;   // 41 <expression> := <simple_expression>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
}

int Syntax::var()
{
    #ifdef DEBUG
    printf("----------var\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isIdentifier())
    {
        int another_index = scanner.curIndex;
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == "[")
        {
            res = expression();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == "]")
                {
                    return 0;   // 43 <var> := ID [ <expression> ]
                }
                else
                {
                    scanner.setIndex(another_index);
                    return 0;   // 42 <var> := ID
                }
            }
            else
            {
                scanner.setIndex(another_index);
                return 0;   // 42 <var> := ID
            }
        }
        else
        {
            scanner.back();
            return 0;   // 42 <var> := ID
        }
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need ID before";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::simple_expression()
{
    #ifdef DEBUG
    printf("----------simple_expression\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = additive_expression();
    if(res == 0)
    {
        int another_index = scanner.curIndex;
        res = relop();
        if(res == 0)
        {
            scanner.back();
            Token temp = scanner.next();    // temp.name: "<=" "<" ">" ">=" "==" "!="
            res = additive_expression();
            if(res == 0)
            {
                return 0;   // 44 <simple_expression> := <additive_expression> <relop> <additive_expression>
            }
            else
            {
                scanner.setIndex(another_index);
                return 0;   // 45 <simple_expression> := <additive_expression>
            }
        }
        else
        {
            scanner.setIndex(another_index);
            return 0;   // 45 <simple_expression> := <additive_expression>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::additive_expression()
{
    #ifdef DEBUG
    printf("----------additive_expression\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = term();
    if(res == 0)
    {
        res = additive_expression_tail();
        if(res == 0)
        {
            return 0;   // 52 <additive_expression> := <term> <additive_expression_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::additive_expression_tail()
{
    #ifdef DEBUG
    printf("----------additive_expression_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = addop();
    if(res == 0)
    {
        scanner.back();
        Token temp = scanner.next();    // temp.name: "+" "-"
        res = term();
        if(res == 0)
        {
            res = additive_expression_tail();
            if(res == 0)
            {
                return 0;   // 53 <additive_expression_tail> := <addop> <term> <additive_expression_tail>
            }
            else
            {
                scanner.setIndex(ori_index);
                return 0;   // 54 <additive_expression_tail> := <empty>
            }
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 54 <additive_expression_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 54 <additive_expression_tail> := <empty>
    }
}

int Syntax::relop()
{
    #ifdef DEBUG
    printf("----------relop\n");
    #endif
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("<="))
    {
        return 0;   // 46 <relop> := <=
    }
    else if(temp.isDelimiter()&&temp.name == std::string("<"))
    {
        return 0;   // 47 <relop> := <
    }
    else if(temp.isDelimiter()&&temp.name == std::string(">"))
    {
        return 0;   // 48 <relop> := >
    }
    else if(temp.isDelimiter()&&temp.name == std::string(">="))
    {
        return 0;   // 49 <relop> := >=
    }
    else if(temp.isDelimiter()&&temp.name == std::string("=="))
    {
        return 0;   // 50 <relop> := ==
    }
    else if(temp.isDelimiter()&&temp.name == std::string("!="))
    {
        return 0;   // 51 <relop> := !=
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need <=, <, >, >=, == or != before";
        }
        scanner.back();
        return -1;
    }
}

int Syntax::term()
{
    #ifdef DEBUG
    printf("----------term\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = factor();
    if(res == 0)
    {
        res = term_tail();
        if(res == 0)
        {
            return 0;   // 57 <term> := <factor> <term_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::term_tail()
{
    #ifdef DEBUG
    printf("----------term_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = mulop();
    if(res == 0)
    {
        scanner.back();
        Token temp = scanner.next();    // temp.name: "*" "/"
        res = factor();
        if(res == 0)
        {
            return 0;   // 58 <term_tail> := <mulop> <factor>
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 59 <term_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 59 <term_tail> := <empty>
    }
}

int Syntax::addop()
{
    #ifdef DEBUG
    printf("----------addop\n");
    #endif
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("+"))
    {
        return 0;   // 55 <addop> := +
    }
    else if(temp.isDelimiter()&&temp.name == std::string("-"))
    {
        return 0;   // 56 <addop> := -
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need + or - before";
        }
        scanner.back();
        return -1;
    }
}

int Syntax::factor()
{
    #ifdef DEBUG
    printf("----------factor\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("("))
    {
        res = expression();
        if(res == 0)
        {
            temp = scanner.next();
            if(temp.isDelimiter()&&temp.name == std::string(")"))
            {
                return 0;   // 62 <factor> := ( <expression> )
            }
            else
            {
                if(scanner.curIndex - 1 > errPos)
                {
                    errPos = scanner.curIndex - 1;
                    errMessage = "need ) before";
                }
                scanner.setIndex(ori_index);
                return -1;
            }
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else if(temp.isInt())
    {
        return 0;   // 65 <factor> := NUM
    }
    else
    {
        scanner.setIndex(ori_index);
        res = call();
        if(res == 0)
        {
            return 0;   // 64 <factor> := <call>
        }
        else
        {
            scanner.setIndex(ori_index);
            res = var();
            if(res == 0)
            {
                return 0;   // 63 <factor> := <var>
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
    }
}

int Syntax::mulop()
{
    #ifdef DEBUG
    printf("----------mulop\n");
    #endif
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string("*"))
    {
        return 0;   // 60 <mulop> := *
    }
    else if(temp.isDelimiter()&&temp.name == std::string("/"))
    {
        return 0;   // 61 <mulop> := /
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need * or / before";
        }
        scanner.back();
        return -1;
    }
}

int Syntax::call()
{
    #ifdef DEBUG
    printf("----------call\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isIdentifier())
    {
        temp = scanner.next();
        if(temp.isDelimiter()&&temp.name == std::string("("))
        {
            res = args();
            if(res == 0)
            {
                temp = scanner.next();
                if(temp.isDelimiter()&&temp.name == std::string(")"))
                {
                    return 0;   // 66 <call> := ID ( <args> )
                }
                else
                {
                    if(scanner.curIndex - 1 > errPos)
                    {
                        errPos = scanner.curIndex - 1;
                        errMessage = "need ) before";
                    }
                    scanner.setIndex(ori_index);
                    return -1;
                }
            }
            else
            {
                scanner.setIndex(ori_index);
                return res;
            }
        }
        else
        {
            if(scanner.curIndex - 1 > errPos)
            {
                errPos = scanner.curIndex - 1;
                errMessage = "need ( before";
            }
            scanner.setIndex(ori_index);
            return -1;
        }
    }
    else
    {
        if(scanner.curIndex - 1 > errPos)
        {
            errPos = scanner.curIndex - 1;
            errMessage = "need ID before";
        }
        scanner.setIndex(ori_index);
        return -1;
    }
}

int Syntax::args()
{
    #ifdef DEBUG
    printf("----------args\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = arg_list();
    if(res == 0)
    {
        return 0;   // 67 <args> := <arg_list>
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 68 <args> := <empty>
    }
}

int Syntax::arg_list()
{
    #ifdef DEBUG
    printf("----------arg_list\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    res = expression();
    if(res == 0)
    {
        res = arg_list_tail();
        if(res == 0)
        {
            return 0;   // 69 <arg_list> := <expression> <arg_list_tail>
        }
        else
        {
            scanner.setIndex(ori_index);
            return res;
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return res;
    }
}

int Syntax::arg_list_tail()
{
    #ifdef DEBUG
    printf("----------arg_list_tail\n");
    #endif
    int res, ori_index;
    ori_index = scanner.curIndex;
    Token temp = scanner.next();
    if(temp.isDelimiter()&&temp.name == std::string(","))
    {
        res = expression();
        if(res == 0)
        {
            res = arg_list_tail();
            if(res == 0)
            {
                return 0;   // 70 <arg_list_tail> := , <expression> <arg_list_tail>
            }
            else
            {
                scanner.setIndex(ori_index);
                return 0;   // 71 <arg_list_tail> := <empty>
            }
        }
        else
        {
            scanner.setIndex(ori_index);
            return 0;   // 71 <arg_list_tail> := <empty>
        }
    }
    else
    {
        scanner.setIndex(ori_index);
        return 0;   // 71 <arg_list_tail> := <empty>
    }
}
