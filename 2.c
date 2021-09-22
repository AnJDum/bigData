#include<stdio.h>
#include<math.h>
#include <mpi.h>
#define N 1000000000.0
#define a 10.0
#define b 100.0

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
	int myid, numprocs;
	double start, end, x, local=0.0, total=0.0;
    double dx = (b-a) / N;
    MPI_Status status;
	start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    --numprocs;

	if (myid != 0){
        for(int i=myid; i<=N; i+=numprocs){
            x = a + i*dx + dx/2;
            local +=x*x*x*dx;
        }
		printf("Process %d; sum: %f\n", myid, local);
//cout << "进程" << myid << "的计算结果为: " << local << endl;
		MPI_Send(&local, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}
	else{
		for (int source = 1; source <=numprocs; source++)
		{
			MPI_Recv(&local, 1, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
			total +=local;
		}
	}
	end = MPI_Wtime();
	if (myid == 0) 
		printf("Outcome %f; time: %f\n", total, end - start);

        //cout << "积分结果为: " << total << "; 所用时间为: " << end - start << endl;

	MPI_Finalize();
}
//mpic++ 5.cpp -o 5
//mpirun -n 4 ./5