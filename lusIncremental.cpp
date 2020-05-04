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
    //cout<<i<<" "<<lo[i]<<" "<<hi[i]<<endl;
	if(a==b)
		return;
	int m=(a+b)/2;
	init(2*i,a,m,lo,hi);
	init(2*i+1,m+1,b,lo,hi);
}
int merge(int nodeNo,int leftCount,int rightCount,Solver &ms_solver,int limit){
    //cout<<"merge on "<<nodeNo<<endl;
    if(limit==1){
        vector <int>* newVector = new vector<int>; 
        ms_solver.newVar();
        ms_solver.newVar();
        newVector->push_back(variableCounter++);
        newVector->push_back(variableCounter++);
        tree[nodeNo]=newVector;
    }else{
        ms_solver.newVar();
        (tree[nodeNo])->push_back(variableCounter++);
    }   
    // clause addition
    // only add for limit
    //cout<<i<<" "<<min(limit,leftCount+rightCount)<<endl;
    int i = limit;
    for(int j=0;j<=min(limit,leftCount);j++){
        if(i-j>=0 && (i-j)<=min(limit,rightCount)){
            //cout<<limit<<" "<<j<<" "<<(i-j)<<endl;
            if(j==0){
                vec <Lit> clause;
                clause.push(mkLit((*tree[2*nodeNo+1])[i-j],false));
                clause.push(mkLit((*tree[nodeNo])[i],true));
              //  cout<<(*tree[2*nodeNo+1])[i-j]<<" -> "<<(*tree[nodeNo])[i]<<" "<<i<<" "<<ms_solver.num_clauses<<endl;
                ms_solver.addClause(clause);
            }
            else if((i-j)==0){
                vec <Lit> clause;
                clause.push(mkLit((*tree[2*nodeNo])[j],false));
                clause.push(mkLit((*tree[nodeNo])[i],true));
                //cout<<(*tree[2*nodeNo])[j]<<" "<<" -> "<<(*tree[nodeNo])[i]<<" "<<i<<" "<<ms_solver.num_clauses<<endl;
                ms_solver.addClause(clause);
            }
            else{
                vec <Lit> clause;
                clause.push(mkLit((*tree[2*nodeNo])[j],false));
                clause.push(mkLit((*tree[2*nodeNo+1])[i-j],false));
                clause.push(mkLit((*tree[nodeNo])[i],true));
                //cout<<(*tree[2*nodeNo])[j]<<" "<<(*tree[2*nodeNo+1])[i-j]<<" -> "<<(*tree[nodeNo])[i]<<" "<<i<<endl;
                ms_solver.addClause(clause);
            }
        }
    }
    
    //cout<<"merge done"<<endl;
    return rightCount+leftCount;
}
int build(int i,int lo[],int hi[],Solver &ms_solver,int limit){    
    //cout<<i<<" "<<lo[i]<<" "<<hi[i]<<endl;
    //cout<<i<<endl;
    int l=lo[i];
	int r=hi[i];
	if(l==r && limit==1){
        vector <int>* newVector = new vector<int>; 
        ms_solver.newVar();
        ms_solver.newVar();
        newVector->push_back(variableCounter++);
        newVector->push_back(variableCounter++);
        vec <Lit> clause;
        clause.push(mkLit(totalVariables+lo[i],false));
        clause.push(mkLit(variableCounter-1,true));
        ms_solver.addClause(clause);
        //cout<<"done "<<endl;
        //cout<<"-"<<totalVariables+lo[i]<<" "<<variableCounter-1<<endl;
        tree[i]=newVector;
        return 1; 
	}
    else if(l==r)
        return 1;
    int leftCount = build(2*i,lo,hi,ms_solver,limit);
    int rightCount = build(2*i+1,lo,hi,ms_solver,limit);
    return merge(i,leftCount,rightCount,ms_solver,limit);
}

inline void initSolver(Solver &ms_solver,vector < vector<int>> basicClauses){
    for(int i=0;i<=totalVariables+totalClauses;i++){
        ms_solver.newVar();
    }
    //cout<<basicClauses.size()<<"?????????"<<endl;
    for(int i=0;i<basicClauses.size();i++){
        vec <Lit> clause;
        for(auto literal:basicClauses[i]){
            //cout<<literal<<" ";
                if(literal > 0)
                    clause.push(mkLit(literal,true));     
                else
                    clause.push(mkLit(literal*-1,false));
        }
        //cout<<endl;
        ms_solver.addClause(clause);
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
    //cout<<totalVariables<<" "<<totalClauses<<endl;
    vector<int>input; // stores literals
    variableCounter = totalVariables +1;
    vector <vector<int>> basicClauses;
    while(cin>>inp){
        vector < int > clause;
        if(inp==0){
            for(auto literal:input)
                clause.push_back(literal);                 
            // cout<<variableCounter<<"?"<<endl;
            clause.push_back(variableCounter++);
            basicClauses.push_back(clause);
            input.clear();
        }else{
            input.emplace_back(inp); // add literal
        }    
    }
    int lo[12*totalClauses+5],hi[12*totalClauses+5];
    init(1,1,totalClauses,lo,hi);
    int ans;
    Solver MS_Solver;
    initSolver(MS_Solver,basicClauses);
    for(int i=0;i<=totalClauses;i++){       
        //cout<<i<<endl;
        build(1,lo,hi,MS_Solver,i+1);    
        //cout<<"build*done"<<endl;
        //cout<<i<<" "<<(*tree[1])[i+1]<<" "<<MS_Solver.num_clauses<<endl;
        vec < Lit > assumps;    
	    assumps.push(mkLit((*tree[1])[i+1], false));
        //cout<<MS_Solver.solve(assumps)<<endl;
        if(MS_Solver.solve(assumps)){
            cout<<"o "<<totalClauses-i<<endl;
            cout<<"s OPTIMUM FOUND"<<endl;
            lbool l_t((uint8_t) 1);
            cout<<"v ";
            // for (vec<Lit>::Size i = 1; i <=totalVariables+totalClauses; i++) {
            //     if (MS_Solver.model[i] == l_t) {
            //         cout << i << ' ';
            //     }
            //     else
            //         cout << -(i) << ' ';
            // }
            break;
        }else{
            // // LSet conflict present
            // for(int i=0;i<MS_Solver.conflict.size;i++)
            //     cout<<MS_Solver.conflict[i].x<<endl;
            
        }

    }
}
