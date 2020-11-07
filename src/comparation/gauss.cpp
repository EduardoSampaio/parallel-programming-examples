#include <iostream>
#include <time.h>

using namespace std;

void print(double **matriz, int n) {
    for (int i = 0; i < n; i++, printf("\n"))
        for (int j = 0; j <= n; j++)
            printf("%lf ", matriz[i][j]);

    printf("\n");
}

void swap_row(double **matriz, int i, int j,int n) {
    for (int k = 0; k <= n; k++) {
        double temp = matriz[i][k];
        matriz[i][k] = matriz[j][k];
        matriz[j][k] = temp;
    }
}

void init_matrix(double **matriz, int n) {

    for (int i = 0; i <n; i++) {
        matriz[i] = (double *) new double[n + 1];
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = (double(rand()) / double(RAND_MAX)) * (100 - -100) + -100;
        }
    }
}

void back_substitution(double **matriz, int n) {

    double x[n];
    for (int i = n - 1; i >= 0; i--) {
        x[i] = matriz[i][n];
        for (int j = i + 1; j < n; j++) {
            x[i] -= matriz[i][j] * x[j];
        }
        x[i] = x[i] / matriz[i][i];
    }
}

void forward_Elimination(double **matriz, int N) {
    for (int k = 0; k < N; k++) {

        int i_max = k;
        double v_max = matriz[i_max][k];

        // Encontra maior pivô
        for (int i = k + 1; i < N; i++) {
            if (abs(matriz[i][k]) > v_max) {
                v_max = matriz[i][k], i_max = i;
            }
        }
        // Relizar troca de linha
        if (i_max != k)
            swap_row(matriz, k, i_max, N);

        // Escalonamento
        for (int i = k + 1; i < N; i++) {
            double f = matriz[i][k] / matriz[k][k];
            for (int j = k + 1; j <= N; j++)
                matriz[i][j] -= matriz[k][j] * f;
            matriz[i][k] = 0;
        }
    }
}


int main(int argc, char **argv)
{
	int N = atoi(argv[1]);
	float b, x[N];
	double** matriz = (double**)new double[N];


	init_matrix(matriz,N);

	clock_t tempo_total_inicio = clock();
	forward_Elimination(matriz,N);
	clock_t tempo_total_fim = clock();
	double tempo_total = (double)(tempo_total_fim - tempo_total_inicio) / CLOCKS_PER_SEC;
	printf("\nTempo de execucao Tempo Total: %.4f segundos\n", tempo_total);

	delete[] matriz;

	return 0;
}
