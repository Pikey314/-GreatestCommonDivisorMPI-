#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>    
#include <time.h>       


int nwd(int a,int b)
{
    int c;                    
    while(b != 0)             
    {
        c=a % b;             
        a = b;               
        b = c;               
    }
    return a;                
}

int nadawca(const int world_rank, const int world_size, const int krok)
{
    if (world_rank - krok >= 0)
        return world_rank - krok;
    else
        return world_size + (world_rank - krok);
}

int main(int* argc, char*** arg )
{
    int moj, pobrany;
    int world_size;
    int world_rank;
    int i;
    MPI_Request request;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) + world_rank);
    srand(rand() % rand());
    moj = rand() % 10000 + 1;
    moj *= 19;

    printf("Id %d - moja liczba poczatkowa to: %d\n", world_rank, moj);

    for (i = 1; i < world_size; i++)
    {
        MPI_Isend(&moj, 1, MPI_INT, (world_rank+i)%world_size,
                    0, MPI_COMM_WORLD, &request);
        MPI_Recv(&pobrany, 1, MPI_INT, nadawca(world_rank, world_size, i),
                    0, MPI_COMM_WORLD, &status);
        printf("Krok %d, id %d, nwd(%d,%d)", i, world_rank, moj, pobrany);
        moj = nwd(moj, pobrany);
        printf("=%d\n", moj);
    }


    MPI_Finalize();
    return 0;
}


