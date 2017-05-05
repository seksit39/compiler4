#include <iostream>
//#include <sstream>
#include <string>

using namespace std;

string xassign(string op1,int offset){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rax"<<endl;	
		asmCode <<"\tmov %rax,-"<<offset<<"(%rbp)"<<endl;
	}else
		asmCode <<"\tmov -"<<op1<<"(%rbp),-"<<offset<<"(%rbp)"<<endl;
	return asmCode.str();
}

string xconstant(int val){
	stringstream asmCode;
	asmCode << "\tmov $"<<val<<", %rax"<<endl;
	asmCode << "\tpush %rax"<<endl;
	return asmCode.str();
}

string xadd(string op1,string op2,string dst){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rbx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op1<<"(%rbp), %rbx"<<endl;
	}
	if(op2==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op2<<"(%rbp), %rax"<<endl;
	}

	asmCode <<"\tadd %rbx, %rax"<<endl;
	asmCode <<"\tpush %rax"<<endl;
	return asmCode.str();
}

string xsub(string op1,string op2,string dst){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rbx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op1<<"(%rbp), %rbx"<<endl;
	}
	if(op2==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op2<<"(%rbp), %rax"<<endl;
	}

	asmCode <<"\tsub %rbx, %rax"<<endl;
	asmCode <<"\tpush %rax"<<endl;
	return asmCode.str();
}

string xmul(string op1,string op2,string dst){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rbx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op1<<"(%rbp), %rbx"<<endl;
	}
	if(op2==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op2<<"(%rbp), %rax"<<endl;
	}

	asmCode <<"\tmul %rbx"<<endl;
	asmCode <<"\tpush %rax"<<endl;
	return asmCode.str();
}
string xdiv(string op1,string op2,string dst){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rbx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op1<<"(%rbp), %rbx"<<endl;
	}
	if(op2==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op2<<"(%rbp), %rax"<<endl;
	}

        asmCode <<"\txor %rdx,%rdx"<<endl;
	asmCode <<"\tidiv %rbx"<<endl;
	asmCode <<"\tpush %rax"<<endl;
	return asmCode.str();
}

string xmod(string op1,string op2,string dst){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rbx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op1<<"(%rbp), %rbx"<<endl;
	}
	if(op2==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op2<<"(%rbp), %rax"<<endl;
	}

        asmCode <<"\txor %rdx,%rdx"<<endl;
	asmCode <<"\tidiv %rbx"<<endl;
	// rdx is reminder of div
	asmCode <<"\tpush %rdx"<<endl;
	return asmCode.str();
}
string xcondition(string op1,string op2,int lCount){
	stringstream asmCode;
	if(op1==""){
		asmCode <<"\tpop %rbx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op1<<"(%rbp),%rbx"<<endl;
	}
	if(op2==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op2<<"(%rbp),%rax"<<endl;
	}
	asmCode <<"\tcmp %rax,%rbx"<<endl;
	asmCode <<"\tjnz LI"<<lCount<<endl;
	return asmCode.str();	
}

string xif(int *lCount){
	//gen label
	stringstream asmCode;
	asmCode <<"LI"<<*lCount<<":"<<endl;
	*lCount=*lCount+1;
	return asmCode.str();
}
string xloopStart(string op,int lCount){

	stringstream asmCode;
	if(op==""){
		asmCode <<"\tpop %rcx"<<endl;
	}else{
		asmCode <<"\tmov -"<<op<<"(%rbp), %rcx"<<endl;
	}
	asmCode <<"\txor %rax,%rax"<<endl;
	asmCode <<"\tcmp %rax,%rcx"<<endl;
	asmCode <<"\tje EL"<<lCount<<endl;
	asmCode <<"L"<<lCount<<":"<<endl;
	return asmCode.str();
}

string xloop(int *lCount){
	stringstream asmCode;

	asmCode <<"\tdec %rcx"<<endl;
	asmCode <<"\tjnz L"<<*lCount<<endl;
	asmCode <<"EL"<<*lCount<<":"<<endl;
	
	*lCount=*lCount+1;
	return asmCode.str();
}

string xprint(string op,bool hex){
	stringstream asmCode;
	asmCode <<"\tpush %rax"<<endl;
	asmCode <<"\tpush %rbx"<<endl;
	asmCode <<"\tpush %rcx"<<endl;
	if(hex){
		asmCode <<"\tmov $showx, %rdi"<<endl;
	}else{
		asmCode <<"\tmov $show, %rdi"<<endl;
	}

	if(op==""){
		asmCode <<"\tpop %rax"<<endl;
	}else{
		asmCode <<"\tmov -"<<op<<"(%rbp), %rax"<<endl;
	}
	asmCode <<"\tmov %rax,%rsi"<<endl;
	asmCode <<"\txor %rax,%rax"<<endl;
	asmCode <<"\tcall printf"<<endl;
	asmCode <<"\tpop %rcx"<<endl;
	asmCode <<"\tpop %rbx"<<endl;
	asmCode <<"\tpop %rax"<<endl;
	return asmCode.str();
}

string genHead(){
	stringstream asmC;
	asmC <<"\t.global main"<<endl;
	asmC <<"\t.text"<<endl;

	asmC <<"main:"<<endl;
	asmC <<"\tmov %rsp,%rbp"<<endl;
	asmC <<"\tsub $208,%rsp"<<endl; // Allocate 26*4 slot for $a-$z
	return 	asmC.str();
}

string genTail(){
	stringstream asmC;
	asmC <<"\tadd $208, %rsp"<<endl;
	asmC <<"\tret"<<endl;
	asmC <<"show:"<<endl;
	asmC <<"\t.asciz \"%d\\n\""<<endl;
	asmC <<"showx:"<<endl;
	asmC <<"\t.asciz \"0x%x\\n\""<<endl;
	return asmC.str();
}

string init_var(string op){
	stringstream asmC;
	asmC <<"\txor %rax,%rax"<<endl;
	asmC << "\tmov %rax,-"<<op<<"(%rbp)"<<endl;
	return asmC.str();
}
