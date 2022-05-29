# Matrix Multiplication
    Matrix multiplication with fork system call and POSIX Threads
    Reads two matrix files by given arguments and outputs their multiplication

## INSTRUCTIONS:
### With Thread:
#### Compile:
    gcc pthread_multiplication.c -o pthread_multiplication -lpthread
#### Run
    ./pthread_multiplication matrix_a.txt matrix_b.txt output_ab.txt
### With Process:
#### Compile:
    gcc process_multiplication.c -o process_multiplication
#### Run
    ./process_multiplication matrix_a.txt matrix_b.txt output_ab.txt

