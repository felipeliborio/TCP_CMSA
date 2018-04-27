#include "CPLEX_TCP.h"

FInstance CPLEX_TCP(FInstance Instance)
{
	try {
		IloEnv Environment;
		IloModel Model(Environment);
		IloCplex Cplex(Model);

		std::deque<std::deque<int>> GraphM = Instance.GetAdjacencyMatrix();


		IloArray<IloBoolVarArray> Belongs (Environment, GraphM.size());
		for (int i = 0; i < GraphM.size(); i++) {
			Belongs[i] = IloBoolVarArray(Environment, GraphM.size());
		}

		IloExpr WeightSum(Environment);

		for (int i = 0; i < GraphM.size(); i++) {
			for (int j = 0; j < GraphM.size(); j++){
				WeightSum += GraphM[i][j] * GraphM[i][j];
			}
		}

		IloObjective Objective = IloMinimize(Environment, WeightSum);
		Model.add(Objective);

		/*
		Exemplos


		IloNumVarArray Array(env, numVar, limInf, limSup, typeVar);
		IloNumVarArray Array(Environment, 10, 0, 1, ILOBOOL);

		IloNumArray NumArray(env, numCoef, c1, c2, ..., c6);
		IloNumArray NumArray(Environment, 3, 10, 11, 12);
		
		IloScalProd(VetorDeVariaveis, VetorDeCoeficientes);

		*/


		/*
		Exemplo 1
		IloNumVar X11(Environment), X12(Environment), X13(Environment),
			X21(Environment), X22(Environment), X23(Environment);

		Model.add(IloMinimize(Environment, 4 * X11 + 2 * X12 + 5 * X13 + 
		11 * X21 + 7 * X22 + 4 * X23));

		
		Model.add(X11 + X12 + X13 <= 800);
		Model.add(X21 + X22 + X23 <= 1000);
		Model.add(X11 + X21		  == 500);
		Model.add(X12 + X22		  == 400);
		Model.add(X13 + X23		  == 900);

		Cplex.extract(Model);
		Cplex.solve();

		std::cout << "Objective Function: " << Cplex.getObjValue() << std::endl;

		std::cout << "X11: " << Cplex.getValue(X11) << std::endl;
		std::cout << "X12: " << Cplex.getValue(X12) << std::endl;
		std::cout << "X13: " << Cplex.getValue(X13) << std::endl;

		std::cout << "X21: " << Cplex.getValue(X21) << std::endl;
		std::cout << "X22: " << Cplex.getValue(X22) << std::endl;
		std::cout << "X23: " << Cplex.getValue(X23) << std::endl;
		*/



		Cplex.end();
		Model.end();
		Environment.end();

		return Instance;
	}
	catch (IloException& Exception) {
		std::cout << "IloException: " << Exception << std::endl;
		return FInstance();
	}
}

