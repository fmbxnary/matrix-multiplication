#include <stdio.h>
#include <pthread.h>

#define MATRIX_SIZE 10

/* global variables shared by all threads */
int m1[MATRIX_SIZE][MATRIX_SIZE], m2[MATRIX_SIZE][MATRIX_SIZE], OutputMatrix[MATRIX_SIZE][MATRIX_SIZE];
int m1_row, m2_row, m2_column;

FILE *fp;

void *fileRead1();      /* the thread for Matrix1Reader */
void *fileRead2();      /* the thread for Matrix2Reader */
void *matrixMultiply(); /* the matrix multiplier thread */

int main(int argc, char *argv[])
{
    pthread_t Matrix1Reader, Matrix2Reader, MatrixMultiplier; /* thread identifier */
    pthread_attr_t attr;                                      /* set of thread attributes */

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the first file reader thread */
    pthread_create(&Matrix1Reader, &attr, fileRead1, argv[1]);

    /* create the second file reader thread */
    pthread_create(&Matrix2Reader, &attr, fileRead2, argv[2]);

    /* wait for the first file reader thread to exit */
    pthread_join(Matrix1Reader, NULL);

    /* wait for the second file reader thread to exit */
    pthread_join(Matrix2Reader, NULL);

    /* create the matrix multiplier thread */
    pthread_create(&MatrixMultiplier, &attr, matrixMultiply, argv[3]);

    /* wait for the matrix multiplier thread to exit */
    pthread_join(MatrixMultiplier, NULL);

    fp = fopen(argv[3], "w");                  // open the output file
    fprintf(fp, "%d %d\n", m1_row, m2_column); // write lengths of matrix

    /* write resultant matrix values to file */
    for (int i = 0; i < m1_row; i++)
    {
        for (int j = 0; j < m2_column; j++)
        {
            fprintf(fp, "%d ", OutputMatrix[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

/* read inputs from argv[1] */
void *fileRead1(void *arg)
{
    FILE *fp;
    int m1_column;

    /* initialize */
    fp = fopen(arg, "r"); // open file where file is located at argv[1]
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open matrix file!\n");
    }

    /* main loop */
    fscanf(fp, "%d", &m1_row);    // read the row of first matrix
    fscanf(fp, "%d", &m1_column); // read the column of first matrix
    /* read all matrix values */
    for (int i = 0; i < m1_row; i++)
    {
        for (int j = 0; j < m1_column; j++)
        {
            fscanf(fp, "%d", &m1[i][j]);
        }
    }
    fclose(fp);

    pthread_exit(0);
}

/* read inputs from argv[2] */
void *fileRead2(void *arg)
{
    FILE *fp;
    /* initialize */
    fp = fopen(arg, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open matrix file!\n");
    }

    /* main loop */
    fscanf(fp, "%d", &m2_row);    // read the row of second matrix
    fscanf(fp, "%d", &m2_column); // read the column of second matrix

    /* read all matrix values */
    for (int i = 0; i < m2_row; i++)
    {
        for (int j = 0; j < m2_column; j++)
        {
            fscanf(fp, "%d", &m2[i][j]);
        }
    }
    fclose(fp);

    pthread_exit(0);
}

/* multiply two matrices */
void *matrixMultiply(void *arg)
{
    int sum = 0;

    // multiplication of two matrices
    for (int i = 0; i < m1_row; i++)
    {
        for (int j = 0; j < m2_column; j++)
        {
            for (int k = 0; k < m2_row; k++)
            {
                sum += m1[i][k] * m2[k][j];
            }
            OutputMatrix[i][j] = sum;
            sum = 0;
        }
    }
}