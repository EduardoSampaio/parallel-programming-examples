#include <iostream>
#include <iomanip>
#include <cstring>
#include <assert.h>
#include <time.h>

using namespace std;

void init_matrix(double *matrix, int N)
{
    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Random number between 100 and -100
            matrix[i * N + j] = (float(rand()) / float(RAND_MAX)) * (100 - -100) + -100;
        }
    }
}

void print_matrix(double *matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << setprecision(3) << matrix[i * N + j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);
    double *matrix = new double[N * N];
    init_matrix(matrix, N);

    clock_t tempo_total_inicio = clock();

    double pivot;
    for (int i = 0; i < N - 1; i++)
    {
        // Pivot will be the diagonal
        pivot = matrix[i * N + i];

        // Iterate of the remaining row elements
        for (int j = i + 1; j < N; j++)
        {
            // Divide by the pivot
            matrix[i * N + j] /= pivot;
        }

        // Do direct assignment for trivial case (self-divide)
        matrix[i * N + i] = 1.0;

        // Eliminate ith element from the jth row
        double scale;
        for (int j = i + 1; j < N; j++)
        {
            // Factor we will use to scale subtraction by
            scale = matrix[j * N + i];

            // Iterate over the remaining columns
            for (int k = i + 1; k < N; k++)
            {
                matrix[j * N + k] -= matrix[i * N + k] * scale;
            }

            // Do direct assignment for trivial case (eliminate position)
            matrix[j * N + i] = 0;
        }
    }

    // Trivial case of last row with only one element
    matrix[(N - 1) * N + N - 1] = 1;

    clock_t tempo_total_fim = clock();
    double tempo_total = (double)(tempo_total_fim - tempo_total_inicio) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao Tempo Total: %.4f segundos\n", tempo_total);

    //print_matrix(matrix,N);
    return 0;
}
