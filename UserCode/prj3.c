#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "buffer.h"

int NUM_INSERTS = 100000;

char zeros[1024] = {0};
char ones[1024] = {1};
char twos[1024] = {2};
char threes[1024] = {3};
char fours[1024] = {4};
char fives[1024] = {5};
char sixes[1024] = {6};
char sevens[1024] = {7};
char eights[1024] = {8};
char nines[1024] = {9};

char user1024[1024] = {};

char numArray[2][1024] = {{0},{1}};

void random_insert(){

	

}

void random_read(){


}



int main(){

printf("Hello World\n");

for (int i = 0; i < 10; i++){
	printf("digit: %d\n",numArray[0][i]);
}

long num = init_buffer_421();

return 0;

}
