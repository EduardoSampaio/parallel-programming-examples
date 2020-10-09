#include <iostream>
#include <fstream>
#include "mpi/mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int size, rank, source, dest, tag = 1;
	int inmsg = 0, outmsg = 0, numero = atoi(argv[1]);
	MPI_Status status;

	std::ofstream ofs;
	ofs.open("resultado.txt", std::ofstream::out | std::ofstream::app);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		outmsg = numero + rank;
		MPI_Send(&outmsg, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
		for (int i = 1; i < size; i++)
		{
			MPI_Recv(&inmsg, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
		}
	}
	else
	{
		MPI_Recv(&inmsg, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
		outmsg = inmsg + rank;

		if (rank != size - 1)
		{
			MPI_Send(&outmsg, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
		}
		MPI_Send(&outmsg, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	ofs << "Identificacao = " << rank << " Valor " << outmsg << endl;

	MPI_Finalize();

	ofs.close();

	return 0;
}

/*
1 Escreva um programa que transmita um dado do processo zero para todos os outros
processos, da seguinte maneira: o processo i deve receber o dado do processo i-1, somar
seu número de identificação e enviar o dado ao processo i+1, até que o último processo seja
atingido.

Assuma que o dado consiste de um número inteiro. O processo zero deve ler o dado
especificado pelo usuário na linha de comando. Todos os processos devem enviar o valor
calculado para o processo zero. Assim, o processo zero deve escrever no arquivo de saída o
número de identificação do processo e o valor por ele calculado. O nome do arquivo de
saída também deve ser um parâmetro na linha de comando. Por exemplo, suponha que o
valor passado inicialmente é igual a 12, o arquivo de saída seria:
Identificação = 0 valor 12
Identificação = 1 valor 13
Identificação = 2 valor 15
Identificação = 3 valor 18
e assim por diante.
*/

/*
Compilando

mpic++ exercicio_1.cpp -o exercicio_1
mpiexec -np 4 ./exercicio_1 12

*/