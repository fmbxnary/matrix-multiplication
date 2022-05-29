#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MATRIX_SIZE 10

// Driver code
int main(int argc, char *argv[])
{
    int fd[2], fd2[2], fd3[2]; // declaration of file-descriptors
    int pid1, pid2, pid3;      // declaration of pids

    /* create the pipe */
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(fd3) == -1)
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid1 = fork();

    /* we are in the child1 process. */
    if (pid1 == 0)
    {
        close(fd[0]);

        FILE *fp;

        int m1_row, m1_column;
        int m1[MATRIX_SIZE][MATRIX_SIZE];

        /* initialize */
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Unable to open matrix file!\n");
        }

        /* main loop */
        fscanf(fp, "%d", &m1_row);
        fscanf(fp, "%d", &m1_column);
        for (int i = 0; i < m1_row; i++)
        {
            for (int j = 0; j < m1_column; j++)
            {
                fscanf(fp, "%d", &m1[i][j]);
            }
        }
        fclose(fp);

        write(fd[1], &m1_row, sizeof(int));
        write(fd[1], &m1_column, sizeof(int));
        write(fd[1], m1, sizeof(int) * 100);
        close(fd[1]);
    }

    else
    {
        pid2 = fork();

        /* we are in the child2 process. */
        if (pid2 == 0)
        {
            close(fd2[0]);

            FILE *fp;
            int m2_row, m2_column;
            int m2[MATRIX_SIZE][MATRIX_SIZE];

            /* initialize */
            fp = fopen(argv[2], "r");
            if (fp == NULL)
            {
                fprintf(stderr, "Unable to open matrix file!\n");
            }

            /* main loop */
            fscanf(fp, "%d", &m2_row);
            fscanf(fp, "%d", &m2_column);
            for (int i = 0; i < m2_row; i++)
            {
                for (int j = 0; j < m2_column; j++)
                {
                    fscanf(fp, "%d", &m2[i][j]);
                }
            }

            fclose(fp);
            write(fd2[1], &m2_row, sizeof(int));
            write(fd2[1], &m2_column, sizeof(int));
            write(fd2[1], m2, sizeof(int) * 100);
            close(fd2[1]);
        }
        else
        {
            pid3 = fork();

            /* we are in the child3 process. */
            if (pid3 == 0)
            {
                close(fd[1]);
                close(fd2[1]);
                close(fd3[0]);
                int m1_row, m1_column, m2_row, m2_column;
                int m1[MATRIX_SIZE][MATRIX_SIZE], m2[MATRIX_SIZE][MATRIX_SIZE], OutputMatrix[MATRIX_SIZE][MATRIX_SIZE];

                read(fd[0], &m1_row, sizeof(int));
                read(fd[0], &m1_column, sizeof(int));
                read(fd[0], m1, sizeof(int) * 100);
                read(fd2[0], &m2_row, sizeof(int));
                read(fd2[0], &m2_column, sizeof(int));
                read(fd2[0], m2, sizeof(int) * 100);

                /* multiplication of two matrices */
                int sum = 0;
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
                write(fd3[1], &m1_row, sizeof(int));
                write(fd3[1], &m2_column, sizeof(int));
                write(fd3[1], OutputMatrix, sizeof(int) * 100);
                close(fd[0]);
                close(fd2[0]);
                close(fd3[1]);
            }

            else
            {
                /* we are in the parent process. */
                close(fd3[1]);
                FILE *fp;

                int m1_row, m2_column;
                int OutputMatrix[MATRIX_SIZE][MATRIX_SIZE];

                read(fd3[0], &m1_row, sizeof(int));
                read(fd3[0], &m2_column, sizeof(int));
                read(fd3[0], OutputMatrix, sizeof(int) * 100);

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
                close(fd3[0]);
            }
        }
    }
    return 0;
}
