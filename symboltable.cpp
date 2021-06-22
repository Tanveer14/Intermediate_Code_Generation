
#include<iostream>
#include<stdio.h>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>


using namespace std;


class SymbolInfo{
string name;
string type;
string declaredType;//return type for functions
vector<SymbolInfo*> funcParameters;
bool isArray;
bool isFunction;
bool isDefined;
SymbolInfo * next;


public :

    SymbolInfo()
    {
        this->next=nullptr;
        this->name="";
        this->type="";
        isArray=false;
        isFunction=false;
        isDefined=false;
        declaredType="UNDECLARED";
    }

    SymbolInfo(string name,string type)
    {
        this->next=nullptr;
        this->name=name;
        this->type=type;
        isArray=false;
        isFunction=false;
        isDefined=false;
        declaredType="UNDECLARED";
    }
    void setName(string n)///apparently not necessary
    {
        name=n;
    }

    string getName()
    {
        return name;
    }

    void setType(string t)///apparently not necessary
    {
        type=t;
    }

    string getType()
    {
        return type;
    }

    void setDeclaredType(string declaredType)
    {
        this->declaredType=declaredType;
    }

    string getDeclaredType()
    {
        return declaredType;
    }


    void setArray()
    {
    isArray=true;

    }

    bool getIsArray()
    {
    return isArray;
    }

    void setFunction(bool func=true)
    {
        isFunction=func;
    }

    bool getIsFunction()
    {
        return isFunction;
    }

    void setIsDefined()
    {
        isDefined=true;
    }

    bool getIsDefined()
    {
        return isDefined;
    }

    void insertParameter(SymbolInfo * s)
    {
     /*  SymbolInfo * s1 = new SymbolInfo(s->getName(),s->getType());
       s1->setDeclaredType(s->getDeclaredType()); */
        funcParameters.push_back(s);
    }

    vector<SymbolInfo*> getParameters()
    {

        return funcParameters;
    }




    void makeCopy(SymbolInfo * s)
    {

        this->declaredType=s->declaredType;
        this->isArray=s->isArray;
        this->isFunction=s->isFunction;
        this->name=s->name;
        this->type=s->type;
        this->isDefined=s->isDefined;
    }

    void setNext(SymbolInfo * p)
    {
        next=p;
    }
    SymbolInfo * getNext()
    {
        return next;
    }

    ~SymbolInfo()
    {

    }



};

class ScopeTable{

SymbolInfo ** arr;
ScopeTable * parentScope;
int total_buckets;
int current_child_id;
string id;

public:
    ScopeTable(int b_size,ScopeTable * parent)///if there is at least one scope existing
    {
         total_buckets=b_size;


        arr= new SymbolInfo*[total_buckets];

        for(int i=0;i<total_buckets;i++)
        {
            arr[i]=nullptr;
        }

        parentScope=parent;
        id=parent->id+"."+to_string(parent->increase_childID());
        current_child_id =0;

    }


    ScopeTable(int b_size,int id)///if it is the first scope
    {
        total_buckets=b_size;

        arr= new SymbolInfo*[total_buckets];
        parentScope=nullptr;
        this->id=to_string(id);
         for(int i=0;i<total_buckets;i++)
        {
            arr[i]=nullptr;
        }

    }

    int increase_childID()
    {
        current_child_id++;
        return current_child_id;
    }

    void setParent(ScopeTable * p)
    {
        parentScope=p;
    }

    ScopeTable * getParent()
    {
        return parentScope;
    }

    string getID()
    {
        return id;
    }

    int hash_func(string s)
    {
        int hash_val=0;
        for(int i=0;i<s.length();i++)
        {
            hash_val+=s[i];
        }

        return hash_val%total_buckets;
    }

    SymbolInfo * lookUp(string symbolName)
    {

        int h=hash_func(symbolName);///the hash_value of symbol
        SymbolInfo * current=arr[h];
        int sl=0;
        while(current != nullptr)
        {

            if(current->getName()==symbolName)
            {


                return current;
            }

            current=current->getNext();
            sl++;

        }

        return nullptr;
    }

    bool Insert(SymbolInfo * symbol)
    {


         int h=hash_func(symbol->getName());///the hash_value of symbol
        SymbolInfo * current=arr[h];
        SymbolInfo * prev=nullptr;
        int position=0;
        while(current != nullptr)
        {

            if(current->getName()==symbol->getName())
            {

                return false;
            }
            prev = current;
            current=current->getNext();
            position++;

        }

        if(prev == nullptr)
        {
            arr[h]=symbol;
        }
        else{
            prev->setNext(symbol);
        }


        return true;
    }

    bool deleteSymbol(string symbolName)
    {


        int h= hash_func(symbolName);
        SymbolInfo * current = arr[h];
        SymbolInfo * prev = nullptr;
          int sl=0;

        while(current != nullptr)
        {
            if(current->getName() == symbolName)
            {



                if(prev == nullptr)
                {
                    arr[h]=current->getNext();



                }
                else
                {

                prev->setNext(current->getNext());


                }
                delete current;

                 return true;
            }

            prev=current;
            current=current->getNext();
            sl++;
        }


        return false;


    }


    void printScopeTable(ofstream &outfile)
    {


        outfile<<"ScopeTable # "<<id<<endl;
        SymbolInfo * current;
        for(int i=0;i<total_buckets;i++)
        {
            current = arr[i];
            if(arr[i]!=nullptr)
            {

                            outfile<<" "<<i<<" -->";
            }
            while(current != nullptr)
            {

                outfile<<" < "<<current->getName()<<" , "<<current->getType()<<" >";
                 current=current->getNext();
            }
        if(arr[i]!=nullptr)
        {

                outfile<<endl;
        }

        }

       outfile<<endl;


    }


    ~ScopeTable()
    {

        SymbolInfo * current;
        SymbolInfo * next;
        for(int i=0;i<total_buckets;i++)
        {
            current = arr[i];

            while(current != nullptr)
            {

                next=current->getNext();
                delete current;
                current=next;
            }

        }

        delete arr;
    }
};

class SymbolTable{

ScopeTable * currentScope;
int global_scope_counter;
int total_buckets;

public:
SymbolTable(int b_size )
{
    total_buckets=b_size;
     global_scope_counter=1;
    currentScope=new ScopeTable(total_buckets,global_scope_counter);


}

void EnterScope(ofstream &outfile)
{
    if(currentScope == nullptr )
    {
        global_scope_counter++;
        currentScope=new ScopeTable(total_buckets,global_scope_counter);
    }
    else
    {
        currentScope=new ScopeTable(total_buckets,currentScope);
    // outfile<<"New ScopeTable with id "<<currentScope->getID()<<" created "<<endl<<endl;

    }


}

void ExitScope(ofstream &outfile)
{
    if(currentScope != nullptr)
    {
    string tableID=currentScope->getID();

    ScopeTable * parent=currentScope->getParent();


    delete currentScope;
    currentScope=parent;

 // outfile<<"ScopeTable with id "<<tableID<<" removed "<<endl<<endl;
    }
}

bool Insert(SymbolInfo * symbol)
{
    bool success = currentScope->Insert(symbol);

    return success;
}

bool RemoveSymbol(string symbolName)
{
    return currentScope->deleteSymbol(symbolName);
}

SymbolInfo * lookUp(string symbolName)
{
    ScopeTable * temp=currentScope;
    while(temp != nullptr)
    {
        SymbolInfo * result=temp->lookUp(symbolName);
        if(result != nullptr)
        {

            return result;
        }
        temp=temp->getParent();
    }



    return nullptr;
}

SymbolInfo * lookUpCurrentScope(string symbolName)
{
    return currentScope->lookUp(symbolName);
}

void printCurrentScope(ofstream &outfile)
{
    currentScope->printScopeTable(outfile);
}

void printAllScope(ofstream &outfile)
{
    ScopeTable * temp=currentScope;
    while(temp != nullptr)
    {
        temp->printScopeTable(outfile);
        temp=temp->getParent();
    }
}


~SymbolTable()
{
    ScopeTable * temp;
    while(currentScope != nullptr)
    {
        temp = currentScope;
        currentScope=currentScope->getParent();
        delete temp;

    }
}


};
/*
int main()
{
 vector<SymbolInfo*>  list_of_declared_vars;
 list_of_declared_vars.push_back(new SymbolInfo("a","b"));
 for(auto it : list_of_declared_vars)
 {
    cout<<1;
 }

 {int a=10;};
 cout<<a;
}
*/
