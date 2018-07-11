#include "CPLEX_TCP.h"

FInstance CPLEX_TCP(FInstance Instance)
{
	try {
		IloEnv Environment;
		IloModel Model(Environment);
		IloCplex Cplex(Model);


		//Adding the objective and related variables*********************************
		std::deque<std::deque<int>> w = Instance.GetAdjacencyMatrix();//Weight matrix
		IloArray<IloBoolVarArray> y(Environment, w.size());//Membership matrix
		for (int i = 0; i < w.size(); i++) {
			y[i] = IloBoolVarArray(Environment, w.size());
		}

		IloExpr WeightSum(Environment);

		for (int i = 0; i < w.size(); i++) {
			for (int j = 0; j < w.size(); j++) {
				WeightSum += w[i][j] * y[i][j];
			}
		}

		IloObjective Objective = IloMinimize(Environment, WeightSum);
		Model.add(Objective);
		//***************************************************************************
		
		std::deque<int> Y;//Optional vertices
		Y.resize(w.size(), 1);
		auto Terminals = Instance.GetTerminals();
		for (auto Terminal : Terminals) {
			Y[Terminal] = 0;
		}
		
		//Adding links and routers restrictions**************************************
		IloExpr LinkCount(Environment);//LinkCount is l
		IloExpr RouterCount(Environment);//RouterCount is r
		for (int i = 0; i < Y.size(); i++) {
			if (Y[i]) {//if i is a optional vertex
				IloExpr Degree(Environment);
				for (int j = 0; j < y[i].getSize(); j++) Degree += y[i][j];
				if (Degree.asVariable == 2) {
					LinkCount += 1;
				}
				else if (Degree.asVariable > 2) {
					RouterCount += 1;
				}
			}
		}
		Model.add(LinkCount <= Instance.GetMaxLinks());//MaxLinks is L
		Model.add(RouterCount <= Instance.GetMaxRouters());//MaxRouters is R
		//***************************************************************************




		IloArray<IloBoolVarArray> Belongs(Environment, GraphM.size());
		IloArray<IloArray<IloBoolVarArray> > x(Environment, GraphM.size());

		for (int i = 0; i < GraphM.size(); i++) {
			Belongs[i] = IloBoolVarArray(Environment, GraphM.size());
			Belongs[i] = IloArray<IloBoolVarArray>(Environment, GraphM.size());
			for (int j = 0; j < GraphM.size(); j++) {
				x[i][j] = IloBoolVarArray(Environment, QNT_TER - 1); // O tamanho de S0
			}
		}

		IloExpr WeightSum(Environment);

		for (int i = 0; i < GraphM.size(); i++) {
			for (int j = 0; j < GraphM.size(); j++) {
				WeightSum += GraphM[i][j] * Belongs[i][j];
			}
		}


		for (int k : S0) {
			if (k == s) continue;

			for (int j = 0; j < GraphM.size(); j++) {
				if (j == s or j == k) continue;

				IloExpr SumOut(Environment);
				for (int i = 0; i < GraphM.size(); i++) {
					SumOut += x[i][j][k];
				}
				IloExpr SumIn(Environment);
				for (int i = 0; i < GraphM.size(); i++) {
					SumIn += x[j][i][k];
				}

				Model.add(SumOut == SumIn);
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

