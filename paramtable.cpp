#include<iostream>
#include<cmath>
#include<cstring>
#include<string>
#include<vector>
#include<stack>
#include<map>
#include<algorithm>
using namespace std;
/*
通过符号表可以很好的指定每个变量在内存中的位置
to-do list：
四元式生成
增加数据类型
*/
struct synbl{
    int name;
    char cat;//f:function,i:int,v:void,l:intarray,c:const
    int addr;
};
struct ainfl{
    int low,up;
    int clen;
};
struct pfinfl{
    int level,off,fn;
    vector<int>param;
    vector<int>synbs;
    int entry;
};
struct consl{
    int data;
};
struct lenl{
    int len;
};
struct elem{
    elem(string ist,int iid1,int iid2,int iid0){
        st=ist;id1=iid1;id2=iid2;id0=iid0;
    }
    void output(){
        cout<<st<<' '<<id1<<' '<<id2<<' '<<id0<<endl;
    }
    string st;
    int id1,id2,id0;
};
vector<synbl>synb;
vector<pfinfl>pfinf;
stack<int>Funcs;
stack<int>calls,callParams;
vector<ainfl>ainf;
vector<consl>cons;
vector<elem>elems;
int size_pfinfl;
int size_ainfl;
int size_consl;
int size_tmp;
stack<char>alNums;
map<int,int>table_synb,table_pfinf;
void initAll(){
    synb.clear();
    pfinf.clear();
    synbl newS;
    pfinfl newP;
    newS.name=0;
    newS.cat='f';
    newP.level=0;
    newP.off=0;
    newP.fn=0;
    newP.param.clear();
    pfinf.push_back(newP);
    newS.addr=pfinf.size()-1;
    synb.push_back(newS);
    while(!Funcs.empty())Funcs.pop();
    Funcs.push(synb[synb.size()-1].addr);
    size_tmp=0;
    elems.clear();
    table_synb.clear();
    table_pfinf.clear();
}
void addFun(int id){
    synbl newS;
    pfinfl newP;
    newS.name=id;
    newS.cat='f';
    newP.level=pfinf[Funcs.top()].level+1;
    newP.off=size_pfinfl;
    newP.fn=0;
    newP.param.clear();
    newP.synbs.clear();
    pfinf.push_back(newP);
    newS.addr=pfinf.size()-1;
    synb.push_back(newS);
    table_pfinf.insert(make_pair(id,synb.size()-1));
    pfinf[Funcs.top()].synbs.push_back(synb.size()-1);
    Funcs.push(synb[synb.size()-1].addr);
}
void backFun(int id){
    Funcs.pop();
}
void addNum(int id,bool isparam=false){
    for(int i=0;i<pfinf[Funcs.top()].synbs.size();i++)
        if(synb[pfinf[Funcs.top()].synbs[i]].name==id){
            cout<<"error: redeclaration of 'int "<<"'"<<endl;
            return;
        }
    synbl newS;
    newS.name=id;
    newS.cat='i';
    synb.push_back(newS);
    if(isparam)
        pfinf[Funcs.top()].param.push_back(synb.size()-1);
    else
        pfinf[Funcs.top()].synbs.push_back(synb.size()-1);
}
void addArray(int id,int len,bool isparam=false){
    for(int i=0;i<pfinf[Funcs.top()].synbs.size();i++)
        if(synb[pfinf[Funcs.top()].synbs[i]].name==id){
            cout<<"error: redeclaration of 'int* "<<"'"<<endl;
            return;
        }
    synbl newS;
    ainfl newA;
    newS.name=id;
    newS.cat='l';
    newA.low=synb.size()+1;
    newA.up=synb.size()+len;
    ainf.push_back(newA);
    newS.addr=ainf.size()-1;
    synb.push_back(newS);
    if(isparam)
        pfinf[Funcs.top()].param.push_back(synb.size()-1);
    else
        pfinf[Funcs.top()].synbs.push_back(synb.size()-1);
    for(int i=1;i<=len;i++)
    {
        synbl newSS;
        newSS.name=-1;
        newSS.cat='i';
        synb.push_back(newSS);
    }
}
void addCon(int id,int data){
    for(int i=0;i<cons.size();i++){
        if(cons[i].data==data){
            return;
        }
    }
    synbl newS;
    consl newC;
    newS.name=id;
    newS.cat='c';
    newC.data=data;
    cons.push_back(newC);
    newS.addr=cons.size()-1;
    synb.push_back(newS);
}
void alGeq(string op){
    int id1,id2;
    if(alNums.empty()){
        cout<<"error: invalid expression"<<endl;
        return;
    }
    id1=alNums.top();
    alNums.pop();
    if(alNums.empty()){
        cout<<"error: invalid expression"<<endl;
        return;
    }
    id2=alNums.top();
    alNums.pop();
    if(op=="="){
        elems.push_back(elem(op,id1,-1,id2));
    }else{
        size_tmp++;
        elems.push_back(elem(op,id1,id2,size_tmp));
        alNums.push(size_tmp);
    }
}
void alPush(int id){
    alNums.push(id);
}
void exIf(){//需要加跳转地址
    if(alNums.empty()){
        cout<<"error: expected primary-expression before ')' token"<<endl;
        return;
    }
    int id1;
    id1=alNums.top();
    alNums.pop();
    elems.push_back(elem("if",id1,-1,-1));
}
void exIe(){
    elems.push_back(elem("ie",-1,-1,-1));
}
void exEl(){
    elems.push_back(elem("el",-1,-1,-1));
}
void exWh(){
    elems.push_back(elem("wh",-1,-1,-1));
}
void exDo(){//需要加跳转地址
    if(alNums.empty()){
        cout<<"error: expected primary-expression before ')' token"<<endl;
        return;
    }
    int id1;
    id1=alNums.top();
    alNums.pop();
    elems.push_back(elem("do",id1,-1,-1));
}
void exWe(){
    elems.push_back(elem("we",-1,-1,-1));
}
void call(int id){
    if(!table_pfinf.count(id)){
        cout<<"error: '"<<"' was not declared in this scope"<<endl;
        return;
    }
    calls.push(id);
    callParams.push(0);
    elems.push_back(elem("call",id,-1,-1));
}
void callEnd(){
    if(calls.empty()||callParams.empty()){
        cout<<"internal error in callEnd"<<endl;
        return;
    }
    if(callParams.top()<pfinf[synb[table_pfinf[calls.top()]].addr].param.size()){
        cout<<"error: too few arguments to function '"<<"'"<<endl;
        return;
    }else
    if(callParams.top()>pfinf[synb[table_pfinf[calls.top()]].addr].param.size()){
        cout<<"error: too many arguments to function '"<<"'"<<endl;
        return;
    }
    calls.pop();
    callParams.pop();
}
void callParam(){
    if(alNums.empty()){
        cout<<"error: expected primary-expression before ')' token"<<endl;
        return;
    }
    int id1;
    id1=alNums.top();
    alNums.pop();
    int num;
    num=callParams.top();
    callParams.pop();
    callParams.push(++num);
    elems.push_back(elem("params",id1,-1,-1));
}
void secondStep(){
    for(int i=0;i<synb.size();i++){
        table_synb.insert(make_pair(synb[i].name,i));
    }
    table_synb[-1]=-1;
    /*
    四元式最终处理和输出，最好直接输出内存地址
    */
}
int main(){
    secondStep();
    return 0;
}