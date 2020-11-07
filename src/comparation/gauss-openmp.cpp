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
		for (int j = 0; j <= n; j++)
			printf("%lf ", matriz[i][j]);

	printf("\n");
}

void swap_row(double **matriz, int i, int j, int n)
{
	for (int k = 0; k <= n; k++)
	{
		double temp = matriz[i][k];
		matriz[i][k] = matriz[j][k];
		matriz[j][k] = temp;
	}
}

void forward_Elimination(double **matriz, int N)
{
#pragma omp parallel
	{
		#pragma omp for
		for (int k = 0; k < N; k++)
		{
			int i_max = k;
			double v_max = matriz[i_max][k];

			// Encontra maior pivô
			for (int i = k + 1; i < N; i++)
			{
				if (abs(matriz[i][k]) > v_max)
				{
					v_max = matriz[i][k], i_max = i;
				}
			}
			// Relizar troca de linha
			if (i_max != k)
				swap_row(matriz, k, i_max, N);
			// Escalonamento
			for (int i = k + 1; i < N; i++)
			{
				double f = matriz[i][k] / matriz[k][k];
				for (int j = k + 1; j <= N; j++)
					matriz[i][j] -= matriz[k][j] * f;
				matriz[i][k] = 0;
			}
		}
	}
}

int main(int argc, char **argv)
{
	int N = atoi(argv[1]);
	float b, x[N];
	double **matriz = (double **)new double[N];

	init_matrix(matriz, N);
	double tempo_total_inicio = omp_get_wtime();
	forward_Elimination(matriz, N);
	double tempo_total_fim = omp_get_wtime();
	double tempo_total = (double)(tempo_total_fim - tempo_total_inicio);
	printf("\nTempo de execucao Tempo Total OMP: %.4f segundos\n", tempo_total);

	delete[] matriz;

	return 0;
}
