#include "cuerpos.h"

void mostrarMenu() {
    printf("\n=========================================\n");
    printf("   SIMULADOR GRAVITACIONAL DE N-CUERPOS  \n");
    printf("=========================================\n");
    printf("1. Configurar parametros\n");
    printf("2. Salir\n");
    printf("Seleccione una opcion: ");
}

int main() {
    int opcion;
    double dt = 86400.0 / PDias; // 1 hora por defecto

    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida.\n");
            break;
        }

    switch (opcion) {
    case 1: {
        int n;
        printf("\nCuantos cuerpos deseas simular? (Max %d): ", N_Max);
        if (scanf("%d", &n) != 1) {
            printf("Entrada no valida.\n");
            break;
        }

        if (n > 0 && n <= N_Max) {
            // Declaración de estructuras y matrices con el límite máximo
            Cuerpo cuerpo[N_Max];
            double Fx[N_Max][N_Max];
            double Fy[N_Max][N_Max];
            static double hist[DIAS + 1][2 * N_Max];

            inicializar(cuerpo, n);//datos por el usuario

            printf("\nIniciando simulacion por %d dias (dt = %.0f s)...\n", DIAS, dt);

            for (int dia = 0; dia <= DIAS; dia++) {
                Gfila(hist, cuerpo, dia, n);

                if (dia < DIAS) {
                    for (int i = 0; i < PDias; i++){
                        CalFuerza(cuerpo, Fx, Fy, n);
                        actualizar(cuerpo, Fx, Fy, dt, n);
                    }  
                }
            }

            GCSV(hist, DIAS, n); //exportar a CSV
            graficarConGnuplot(n);
            printf("\n¡Simulacion finalizada! Datos guardados en el archivo CSV.\n");

        } else {
            printf("Error: El numero de cuerpos debe estar entre 1 y %d.\n", N_Max);
        }
        break;
    }
    case 2:
        printf("\nSaliendo del simulador...\n");
        break;
    default:
        printf("\nOpcion no valida. Intente de nuevo.\n");
    }   
    } while (opcion != 2);

    return 0;
}