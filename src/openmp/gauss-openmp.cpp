#include <iostream>
#include <time.h>
#include <omp.h>

using namespace std;

int main(int argc, char **argv)
{
	int N = atoi(argv[1]);
	float x[N];
	float **matriz = (float **)new float[N];
	
	omp_set_num_threads(4);

	double tempo_total_inicio = omp_get_wtime();
	double criando_matriz_inicio = omp_get_wtime();

	for (int i = 0; i < N; i++)
	{
		matriz[i] = (float *)new float[N];
	}

	for (int i = 1; i < N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			matriz[i][j] = rand() % 10;
		}
	}

	double criando_matriz_fim = omp_get_wtime();
	printf("\nTempo de execucao criação da matriz: %.4f segundos\n", criando_matriz_fim - criando_matriz_inicio);

	double escalonamento_inicio = omp_get_wtime();;

	for (int j = 0; j < N; j++)
	{
		for (int i = 1; i < N; i++)
		{
			if (i != j)
			{
				int b = matriz[i][j] / matriz[j][j];
				for (int k = 0; k <= N + 1; k++)
				{
					matriz[i][k] = matriz[i][k] - b * matriz[j][k];
				}
			}
		}
	}

	for (int i = 0; i < N; i++)
	{
		x[i] = matriz[i][N + 1] / matriz[i][i];
	}

	double escalonamento_fim = omp_get_wtime();
	printf("\nTempo de execucao escalonamento:  %.4f segundos\n", escalonamento_fim - escalonamento_inicio);
	double tempo_total_fim = omp_get_wtime();
	printf("\nTempo de execucao Tempo Total: %.4f segundos\n", tempo_total_fim - tempo_total_inicio);

	return 0;
}
