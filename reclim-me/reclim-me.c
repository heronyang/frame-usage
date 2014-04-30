#include<stdio.h>
#include<stdlib.h>

char* buffer0;;
int main(){

    int i = 16000000;
    char *buffer1;

    getchar();

    buffer0 = (char*) malloc(i);
    buffer1 = (char*) malloc(i);
    getchar();
    int n;
    for(n = 0 ; n<i ; n++){
        buffer1[n] = n%26+'a';
        buffer0[n] = n%26+'a';
    }

    getchar();

    return 0;
}
