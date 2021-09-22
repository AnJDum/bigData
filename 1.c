// 用MPI_Reduce接口改写大数组各元素开平方求和 (data[N], data[i]=i*(i+1))的代码(可通过命令行传入 N的值，比如1000，10000，100000)
// mpic++ 1.cpp -o 1
// 
#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int myid, N, numprocs;
    double SqrtSum=0.0, SingleSum=0.0, start, end;
    MPI_Status status;
    start = MPI_Wtime();
    char message[100];
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); 
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); 
    if(myid == 0){
        printf("N: ");
        fflush(stdout);
        scanf("%d", &N);
    }
    MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD); 
    int k;
    for(k = myid; k < N; k+=numprocs){
        SingleSum +=sqrt(k*(k+1));
    }
    printf("Process %d; sum: %f\n", myid, SingleSum);
    //cout << "进程" << myid << ", 和为" << SingleSum << endl;
    MPI_Reduce(&SingleSum, &SqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();
    if (myid == 0){
        printf("Sum: %f; Total time: %f\n", SqrtSum, end - start);
        //cout << "总和：" << SqrtSum << "; 所用时间为: " << end - start << endl; 
    }
    MPI_Finalize();
}