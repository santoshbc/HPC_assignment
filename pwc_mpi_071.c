#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the code. 

  Compile with:
    mpicc -o pwc_mpi_071 pwc_mpi_071.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    mpirun -np 3 ./pwc_mpi_071 

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

//Encrypted passwords are PO1234,BA1111, CA2222, DA3333.

"$6$KB$hRRqurIwHk3qy.1pznj4sSV61i0UyD9H5aTLGKDIq2N9H9Yd9ctTD1Ii1PSajs7TnwnZ1CUbovYOWY9SkgT2S.",

"$6$KB$A2nYXLJfEPiCpCQdLBG/iK.VbnRUX25gtSvn1UrHS0WFLiGsY576XSOb6KwUvasq7xXMW9ZBtKJQYLM0O0atV.",

"$6$KB$jcm2lICxLGaPvqhzTyL1eVDtcM1XUbwFxIFDRrbqNYWKEfgm13pIoB1Cq6kS/OU7MGrXlH5XwS/w1gSe33Mil.",

"$6$KB$ah3qNB.7mJMTd9S.5NDs28PwKIm.jNl9/r9MGLtDQDAaO/ekzejGJ93kfCApTvSeKgITOl/rb.syvtbzlwfdN1"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack1(char *salt_and_encrypted){
  int x, y, z, a;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
	for(a=0; a<=99; a++){
        sprintf(plain, "%c%c%02d%2d", x, y, z, a); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
	}
        }
      }
    }
  }
  //printf("%d solutions explored\n", count);
}

void crack2(char *salt_and_encrypted){
  int x, y, z, a;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='N'; x<='Z'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){ 
	for(a=0; a<=99; a++){
        sprintf(plain, "%c%c%02d%2d", x, y, z, a); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        }
	}
      }
    }
  }
  //printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  
  return !(*difference > 0);
}


int main(int argc, char *argv[]){

  struct timespec start, finish;
  long long int difference;   
  
  int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {

    clock_gettime(CLOCK_MONOTONIC, &start);

    if(rank ==0){
      int a;
      int b;
      int i;
      MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
      MPI_Send(&b, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	
    } else {
      if(rank == 1){
	int i;
        int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i++){
	 crack1(encrypted_passwords[i]);
	}
      }
      else if(rank == 2){
	int i;
      	int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i++){
	   crack2(encrypted_passwords[i]);
	}
      }
    }
  }
  MPI_Finalize(); 

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);

  printf("Time elapsed was %9.5lldns\n", difference);

  return 0;
}


