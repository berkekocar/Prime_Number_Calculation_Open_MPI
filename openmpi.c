# include <mpi.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <time.h>

int main ( int argc, char *argv[] );
int prime_number (int n, int id, int p);

int main ( int argc, char *argv[] )

{
  int id,n,range,n_higher,n_lower,p,primes,primes_part,myid,numprocs;
  double t1, t2;


  n_lower = 1;
  n_higher = 100000;
  range = 1000;

MPI_Init (&argc, &argv);
t1 = MPI_Wtime();
MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
MPI_Comm_rank (MPI_COMM_WORLD, &myid);
MPI_Comm_size ( MPI_COMM_WORLD, &p );
MPI_Comm_rank ( MPI_COMM_WORLD, &id );


  n = n_lower;

  while (n <= n_higher)
  {
    MPI_Bcast ( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );

    primes_part = prime_number ( n, id, p );

    MPI_Reduce ( &primes_part, &primes, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );

    n = n * range;
  }
  t2 = MPI_Wtime();

  printf ("%s\n", "Using 8 tasks to scan 10000000 numbers");
  printf ("%s\n", "MPI ( 8 Threads)");
  printf ("Total primes: %d\n", primes);
  printf ("Time elapsed: %f\t", t2 - t1);

  MPI_Finalize ( );

  return 0;
}

int prime_number ( int n, int id, int p )
{
  int i;
  int j;
  int prime;
  int sum;

  sum = 0;

  for ( i = 2 + id; i <= n; i = i + p )
  {
    prime = 1;
    for ( j = 2; j < i; j++ )
    {
      if ( ( i % j ) == 0 )
      {
        prime = 0;
        break;
      }
    }
    sum = sum + prime;
  }
  return sum;
}
