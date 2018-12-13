#include <cstdio>
#include "LexicalAnalysis.h"
#include "SyntacticAnalysis.h"
#include "paramtable.h"
using namespace std;

int main(void)
{
    Scan scanner("test.txt");
    Syntax syn(scanner);
    
    if(syn.program() == 0&&syn.scanner.next().isEOF())
        printf("=====YES=====\n");
    else
    {
        printf("=====NO=====\n");
        printf("Error at %d %s %s\n", syn.errPos, syn.errMessage.c_str(), syn.scanner.tokens.at(syn.errPos).name.c_str());
    }
    gen4elem();
    return 0;
}
