#include <iostream>
#include <time.h>

using namespace std;

void print(double **matriz, int n)
{
    for (int i = 0; i < n; i++, printf("\n"))
        for (int j = 0; j <= n; j++)
            printf("%lf ", matriz[i][j]);

    printf("\n");
}

void init_matrix(double **matriz, int n)
{

    for (int i = 0; i < n; i++)
    {
        matriz[i] = (double *)new double[n + 1];
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matriz[i][j] = (float(rand()) / float(RAND_MAX)) * (100 - -100) + -100;
        }
    }
}

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    double x[N];
    double **matriz = (double **)new double[N];

    init_matrix(matriz, N);

    clock_t tempo_total_inicio = clock();

    // Eliminação para frente
    for (int k = 0; k < N; k++)
    {
        for (int i = k + 1; i < N; i++)
        {
            double f = matriz[i][k] / matriz[k][k];
            for (int j = k + 1; j <= N; j++)
                matriz[i][j] -= matriz[k][j] * f;
            matriz[i][k] = 0;
        }
    }

    // Substituição Para atrás
    for (int i = N - 1; i >= 0; i--)
    {
        x[i] = matriz[i][N];
        for (int j = i + 1; j < N; j++)
        {
            x[i] -= matriz[i][j] * x[j];
        }
        x[i] = x[i] / matriz[i][i];
    }

    clock_t tempo_total_fim = clock();
    double tempo_total = (double)(tempo_total_fim - tempo_total_inicio) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao Tempo Total: %.4f segundos\n", tempo_total);

    //print(matriz, N - 1);
    delete[] matriz;

    return 0;
}
