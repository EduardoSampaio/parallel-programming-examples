#include <iostream>
#include <time.h>
#define N 1000

using namespace std;

int main()
{
	float b, x[N];
	float** matriz = (float**)new float[N];

	clock_t tempo_total_inicio = clock();
	clock_t criando_matriz_inicio = clock();

	// Gerar Matriz com números aleatórios
	for (int i = 0; i < N; i++)
	{
		matriz[i] = (float*)new float[N];
	}

	for (int i = 1; i < N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			matriz[i][j] = rand() % 10;
		}
	}

	clock_t criando_matriz_fim = clock();
	double criando_tempo = (double)(criando_matriz_fim - criando_matriz_inicio) / CLOCKS_PER_SEC;
	printf("\nTempo de execucao criação da matriz: %.4f segundos\n", criando_tempo);

	clock_t escalonamento_inicio = clock();

	// Eliminação de gauss criando matriz tringular superior
	for (int j = 0; j < N; j++)
	{
		for (int i = 1; i < N; i++)
		{
			if (i != j)
			{
				b = matriz[i][j] / matriz[j][j];
				for (int k = 0; k <= N + 1; k++)
				{
					matriz[i][k] = matriz[i][k] - b * matriz[j][k];
				}
			}
		}
	}

	// Resolvendo o sistema linear
	for (int i = 0; i < N; i++)
	{
		x[i] = matriz[i][N + 1] / matriz[i][i];
	}

	clock_t escalonamento_fim = clock();
	double escalonando = (double)(escalonamento_fim - escalonamento_inicio) / CLOCKS_PER_SEC;
	printf("\nTempo de execucao escalonamento:  %.4f segundos\n", escalonando);


	clock_t tempo_total_fim = clock();
	double tempo_total = (double)(tempo_total_fim - tempo_total_inicio) / CLOCKS_PER_SEC;
	printf("\nTempo de execucao Tempo Total: %.4f segundos\n", tempo_total);

	return 0;
}
