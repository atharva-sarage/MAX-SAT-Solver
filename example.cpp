#include <iostream>

#include <minisat/simp/SimpSolver.h>

using namespace Minisat;
using namespace std;

//The function initializes the
//Solver with the following CNF formula.
//	 1  2 0
//	-1 -2 0
inline void init_solver(Solver &ms_solver) {

	ms_solver.newVar();
	ms_solver.newVar();

	vec < Lit > clause_1;
	clause_1.push(mkLit(0, false));
	clause_1.push(mkLit(1, false));
	ms_solver.addClause(clause_1);

	vec < Lit > clause_2;
	clause_2.push(mkLit(0, true));
	clause_2.push(mkLit(1, true));
	ms_solver.addClause(clause_2);
}

int main() {
	Solver MS_Solver;

	init_solver(MS_Solver);

	vec < Lit > assumps;
	assumps.push(mkLit(0, true));		//Assume 1 is falsified.

	if (MS_Solver.solve()) {
		cout << "SAT" << endl;
		lbool l_t((uint8_t) 0);
		for (vec<Lit>::Size i = 0; i < 2; i++) {
			if (MS_Solver.model[i] == l_t) {
				cout << i + 1 << ' ';
			}
			else
				cout << -(i + 1) << ' ';
		}
	}
	else
		cout << "UNSAT";
	cout << endl;
	return 0;
}
