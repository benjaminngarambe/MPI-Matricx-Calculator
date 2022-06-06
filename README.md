# Matrix-Multiplication-Using-MPI
The principal purpose of this project is to perform various operations on multiprocessing and record the time taken for computation with serial computation on the same functions. 
For this work, I used matrix multiplication as it requires NxN operations to compute if N is the matrix size.

### Executing the cpp file

- Open the Terminal
- Compile the .cpp file
    - Method 1 
        - g++ -o matrix_multiplication matrix_multiplication.cpp
    - Method 2 (Better Approach)
        - mpicxx -o matrix_multiplication matrix_multiplication.cpp
- Execution
    - mpirun -np number_of_cores ./matrix_multiplication
