#include "cuerpos.h"

void inicializar(Cuerpo cuerpo[],int n){

printf("\n==================================\n");
printf("\n     Datos de los %d Cuerpos      \n",n);
printf("\n==================================\n");

for (int i = 0; i < n; i++){
    printf("\n=== Cuerpo %d ===", i+1);

    printf("\nIngrese el nombre del cuerpo %d: ", i+1);
    scanf("%19s", cuerpo[i].nombre);

    printf("\n Ingrese la masa de %s (En notacion cientifica 1e30):", cuerpo[i].nombre);
    scanf("%lf", & cuerpo[i].masa);

    printf("\n Ingrese la posicion de %s (x y):", cuerpo[i].nombre);
    scanf("%lf  %lf", & cuerpo[i].x, & cuerpo[i].y);

    printf("\n Ingrese la velocidad de %s (vx vy):", cuerpo[i].nombre);
    scanf("%lf %lf", &cuerpo[i].vx, &cuerpo[i].vy);
    }
}

void CalFuerza(Cuerpo cuerpo[], double Fx[N_Max][N_Max], double Fy[N_Max][N_Max], int n){

    for (int i = 0; i < n; i++){
        for( int j = 0; j < n; j++){
            if (i == j){ // fuerza de un cuerpo en si mismo
            Fx[i][j] = 0.0;
            Fy[i][j] = 0.0;}
            else{//fuerza para los otros cuerpos

                //calcular distancia
                double dx = cuerpo[j].x - cuerpo[i].x;
                double dy = cuerpo[j].y - cuerpo[i].y;

                double dist = sqrt(pow(dx,2) + pow(dy,2));

                if (dist > 0.00){

                    double d2 = pow(dist,2) + pow(S,2);
                    double F = (G * cuerpo[i].masa * cuerpo[j].masa)/ pow(dist,2)/d2; //modulo de la fuerza
                    Fx[i][j] = F * (dx / dist);
                    Fy[i][j] = F * (dy / dist);}

                else{
                    Fx[i][j] = 0.00;
                    Fy[i][j] = 0.00;}
            }
        }
    }
}

void actualizar(Cuerpo cuerpo[], double Fx[N_Max][N_Max], double Fy[N_Max][N_Max], double dt, int n){

    for (int i = 0; i < n; i++){
        double FxTotal = 0.0, FyTotal = 0.0;
        for (int j = 0; j < n; j++){
            FxTotal += Fx[i][j];
            FyTotal += Fy[i][j];
        }
        cuerpo[i].vx += (FxTotal / cuerpo[i].masa) * dt;
        cuerpo[i].vy += (FyTotal / cuerpo[i].masa) * dt;
    }

    for (int i = 0; i < n; i++){
        cuerpo[i].x += cuerpo[i].vx * dt;
        cuerpo[i].y += cuerpo[i].vy * dt;
    }
}

double ETotal(Cuerpo cuerpo[], int n){

    double K = 0.00;
    double U = 0.00;

    //Energia cinetica
    for (int i = 0; i < n; i++){
        double v2 = pow(cuerpo[i].vx,2) + pow(cuerpo[i].vy,2);
        K = 0.5 * cuerpo[i].masa * v2;
    }
    //Energia potencial
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            double dx = cuerpo[j].x - cuerpo[i].x;
            double dy = cuerpo[j].y - cuerpo[i].y;
            double dist = sqrt(pow(dx,2) + pow(dy,2));

            if (dist > 0.0){
                U -= (G*cuerpo[i].masa * cuerpo[j].masa)/dist ;
            }
        }
    }

    return K + U;
}

void Gfila(double hist[DIAS+1][2*N_Max], Cuerpo cuerpo[], int dia, int n){
     for (int i = 0; i < n; i++){
        hist[dia][2*i]     = cuerpo[i].x;
        hist[dia][2*i + 1] = cuerpo[i].y;
    }
}

void GCSV(double hist[DIAS+1][2*N_Max], int DiasTotales, int n) {

    FILE *archivo = fopen("trayectorias.csv", "w");

    if (archivo == NULL){
        printf("\nError: no se pudo crear el archivo CSV.\n");
        return;}

    fprintf(archivo, "dia");
    for (int i = 0; i < n; i++)
        fprintf(archivo, ",Cuerpo%d_x,Cuerpo%d_y", i+1, i+1);
    fprintf(archivo, "\n");

    for (int dia = 0; dia <= DiasTotales; dia++){
        fprintf(archivo, "%d", dia);
        for (int k = 0; k < 2*n; k++)
            fprintf(archivo, ",%.6e", hist[dia][k]);
        fprintf(archivo, "\n");
    }
    fclose(archivo);
}

void MostrarEstado(Cuerpo cuerpo[], int dia, int n) {

    printf("\n================ Dia %d ===============\n", dia);
    printf("%-10s %14s %14s %12s\n", "Cuerpo", "x (m)", "y (m)", "v (m/s)");

    for (int i = 0; i < n; i++){

        double v = sqrt(pow(cuerpo[i].vx,2) + pow(cuerpo[i].vy,2));
        printf("%-10s %14.4e %14.4e %12.2f\n", cuerpo[i].nombre, cuerpo[i].x, cuerpo[i].y, v);
    }
}

void graficarConGnuplot(int n) {
    FILE *gp = _popen("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persistent", "w");
    if (gp == NULL) {
        printf("Error: No se pudo abrir Gnuplot.\n");
        return;
    }

    fprintf(gp, "set terminal wxt size 900,900\n");
    fprintf(gp, "set datafile separator ','\n");
    fprintf(gp, "set title 'Simulacion de N-Cuerpos'\n");
    fprintf(gp, "set xlabel 'Eje X (m)'\n");
    fprintf(gp, "set ylabel 'Eje Y (m)'\n");
    fprintf(gp, "set size square\n");
    fprintf(gp, "set key outside right top\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot ");

    for (int i = 0; i < n; i++) {
        int colX = 2 + 2*i;
        int colY = 3 + 2*i;
        fprintf(gp, "'trayectorias.csv' every ::1 using %d:%d with lines title 'Cuerpo %d'",
                colX, colY, i + 1);
        if (i < n - 1) fprintf(gp, ", ");
    }
    fprintf(gp, "\n");

    fflush(gp);
    _pclose(gp);
}