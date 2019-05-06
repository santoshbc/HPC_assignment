#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <crypt.h>
#include <time.h>



/****************************************************************************
  The variable names and the function names of this program is same as provided by the university.
  The added variable and function are the only changes made to this program.

  Compile and run with:
     cc -o passwordcracking_071 passwordcracking_071.c -lcrypt -pthread
    ./passwordcracking_071 > results.txt
  
*****************************************************************************/


int n_passwords = 4;
pthread_t thread1, thread2;
char *encrypted_passwords[] = {

"$6$KB$yPvAP5BWyF7oqOITVloN9mCAVdel.65miUZrEel72LcJy2KQDuYE6xccHS2ycoxqXDzW.lvbtDU5HuZ733K0X0",

"$6$KB$rOGcNtZd4lOABjC1NL933L/bSqkiX4Cz9zj.PyJnV6NMZgRYii2piqQuVns0q/PoBJefsqgVNOHKqg41thBCt/",

"$6$KB$6NNNfaih3/Rud9hmARNkALWBxu2vb/hpZHTdcX4tnafx0GczkCxzOkccgxTk9t2WQDpuc9ixARkLLBVplQlh0.",

"$6$KB$A0hfFgW62.QrZIf3y.eOxR7FFFAqFe/yFD5q2hIxSbom5AHJ8WkcS3w9dLoKIW7kwtD4207d7IfnpiPBgYz/T."
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
  
void *Fun_f1(void *salt_and_encrypted){

int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(x='A'; x<='M'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		//printf("Thread1");
		sprintf(plain, "%c%c%02d",x, y, z);
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
 // printf("%d solutions explored\n", count);

}
void *Fun_f2(void *salt_and_encrypted){

int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(x='N'; x<='Z'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		
		sprintf(plain, "%c%c%02d", x, y, z);
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
  int i;
struct timespec start, finish;
  long long int time_elapsed;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i=0;i<n_passwords;i<i++) {
  pthread_create(&thread1, NULL, Fun_f1, encrypted_passwords[i]);
  pthread_create(&thread2, NULL, Fun_f2, encrypted_passwords[i]);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);		

  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9));
  return 0;
}
