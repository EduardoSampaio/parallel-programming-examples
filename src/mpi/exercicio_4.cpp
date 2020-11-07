#include <iostream>
#include <fstream>
#include "mpi/mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	int size, rank, source, dest, tag = 1;
	int inmsg = 0, outmsg = 0, numero = atoi(argv[1]);
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{

	}
	else
	{

	}

	MPI_Finalize();


	return 0;
}
