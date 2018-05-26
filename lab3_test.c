#include "types.h"
#include "user.h"

int test(int n) { 
    int x = n + 1;
    return x;
}

int fibonacci(int f) {
    if(f == 0 || f == 1) {
        return f;
    }
    return fibonacci(f-1) + fibonacci(f-2);
}

int main(int argc, char *argv[]) {
    printf(1, "####################################################\n");
    printf(1, "# This program tests the correctness of your lab #3\n");
    printf(1, "####################################################\n");

    printf(1, "Test 1: Simple test\n");
    int pid = 0;
    pid = fork();
    if(pid == 0){
        test(1);
        exit();
    }
    wait();

    printf(1, "Test 2: Stack growth test.\n");
    // Write your own test
    fibonacci(75);
    exit();
    return 0;
}
