#include <iostream>
#include <fstream>
#include <time.h>
#include "mpi/mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int size, rank, dest, source, tag = 1;
	int N = atoi(argv[1]), count = atoi(argv[2]);
	double *vetor = (double *)new double[N];
	double start, end;

	if (argc < 2)
	{
		cout << "Quantidade de parametros invalidos";
		exit(0);
	}

	for (int i = 0; i < N; i++)
	{
		vetor[i] = -1.0;
	}

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	start = MPI_Wtime();

	while (count > 0)
	{
		if (rank == 0)
		{
			for (int i = 0; i < N; i++)
			{
				vetor[i] = 2.0;
			}

			MPI_Send(vetor, N, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD);
			MPI_Recv(vetor, N, MPI_DOUBLE, 1, tag, MPI_COMM_WORLD, &status);
		}
		if (rank == 1)
		{
			for (int i = 0; i < N; i++)
			{
				vetor[i] = 4.0;
			}
			MPI_Recv(vetor, N, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			MPI_Send(vetor, N, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
		}
		count--;
	}
	end = MPI_Wtime();
	cout << "Rank: " << rank << " tempo = " << end - start << endl;

	MPI_Finalize();


	return 0;
}

/*
Escreva um programa para medir o tempo gasto para enviar um vetor (tamanhos 100,
1K e 1M) de números de precisão dupla (double) do processo zero para o processo 1
usando MPI_Send e MPI_Recv. Os tamanhos dos vetores devem ser um parâmetro na linha
de comando. Cada vetor deve ser armazenado dinamicamente e iniciado com o valor -1.0.
O processo zero deve localmente trocar este valor para 2.0 e o processo um para o valor
4.0. Defina uma variável para contabilizar a quantidade de vezes que cada vetor deve ser
trocado entre os processos. A quantidade de vezes também deve ser determinada como um
parâmetro na linha de comando.
*/

/*
mpic++ exercicio_3.cpp -o exercicio_3
mpiexec -np 4 ./exercicio_3 tamanho_vetor quantidade
*/