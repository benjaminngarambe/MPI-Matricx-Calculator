//Benjamin Ngarambe
// Link to Github Repository : https://github.com/benjaminngarambe/MPI-Matricx-Calculator
// Matrices multiplication
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>

#define DIMENSION 4

MPI_Status status;

double matrix_a[DIMENSION][DIMENSION],matrix_b[DIMENSION][DIMENSION],matrix_c[DIMENSION][DIMENSION];

int main(int argc, char **argv)
{
  int actions, ID, totalHoldActions, startPoint, terminalPoint, rows, cancel;

  struct timeval start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &ID);
  MPI_Comm_size(MPI_COMM_WORLD, &actions);

  totalHoldActions = actions - 1;

 if (ID == 0) {
    srand( time(NULL) );
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++) {
        matrix_a[i][j]= rand()%10;
        matrix_b[i][j]= rand()%10;
      }
    }
	
  printf("\n\t\tMatrix Multiplication - MPI - 4x4\n");
    
    printf("\n 1st Matrix\n\n");
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
	    printf("\n");
    }

    printf("\n 2nd Matrix\n\n");
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++) {
        printf("%.0f\t", matrix_b[i][j]);
      }
	    printf("\n");
    }

    rows = DIMENSION/totalHoldActions;
    cancel = 0;

    for (terminalPoint=1; terminalPoint <= totalHoldActions; terminalPoint++)
    {
      MPI_Send(&cancel, 1, MPI_INT, terminalPoint, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, terminalPoint, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_a[cancel][0], rows*DIMENSION, MPI_DOUBLE,terminalPoint,1, MPI_COMM_WORLD);
      MPI_Send(&matrix_b, DIMENSION*DIMENSION, MPI_DOUBLE, terminalPoint, 1, MPI_COMM_WORLD);
      cancel = cancel + rows;
    }

    for (int i = 1; i <= totalHoldActions; i++)
    {
      startPoint = i;
      MPI_Recv(&cancel, 1, MPI_INT, startPoint, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, startPoint, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_c[cancel][0], rows*DIMENSION, MPI_DOUBLE, startPoint, 2, MPI_COMM_WORLD, &status);
    }

    printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
    for (int i = 0; i<DIMENSION; i++) {
      for (int j = 0; j<DIMENSION; j++)
        printf("%.0f\t", matrix_c[i][j]);
      printf ("\n");
    }
    printf ("\n");
  }

  if (ID > 0) {

    startPoint = 0;
    MPI_Recv(&cancel, 1, MPI_INT, startPoint, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, startPoint, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_a, rows*DIMENSION, MPI_DOUBLE, startPoint, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_b, DIMENSION*DIMENSION, MPI_DOUBLE, startPoint, 1, MPI_COMM_WORLD, &status);

    for (int k = 0; k<DIMENSION; k++) {
      for (int i = 0; i<rows; i++) {
        matrix_c[i][k] = 0.0;
        for (int j = 0; j<DIMENSION; j++)
          matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
      }
    }

    MPI_Send(&cancel, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&matrix_c, rows*DIMENSION, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
