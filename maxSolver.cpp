/******************************************
*    AUTHOR:         Atharva Sarage       *
*    INSTITUITION:   IIT HYDERABAD        *
******************************************/
#include<bits/stdc++.h>
#include <minisat/simp/SimpSolver.h>
using namespace Minisat;
using namespace std;
int totalClauses,totalVariables;
int variableCounter;
map <int,vector<int>*> tree;
void init(int i,int a,int b,int lo[],int hi[])
{
	lo[i]=a;
	hi[i]=b;
    cout<<i<<" "<<lo[i]<<" "<<hi[i]<<endl;
	if(a==b)
		return;
	int m=(a+b)/2;
	init(2*i,a,m,lo,hi);
	init(2*i+1,m+1,b,lo,hi);
}
int merge(int nodeNo,int leftCount,int rightCount,Solver &ms_solver){
    cout<<"merge on "<<nodeNo<<endl;
    vector <int>* newVector = new vector<int>; 
    for(int i=0;i<=leftCount+rightCount;i++){  
        ms_solver.newVar();
        newVector->push_back(variableCounter++);
    }
    tree[nodeNo]=newVector;
    // clause addition
    for(int i=1;i<=leftCount+rightCount;i++){
        for(int j=0;j<=leftCount;j++){
            if(i-j>=0 && (i-j)<=rightCount){
                if(j==0){
                    cout<<(*tree[2*nodeNo+1])[i-j]<<" -> "<<(*tree[nodeNo])[i]<<" "<<i<<endl;
                    vec <Lit> clause;
                    clause.push(mkLit((*tree[2*nodeNo+1])[i-j],false));
                    clause.push(mkLit((*tree[nodeNo])[i],true));
                    ms_solver.addClause(clause);
                }
                else if((i-j)==0){
                    cout<<(*tree[2*nodeNo])[j]<<" "<<" -> "<<(*tree[nodeNo])[i]<<" "<<i<<endl;
                    vec <Lit> clause;
                    clause.push(mkLit((*tree[2*nodeNo])[j],false));
                    clause.push(mkLit((*tree[nodeNo])[i],true));
                    ms_solver.addClause(clause);
                }
                else{
                    cout<<(*tree[2*nodeNo])[j]<<" "<<(*tree[2*nodeNo+1])[i-j]<<" -> "<<(*tree[nodeNo])[i]<<" "<<i<<endl;
                    vec <Lit> clause;
                    clause.push(mkLit((*tree[2*nodeNo])[j],false));
                    clause.push(mkLit((*tree[2*nodeNo+1])[i-j],false));
                    clause.push(mkLit((*tree[nodeNo])[i],true));
                    ms_solver.addClause(clause);
                }
            }
        }
    }
    return rightCount+leftCount;
}
int build(int i,int lo[],int hi[],Solver &ms_solver){    
    cout<<i<<" "<<lo[i]<<" "<<hi[i]<<endl;
    int l=lo[i];
	int r=hi[i];
	if(l==r){
        vector <int>* newVector = new vector<int>; 
        ms_solver.newVar();
        ms_solver.newVar();
        newVector->push_back(variableCounter++);
        newVector->push_back(variableCounter++);
        vec <Lit> clause;
        clause.push(mkLit(totalVariables+lo[i],false));
        clause.push(mkLit(variableCounter-1,true));
        ms_solver.addClause(clause);
        cout<<"-"<<totalVariables+lo[i]<<" "<<variableCounter-1<<endl;
        tree[i]=newVector;
        return 1; 
	}
    int leftCount = build(2*i,lo,hi,ms_solver);
    int rightCount = build(2*i+1,lo,hi,ms_solver);
    return merge(i,leftCount,rightCount,ms_solver);
}

inline void initSolver(Solver &ms_solver){
    for(int i=0;i<=totalVariables;i++){
        ms_solver.newVar();
    }
}
int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0); // fast IO
    srand(time(NULL)); // seed
    string strOneLine,str;
    /* INPUT HANDLING */
    int inp;
    char ch; 
    cin>>ch;
    // ignore lines starting with 'c'
    while (ch=='c'){
        getline(cin,strOneLine); 
        cin>>ch;
    }
    cin>>str>>totalVariables>>totalClauses;
    cout<<totalVariables<<" "<<totalClauses<<endl;
    vector<int>input; // stores literals
    Solver MS_Solver;
	initSolver(MS_Solver);	
    variableCounter = totalVariables +1;
    while(cin>>inp){
        vec < Lit > clause;
        if(inp==0){
            for(auto literal:input){
                cout<<literal<<" ";
                if(literal > 0)
                    clause.push(mkLit(literal,true));     
                else
                    clause.push(mkLit(literal*-1,false));
            }
            cout<<variableCounter<<"?"<<endl;
            MS_Solver.newVar();
            clause.push(mkLit(variableCounter++,true));
            MS_Solver.addClause(clause);
            input.clear();
        }else{
            input.emplace_back(inp); // add literal
        }    
    }
    /* INPUT HANDLING DONE */
    /* TOTALIZER ENCODING */
    cout<<"totalizer encoding start"<<endl;
    int lo[12*totalClauses+5],hi[12*totalClauses+5];
    init(1,1,totalClauses,lo,hi);
    cout<<"init done"<<endl;
    build(1,lo,hi,MS_Solver);
    cout<<"build done"<<endl;
    vec < Lit > assumps;
    cout<<(*tree[1])[1]<<"???"<<endl;
	assumps.push(mkLit((*tree[1])[1], false));
    if(MS_Solver.solve(assumps)){
    lbool l_t((uint8_t) 1);
        cout<<"SAT\n";
        for (vec<Lit>::Size i = 1; i <=totalVariables+totalClauses; i++) {
            if (MS_Solver.model[i] == l_t) {
                cout << i << ' ';
            }
            else
                cout << -(i) << ' ';
        }
    }else {
        cout<<"UNSAT"<<endl;
    }

}
