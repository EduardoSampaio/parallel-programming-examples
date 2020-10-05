#include <iostream>
#include <fstream>
#include <mpi.h>

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
			ofs << buffer_received <<endl;	
		}		
	}

	
	MPI_Finalize();

	ofs.close();
	return 0;
}