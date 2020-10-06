#include <iostream>
#include <fstream>
#include "mpi/mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int size, rank, count;
	string filename = argv[1];
	MPI_Status status;

	std::ofstream ofs;
	ofs.open(filename + ".txt", ofstream::out | ofstream::app);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank != 0)
	{
		for (int i = 1; i <= 100; i++)
		{
			int buffer_sent = 1;
			MPI_Send(&buffer_sent, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
	}
	else
	{
		int buffer_received;
		for (int i = 1; i <= (size - 1) * 100; i++)
		{
			MPI_Recv(&buffer_received, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
			ofs << buffer_received << endl;
		}
	}

	MPI_Finalize();

	ofs.close();
	return 0;
}

/*
Escreva um programa que teste quão justa é a implementação da passagem de
mensagens. Para isso, todos os processos, exceto o processo zero, deve enviar 100
mensagens (quaisquer) para o processo zero. O processo zero deve escrever as mensagens
recebidas num arquivo de saída. O nome do arquivo de saída deve ser um parâmetro na
linha de comando. Use na recepção de mensagens as variáveis pré-definidas
MPI_ANY_SOURCE e MPI_ANY_TAG. A implementação MPI é justa?
*/