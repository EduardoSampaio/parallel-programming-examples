#include <iostream>
#include <fstream> 
#include <mpi.h>

using namespace std;

int main(int argc, char** argv)
{
	int size, rank, source, dest, N = 0, count, tag = 1;
	int inmsg = 0, outmsg = 0, numero = atoi(argv[1]);
	MPI_Status status;

	std::ofstream ofs;
	ofs.open("resultado.txt", std::ofstream::out | std::ofstream::app);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		outmsg = numero + rank;;
		MPI_Send(&outmsg, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
		for (int i = 1; i < size; i++) {
			MPI_Recv(&inmsg, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
		}		
	}
	else {
		MPI_Recv(&inmsg, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
		outmsg = inmsg + rank;

		if (rank != size - 1) {
			MPI_Send(&outmsg, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
		}
		MPI_Send(&outmsg, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}

	ofs << "Identificacao = " << rank << " Valor " << outmsg << endl;

	MPI_Finalize();

	ofs.close();

	return 0;
}