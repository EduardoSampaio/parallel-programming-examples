#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <iostream>
#include <iomanip>
#include "mpi/mpi.h"

using namespace std;

void print_matrix(double *matrix, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << setprecision(3) << matrix[i * N + j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void init_matrix(double *matrix, int N)
{
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i * N + j] = (float(rand()) / float(RAND_MAX)) * (100 - -100) + -100;
        }
    }
}
int main(int argc, char *argv[])
{
   	int N = atoi(argv[1]);
    double t_start, t_end, t_total;
    int rank, size;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_rows = N / size;

    double *matrix = new double[N * N];
    init_matrix(matrix, N);

    double *sub_matrix = new double[N * num_rows];
    MPI_Scatter(matrix, N * num_rows, MPI_DOUBLE, sub_matrix, N * num_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double *row = new double[N * num_rows];

    if (rank == 0)
    {
        t_start = MPI_Wtime();
    }


    for (int i = 0; i < (rank * num_rows); i++)
    {
        // Wait for the preceeding ranks to forward us a row
        MPI_Bcast(row, N, MPI_DOUBLE, i / num_rows, MPI_COMM_WORLD);

        // Eliminate from this element from all rows mapped to this
        // rank
        for (int j = 0; j < num_rows; j++)
        {
            for (int k = i + 1; k < N; k++)
            {
                sub_matrix[j * N + k] -= sub_matrix[j * N + i] * row[k];
            }
            sub_matrix[j * N + i] = 0;
        }
    }

    // Senders go here
    int pivot_column;
    for (int i = 0; i < num_rows; i++)
    {
        // Normalize this row to the pivot
        pivot_column = rank * num_rows + i;

        // Normalize every other element in this row to the pivot
        for (int j = pivot_column + 1; j < N; j++)
        {
            sub_matrix[i * N + j] /= sub_matrix[i * N + pivot_column];
        }

        // Normalize trivial case
        sub_matrix[i * N + pivot_column] = 1;

        // Fill row to be sent
        memcpy(row, &sub_matrix[i * N], N * sizeof(double));

        // Broadcast the normalized row to everyone else;
        MPI_Bcast(row, N, MPI_DOUBLE, rank, MPI_COMM_WORLD);

        // Update the rest of the rows for this rank
        for (int j = i + 1; j < num_rows; j++)
        {
            // Subtract from all the elements in a lower row
            for (int k = pivot_column + 1; k < N; k++)
            {
                sub_matrix[j * N + k] -= sub_matrix[i * N + i] * row[k];
            }

            // Eliminate the trivial case
            sub_matrix[j * N + pivot_column] = 0;
        }
    }

    // Finished ranks must still wait with synchronous broadcast
    for (int i = rank * num_rows + 1; i < N; i++)
    {
        MPI_Bcast(row, N, MPI_DOUBLE, i / num_rows, MPI_COMM_WORLD);
    }

    // Barrier to track when calculations are done
    MPI_Barrier(MPI_COMM_WORLD);

    // Stop the time before the gather phase
    if (rank == 0)
    {
        t_end = MPI_Wtime();
        t_total = t_end - t_start;
    }

    /*
     * Collect all Sub-Matrices
     * All sub-matrices are gathered using the gather function
     */
    MPI_Gather(sub_matrix, N * num_rows, MPI_DOUBLE, matrix, N * num_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    // Check the result, and print the time
    if (rank == 0)
    {
        //print_matrix(matrix, N);
        printf("\nTempo de execucao Tempo Total MPI: %.4f segundos\n", t_total);
    }

    // Free heap-allocated memory
    if (rank == 0)
    {
        delete[] matrix;
    }
    delete[] sub_matrix;
    delete[] row;

    return 0;
}
