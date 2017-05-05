%{
#include <cstdio>
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <cstdlib>
#include <fstream>
#include "nodeblock.cpp"
#include "asmgen.cpp"
using namespace std;

void stack_print();
queue<NodeBlock*> queue_node;
int reserveReg[27] = { }; 

int lCount =0;
int ifCount =0;
stack<int> temp;

int swap_temp;
NodeBlock nodeblock; //create nodeblock << need to fixed !!

struct node{
   int data;
   struct node *right, *left;
};

queue<string> asmQ;
queue<string> asmV;
typedef struct node node;
node *subtree;

stack<NodeBlock*> stack_node;

void print_inorder(node *tree)
{
    if (tree)
    {
      printf("%d\n",tree->data);
    }
}

void deltree(node * tree)
{
    if (tree)
    {
      deltree(tree->left);
      deltree(tree->right);
      free(tree);
    }
}

string asmShow(){
	return "mov $show,%edi \nmov %eax,%esi \npush %rax\ncall printf\npop %rax\nret\n";
}

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void convert_to_asm(int opr1,int opr2);
void yyerror(const char *s);
%}

%token CONST
%token OTHERS
%token LEFT RIGHT
%token ENDLN
%token ASSIGN EQ IF ENDIF LOOP END SHOW SHOWX COLON
%token VAR

%left PLUS MINUS 
%left TIMES DIVIDE MOD
%left NEG

%start Input

%%

Input:
     | Line Input;
;

Line:
  ENDLN 
  | Ifstm
  | Loopstm
  | Stms 
  | Display
  | Condition
  | error { yyerror("oops\n"); }
;


Oprn:
  VAR 
  {
  	Variable *node_var = new Variable($1);
  	int char_index = $1;
 	if(reserveReg[char_index] == 0)
 	{
 		asmV.push(init_var(node_var->getAsm()));
 		reserveReg[char_index] = 1;
 	}
 	stack_node.push(node_var);
  }

  | CONST
  {
  	Constant *node_const = new Constant(); //create constant object 
   node_const->setValue($1);  //add value to constant node
   stack_node.push(node_const);
	asmQ.push(xconstant(node_const->getValue()));
  }
;

Condition:
    Oprn EQ Oprn 
  { 
  	NodeBlock *node1 = stack_node.top();
  	stack_node.pop();
  	NodeBlock *node2 = stack_node.top();
  	stack_node.pop();

  	Equal *node_equal = new Equal(node2,node1); //condition object 
  	stack_node.push(node_equal);
	asmQ.push(xcondition(node1->getAsm(),node2->getAsm(),ifCount));
  }
;


Ifstm:
  IF Condition ENDLN Stms ENDIF ENDLN  // change Stms to Stm for first version support only one statement
  {
	asmQ.push(xif(&ifCount));
  }

;

VARF:
	VAR { 
		Variable *node_var = new Variable($1);
	  	int char_index = $1;
	 	if(reserveReg[char_index] == 0)
	 	{	
	 		asmV.push(init_var(node_var->getAsm()));
	 		reserveReg[char_index] = 1;
	 	}
	 	stack_node.push(node_var);
	}

Stm:
  VARF ASSIGN Exp ENDLN{
	NodeBlock *node_exp = stack_node.top();
  	Variable *node_var = new Variable($1);
 	stack_node.push(node_exp);
	asmQ.push(xassign(node_exp->getAsm(),node_var->getValue()));
  }
  | Display {
  }
;

Block:
  Stms {}
  | Ifstm {}
  | Ifstm Stms {}
  | Stms Ifstm {}
  | Stms Ifstm Stms {}
;

Stms:
  Stm {  }
  | Stm Stms {}
;

Exp: 
   CONST {
   Constant *node_const = new Constant(); //create constant object 
   node_const->setValue($1);  //add value to constant node
   stack_node.push(node_const);
	asmQ.push(xconstant(node_const->getValue()));
   } 
  | VAR {
  	// add var to tree it's looklike constant but keep on address form fp(frame pointer)
 	Variable *node_var = new Variable($1);

 	int char_index = $1;

 	if(reserveReg[char_index] == 0)
 	{		
 		asmV.push(init_var(node_var->getAsm()));
 		reserveReg[char_index] = 1;
 	}
 	stack_node.push(node_var);
  }
  | Exp PLUS Exp {
      NodeBlock *node_left;
      NodeBlock *node_right;
      node_right = stack_node.top();
      stack_node.pop();
      node_left = stack_node.top();
      stack_node.pop();
      AddSyntax *addsyn = new AddSyntax(node_left,node_right);
      stack_node.push(addsyn);
 	asmQ.push(xadd(node_right->getAsm(),node_left->getAsm(),"")); 
    }
  | Exp MINUS Exp {
      NodeBlock *node_left;
      NodeBlock *node_right; 
      node_right = stack_node.top();
	  stack_node.pop();
      node_left = stack_node.top();
      stack_node.pop();
      MinusSyntax* minsyn = new MinusSyntax(node_left,node_right);
      stack_node.push(minsyn);
 	asmQ.push(xsub(node_right->getAsm(),node_left->getAsm(),""));      
    }
  | Exp TIMES Exp {
      NodeBlock *node_left;
      NodeBlock *node_right;
      node_right = stack_node.top();
      stack_node.pop();
      node_left = stack_node.top();
      stack_node.pop();
      TimesSyntax* timessyn = new TimesSyntax(node_left,node_right);
      stack_node.push(timessyn);
 	asmQ.push(xmul(node_right->getAsm(),node_left->getAsm(),""));
    }         
  | Exp DIVIDE Exp {
      NodeBlock *node_left;
      NodeBlock *node_right;
      node_right = stack_node.top();
      stack_node.pop();
      node_left = stack_node.top();
      stack_node.pop();
      DivideSyntax* dividesyn = new DivideSyntax(node_left,node_right);
      stack_node.push(dividesyn);
 	asmQ.push(xdiv(node_right->getAsm(),node_left->getAsm(),""));
} 
  | Exp MOD Exp {
      NodeBlock *node_left;
      NodeBlock *node_right;
      node_right = stack_node.top();
      stack_node.pop();
      node_left = stack_node.top();
      stack_node.pop();
      ModSyntax* modsyn = new ModSyntax(node_left,node_right);
      stack_node.push(modsyn);
 	asmQ.push(xmod(node_right->getAsm(),node_left->getAsm(),""));

    }
  | LEFT Exp RIGHT { }
  | MINUS Exp %prec NEG {
      NodeBlock *node;
      node = stack_node.top();
      stack_node.pop();
      int temp_neg = -node->getValue();
	asmQ.push("\tpop %rax");
	asmQ.push("\txor %rbx,%rbx");
	asmQ.push("\tsub %rax,%rbx");
	asmQ.push("\tpush %rbx\n");
      node->setValue(temp_neg);
      stack_node.push(node);
    }
;
LNO:
  VAR 
  {
  	Variable *node_var = new Variable($1);

  	int char_index = $1;

  	if(reserveReg[char_index] == 0)
 	{
 		asmV.push(init_var(node_var->getAsm()));
 		reserveReg[char_index] = 1;
 	}

 	stack_node.push(node_var);
	asmQ.push(xloopStart(node_var->getAsm(),lCount));
  }

  | CONST
  {
  	Constant *node_const = new Constant();
	node_const->setValue($1);  //add value to constant node
	stack_node.push(node_const);
	asmQ.push(xconstant(node_const->getValue()));
	asmQ.push(xloopStart(node_const->getAsm(),lCount));
  }
;
Loopstm:
  LOOP LNO ENDLN Block END ENDLN {
    Variable *node_var = new Variable(-1);
    LoopStatement *node_loop = new LoopStatement(node_var);
	asmQ.push(xloop(&lCount));
  }
;

Display:
  SHOW VAR ENDLN{  
    Variable *node_var = new Variable($2);
    Show *node_show = new Show ($2*4);
    asmQ.push(xprint(node_var->getAsm(),false));
  }
  | SHOWX VAR ENDLN{
    Variable *node_var = new Variable($2);
    ShowX *node_show = new ShowX ($2*4);
    asmQ.push(xprint(node_var->getAsm(),true));
  }
;
%%

void yyerror(const char *s) {
  cout << "ERROR Message: " << s << endl;
  exit(-1);
}


void stack_print()
{
	stack<NodeBlock*> stack_tmp;

	cout << "====== STACK PRINT =======" << endl;

	while(!stack_node.empty())
	{
		stack_node.top()->print();
		NodeBlock* tmp = stack_node.top();
		stack_tmp.push(tmp);
		stack_node.pop();
	}

	while(!stack_tmp.empty()){
		NodeBlock *tmp2 = stack_tmp.top();
		stack_node.push(tmp2);
		stack_tmp.pop();
	}

	cout << "==========================" << endl;

}


int main(int argc, const char *argv[]) {
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
     printf("invalid input file\n");
     exit(1);
  }
  do { 
	yyparse();
  } while(!feof(yyin));
  //while(yyparse());
	ofstream myfile;
	myfile.open ("output.asm");

	myfile<<genHead()<<endl;

	while(!asmV.empty()){
		 myfile<<asmV.front()<<endl;
		asmV.pop();
	}

	while(!asmQ.empty()){
		 myfile<<asmQ.front()<<endl;
		asmQ.pop();
	}
	myfile<<genTail()<<endl;
	myfile.close();

  return 0;
}
