#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

void init_matrix(double **matriz, int n)
{
	for (int i = 0; i < n; i++)
	{
		matriz[i] = (double *)new double[n + 1];
	}

	srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			matriz[i][j] = (double(rand()) / double(RAND_MAX)) * (100 - -100) + -100;
		}
	}
}

void print(double **matriz, int n)
{
	for (int i = 0; i < n; i++, printf("\n"))
		for (int j = 0; j < n; j++)
			printf("%lf ", matriz[i][j]);

	printf("\n");
}

int main(int argc, char **argv)
{
	int N = atoi(argv[1]);
	double x[N];
	double **matriz = (double **)new double[N];

	init_matrix(matriz, N);
	double tempo_total_inicio = omp_get_wtime();

#pragma omp parallel
	{
		for (int k = 0; k < N; k++)
		{
			#pragma omp for
			for (int i = k + 1; i < N; i++)
			{
				double f = matriz[i][k] / matriz[k][k];
				for (int j = k + 1; j <= N; j++)
					matriz[i][j] -= matriz[k][j] * f;
				matriz[i][k] = 0;
			}
		}

		for (int i = N - 1; i >= 0; i--)
		{
			x[i] = matriz[i][N];
			for (int j = i + 1; j < N; j++)
			{
				x[i] -= matriz[i][j] * x[j];
			}
			x[i] = x[i] / matriz[i][i];
		}
	}

	double tempo_total_fim = omp_get_wtime();
	double tempo_total = (double)(tempo_total_fim - tempo_total_inicio);
	printf("\nTempo de execucao Tempo Total OMP: %.4f segundos\n", tempo_total);

	// print(matriz, N);

	delete[] matriz;

	return 0;
}
