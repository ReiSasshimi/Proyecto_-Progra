#ifndef cuerpos_h
#define cuerpos_h

#include<stdio.h>
#include <math.h>
#include <string.h>

#define N_Max 10
#define DIAS 365
#define G 6.674330e-11 //constante de gravitacion universal

typedef struct{
 char nombre[20];
 double masa;
 double x, y; //Posicion
 double vx, vy; //Velocidad    
}Cuerpo;

void inicializar(Cuerpo cuerpo[], int N);
void CalFuerza(Cuerpo cuerpo[], double Fx[N_Max][N_Max], double Fy[N_Max][N_Max], int N);
void actualizar(Cuerpo cuerpo[], double Fx[N_Max][N_Max], double Fy[N_Max][N_Max], double dt, int N);
double ETotal(Cuerpo cuerpo[], int N);
void Gfila(double hist[DIAS+1][2*N_Max], Cuerpo cuerpo[], int dia, int N);
void GCSV(double hist[DIAS+1][2*N_Max], int DiasTotales, int N);
void MostrarEstado(Cuerpo cuerpo[], int dia, int N);

void graficarConGnuplot(int n);

#endif