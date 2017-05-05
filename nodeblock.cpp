#include<string> 
#include<iostream>
#include<sstream>
using namespace std;


class NodeBlock
{
	public:
		char type; //tell about type of nodeBlock
		NodeBlock *left; 
		NodeBlock *right;
		//int value;
		NodeBlock() {}
		~NodeBlock() {}
		virtual void setValue(int) {};
		virtual int  getValue() {};
		virtual void print() { cout << "NodeBlock" << endl; };
		virtual string getAsm() {};
		void cgen(); // generate code assembly
};


class Variable : public NodeBlock 
{
	//In X86 have frame pointer name ebp
	private:
		char var_name;
		int address;
	public:
		Variable(int var_name) {
			address = (var_name+1) * 8; //each frame has 8 bytes
			this->type = 'v';
		}
		~Variable() {}
		int getValue() {
			return address;
		}

		virtual void print(){
			cout << "variable = " << endl;
		}
		virtual string getAsm(){
	//		stringstream val;
	//		val <<"-"<<address<<"(%rsp)";
	//		return val.str();
			stringstream val;
			val <<address;
			return val.str();
		}
};

class Constant : public NodeBlock  
{
	private: 
		int value;
	public:
		Constant() {};
		virtual void setValue(int value){
			this->value = value;
			this->type = 'c';
		}

		virtual int getValue(){
			return value;
		}

		virtual void print(){
			cout << "Constant = " << value << endl;
		}
		virtual string getAsm(){
			//stringstream val;
			//val <<"$"<<value;
			//return val.str();
			return "";
		}
};

class IfStatement : public NodeBlock
{
	public:
		IfStatement(NodeBlock *condition) {
			this->left = condition;
			//this->right = then;
			this->type = 'i';
		}
		~IfStatement() {}
		virtual void print() {
			cout << "condition is ";  
			this->left->print();
			//cout << " then ";
			//this->right->print();
		}
};

class AddSyntax : public NodeBlock 
{

	public:
		AddSyntax(NodeBlock *left,NodeBlock *right)
		{
			this->left = left;
			this->right = right;
			this->type = 'a';
		}
		~AddSyntax(){}
      virtual int getValue(){
         return 0;
      }
		virtual void print()
		{
			cout << "PLUS :: left = ";
			this->left->print();
			cout << " right = ";
			this->right->print();
			cout<< endl;
		} 
		virtual string getAsm(){
			return "";
		}
};

class Equal :public NodeBlock
{
	public:
		Equal(NodeBlock *left,NodeBlock *right)
		{
			this->left = left;
			this->right = right;
			this->type = 'e';
		}
		~Equal(){}
		virtual void print()
		{
			cout << " Equal :: " << this->left->getValue() << " == " << this->right->getValue() << endl;
		}
};

class MinusSyntax : public NodeBlock 
{
	public:
		MinusSyntax(NodeBlock *left,NodeBlock *right){
			this->left = left;
			this->right = right;
			this->type = 'm';
		}
		~MinusSyntax(){}
		virtual void print(){
			cout << "MINUS :  left = " << this->left->getValue();
			cout << " right = " << this->right->getValue() << endl;
		}
		virtual string getAsm(){
			return "";
		}
};

class TimesSyntax : public NodeBlock
{
	public:
		TimesSyntax(NodeBlock *left,NodeBlock *right){
			this->left = left;
			this->right = right;
		}
		~TimesSyntax(){}
		virtual void print(){
			cout << " MUL :: left = " << this->left->getValue() << endl;
			cout << " right = " << this->right->getValue() << endl;
		}
		virtual string getAsm(){
			return "";
		}
};

class DivideSyntax : public NodeBlock
{
	public:
		DivideSyntax(NodeBlock *left,NodeBlock *right){
			this->left = left;
			this->right = right;
		}
		~DivideSyntax(){}
		virtual void print(){
			cout << "DIV :: left = " << this->left->getValue() << endl;
			cout << " right = " << this->right->getValue() << endl;
		}
		virtual string getAsm(){
			return "";
		}
};

class ModSyntax : public NodeBlock
{
	public:
		ModSyntax(NodeBlock *left,NodeBlock *right){
			this->left = left;
			this->right = right;
		}
		~ModSyntax(){}
		virtual void print(){
			cout << "MOD :: left = " << this->left->getValue() << endl;
			cout << " right = " << this->right->getValue() << endl;
		}
		virtual string getAsm(){
			return "";
		}
};

class LoopStatement : public NodeBlock
{
      public:
          LoopStatement(NodeBlock *condition) {
          this->left = condition;
          //this->right = statement;
          this->type = 'l';
      }
      ~LoopStatement() {}
      virtual void print() {
         cout << "condition is ";
         this->left->print();
         //cout << " statement ";
         //this->right->print();
      }

};

class Show : public NodeBlock
{
      private:
            int variable;
      public:
         Show(int variable){
            this->variable = variable;
	 }
	 ~Show(){}
	 virtual void print(){
	   cout << "SHOW : " << variable << endl;
	 }
};

class ShowX : public NodeBlock 
{
       private:
          int variable;
       public:
          ShowX(int variable){
             this->variable = variable;
       }
       ~ShowX(){}
       virtual void print(){
            cout << "SHOWX : " << variable << endl;
       }
};
