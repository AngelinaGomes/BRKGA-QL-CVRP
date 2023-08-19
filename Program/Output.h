/************************************************************************************
									IO Functions
*************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "Data.h"

void WriteSolutionScreen(TSol s, int n, float timeBest, float timeTotal, char instance[])
{
	printf("\n\n\n Instance: %s \nsol: ", instance);
	for (int i=0; i<n; i++)
		printf("%d ", s.vec[i].sol);

    printf("\nDecoder: %.2lf",s.vec[n].rk);
	printf("\nfo: %.5lf",s.fo);
	printf("\nTotal time: %.3f",timeTotal);
	printf("\nBest time: %.3f\n\n",timeBest);

    //printf("\nH1: %d (%d) H2: %d (%d) H3: %d (%d) H4: %d (%d)\n\n", H1, SH1, H2, SH2, H3, SH3, H4, SH4);
}

void WriteSolution(TSol s, int n, float timeBest, float timeTotal, char instance[])
{
	FILE *arquivo;
    arquivo = fopen("./Results/Solutions.txt","a");

	if (!arquivo)
	{
		printf("\n\nFile not found Solutions.txt!!!");
		getchar();
		exit(1);
	}

    fprintf(arquivo,"\n\nInstance: %s", instance);
	fprintf(arquivo,"\nSol: ");
	for (int i=0; i<n; i++)
		fprintf(arquivo,"%d ", s.vec[i].sol);
	fprintf(arquivo,"\nFO: %.5lf", s.fo);
  	fprintf(arquivo,"\nBest time: %.3f",timeBest);
	fprintf(arquivo,"\nTotal time:%.3f \n",timeTotal);

	fclose(arquivo);
}

void WriteResults(double fo, double foAverage, std::vector <double> fos, float timeBest, float timeTotal, char instance[])
{
	FILE *arquivo;
    arquivo = fopen("./Results/Results.csv","a");

	if (!arquivo)
	{
		printf("\n\nFile not found Results.xls!!!");
		getchar();
		exit(1);
	}

	fprintf(arquivo,"\n%s", instance);
    fprintf(arquivo,"\t%d", (int)fos.size());
    for (int i=0; i<fos.size(); i++)
        fprintf(arquivo,"\t%lf", fos[i]);
	fprintf(arquivo,"\t%lf", fo);
	fprintf(arquivo,"\t%lf", foAverage);
	fprintf(arquivo,"\t%.3f", timeBest);
	fprintf(arquivo,"\t%.3f", timeTotal);

	fclose(arquivo);
}

void WriteRoutes(vector<Vehicle> v, char instance[], double bestFo)
{
	FILE *arqr;
	arqr = fopen("./Results/Routes.txt","a");
	if (!arqr) {
		printf("\n\nFile not found Solutions.txt!!!");
		getchar();
		exit(1);
	}

	double foV = 0;
	vector<int> customers(n+1,0);

	fprintf(arqr, "Instance: %s", instance);
	fprintf(arqr, "\n Num vehicles %ld:", v.size());
	for(int i = 0; i < v.size(); i++){
		int loadV = 0;
		fprintf(arqr, "\n Vehicle %d:", i);
		fprintf(arqr, "\n  size: %ld\n  route: ", v[i].route.size());
		for(int j = 0; j < v[i].route.size(); j++) {
			fprintf(arqr, "%d ", v[i].route[j].point);
			foV += bestVehicles[i].route[j].dist; // fo
			customers[bestVehicles[i].route[j].point]++; // check customers
			loadV += vertex[bestVehicles[i].route[j].point].size; // factibilidade
		}
		// factibilidade
		if (loadV <= capacity)
			fprintf(arqr, "\n  capacity OK");
		else
			fprintf(arqr, "\n  capicity error - value: %d", loadV);
	}
	// fo
	if (abs(foV - bestFo) < 1e-9) // foV == bestFo
		fprintf(arqr, "\n fo: %f", foV);
	else
		fprintf(arqr, "\n fo diferente: foV = %f & bestFo = %f", foV, bestFo);
	//check depot
	if (customers[0] != v.size())
		fprintf(arqr, "\n Repeating depot on some route. Extra depots: %d", customers[0] - (int)v.size());
	// check customers
	for (int i = 1; i < n+1; i++) {
		if (customers[i] != 1)
			fprintf(arqr, "\n Customer %d served %d times.", i, customers[i]);
	}
	fprintf(arqr,"\n\n\n");
	fclose(arqr);
}
