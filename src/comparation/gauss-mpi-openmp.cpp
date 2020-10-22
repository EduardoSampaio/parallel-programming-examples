#include <iostream>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "mpi/mpi.h"
#include <omp.h>

using namespace std;

void init_matrix(double **matriz, int n)
{

	for (int i = 0; i < n; i++)
	{
		matriz[i] = (double *)new double[n + 1];
	}

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

void forward_Elimination(double **matriz, int begin, int end)
{
#pragma omp parallel
	{
		#pragma omp for
		for (int k = begin; k < end; k++)
		{

			int i_max = k;
			double v_max = matriz[i_max][k];

			// Encontra maior pivô
			for (int i = k + 1; i < end; i++)
			{
				if (abs(matriz[i][k]) > v_max)
				{
					v_max = matriz[i][k], i_max = i;
				}
			}

			// Relizar troca de linha
			if (i_max != k)
				swap_row(matriz, k, i_max, end);

			// Escalonamento
			for (int i = k + 1; i < end; i++)
			{
				double f = matriz[i][k] / matriz[k][k];
				for (int j = k + 1; j <= end; j++)
					matriz[i][j] -= matriz[k][j] * f;
				matriz[i][k] = 0;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int N = atoi(argv[1]);
	int rank, size, TAG = 1;
	double t_start, t_end, t_total;

	double **matriz = (double **)new double[N];
	init_matrix(matriz, N);

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		t_start = MPI_Wtime();

		int begin = (N / size) * rank;
		int end = (N / size) * (rank + 1);

		for (int i = 1; i < size; i++)
		{
			MPI_Send(matriz, N, MPI_DOUBLE, i, TAG, MPI_COMM_WORLD);
		}

		forward_Elimination(matriz, 0, N);
	}
	else
	{
		MPI_Recv(matriz, N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

		int begin = (N / size) * rank;
		int end = (N / size) * (rank - 1);
		double **output = matriz;
		forward_Elimination(output, begin, end);
	}

	if (rank == 0)
	{
		//print(matriz,N - 1);
		t_end = MPI_Wtime();
		t_total = t_end - t_start;
		printf("\nTempo de execucao Tempo Total MPI: %.4f segundos\n", t_total);
	}

	MPI_Finalize();

	delete matriz;
	return 0;
}
