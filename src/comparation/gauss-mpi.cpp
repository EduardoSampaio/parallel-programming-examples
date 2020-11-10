#include <iostream>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "mpi/mpi.h"
#define MASTER 0

using namespace std;

float **alloc_2d(int rows, int cols)
{
	float *data = (float *)malloc(rows * cols * sizeof(int));
	float **array = (float **)malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++)
		array[i] = &(data[cols * i]);

	return array;
}

int main(int argc, char *argv[])
{
	// Obter tamanho da matriz
	int N = atoi(argv[1]);

	// processo atual
	int rank;
	// quantidade de processos
	int size;

	int tag = 1;

	// Matriz
	float **mSend, **mRecv_1;

	//Variaveis de Tempo
	double t_start, t_end, t_total;

	mSend = alloc_2d(N, N);

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			mSend[i][j] = (float(rand()) / float(RAND_MAX)) * (100 - -100) + -100;
		}
	}

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == MASTER)
	{

		t_start = MPI_Wtime();

		for (int dest = 1; dest < size; dest++)
		{
			MPI_Send(&(mSend[0][0]), N * N, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
			MPI_Recv(&(mSend[0][0]), N * N, MPI_FLOAT, dest, 1, MPI_COMM_WORLD, &status);
		}

		int start = (N / size) * rank;
		int end = (N / size) * (rank + 1);

		for (int k = start; k <= end; k++)
		{
			for (int i = k + 1; i < N; i++)
			{
				float f = mSend[i][k] / mSend[k][k];
				for (int j = k + 1; j <= end; j++)
				{
					mSend[i][j] -= mSend[k][j] * f;
				}
				mSend[i][k] = 0;
			}
		}

		double x[N];
		for (int i = N - 1; i >= 0; i--)
		{
			x[i] = mSend[i][N];
			for (int j = i + 1; j < N; j++)
			{
				x[i] -= mSend[i][j] * x[j];
			}
			x[i] = x[i] / mSend[i][i];
		}

		t_end = MPI_Wtime();
		t_total = t_end - t_start;
		printf("\nTempo de execucao Tempo Total MPI: %.4f segundos\n", t_total);
	}
	else
	{
		int start = ((N / size) * rank);
		int end = (N / size) * (rank + 1);

		if (N % size != 0)
		{
			end++;
		}

		mRecv_1 = alloc_2d(N, N);
		MPI_Recv(&(mRecv_1[0][0]), N * N, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);

		for (int k = start; k <= end; k++)
		{
			for (int i = k + 1; i < N; i++)
			{
				float f = mRecv_1[i][k] / mRecv_1[k][k];
				for (int j = k + 1; j <= end; j++)
				{
					mRecv_1[i][j] -= mRecv_1[k][j] * f;
				}
				mRecv_1[i][k] = 0;
			}
		}

		MPI_Send(&(mRecv_1[0][0]), N * N, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	if (rank == MASTER)
	{
		for (int i = 0; i < N; i++, printf("\n"))
			for (int j = 0; j < N; j++)
				printf("%.3lf ", mSend[i][j]);
	}

	return 0;
}
