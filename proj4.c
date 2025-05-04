#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> //srand(), rand()
#include <time.h> //time()
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "proj4.h"
#include <string.h>

void scSuccessCheck(char * call, int returnVal) {
    if(strcmp("open",call) == 0 && returnVal == -1) {
        perror("There was an issue opening the text file.");
        exit(0);
    } else if(strcmp("read",call) == 0 && returnVal == -1) {
        printf("There was a problem reading the file.\n");
        exit(0);
    } else if(strcmp("lseek",call) == 0 && returnVal == -1) {
        printf("There was an error changing the offeset of the file.\n");
        exit(0);
    } else if(strcmp("write", call) == 0 && returnVal == -1) {
        printf("There was an error writing to the file.\n");
        exit(0);
    } // if
} // scSuccessCheck

void initializeOutputGrid(grid * input, grid * output) {
    output->n = input->n;
    output->p = (unsigned char **) malloc (sizeof(char *) * output->n);
    for(int i = 0; i < output->n; i++) {
        output->p[i] = (unsigned char *) malloc (sizeof(char) * output->n);
        for(int j = 0; j < output->n; j++) {
            memset(output->p[i],'0', sizeof(char)*output->n);
        } // for
    } // for

} // initializeOutputGrid


void initializeGrid(grid * g, char * fileName) { // initializes one ROW at a time
    // grid * g is the whole struct grid since you can treat structs as pointers
    // Figuring out dimensions of grid
    unsigned int dimensions = 0; // Holds the number of rows/columns

    int fd = open(fileName, O_RDONLY, NULL);
    scSuccessCheck("open",fd);

    // getting dimensions of file
    // To count how many rows and columns there are
    int bufSize = 2;
    char * buffer = (char *) malloc (sizeof(char) * bufSize);
    buffer[bufSize - 1] = '\0';

    ssize_t bytesRead = read(fd, buffer, 1); // reading ONE byte at a time to see if it's '\n'
    scSuccessCheck("read",bytesRead);
    while (bytesRead > 0 && buffer[0] != 10) { // While we're not at the end of the file
        dimensions++;
        bytesRead = read(fd, buffer, 1); // reading ONE byte at a time to see if it's '\n'
        scSuccessCheck("read",bytesRead);

    } // while
    free(buffer);
    buffer = NULL;
    // Resetting the offset of the file back to the beginning
    off_t x = lseek(fd, 0, SEEK_SET);
    scSuccessCheck("lseek",x);
    g->n = dimensions; // struct element for dimensions of grid
    // Finished getting dimensions of file!




    g->p = (unsigned char **) malloc (sizeof(char *) * dimensions);
    buffer = (char *) malloc (sizeof(char) * bufSize);
    buffer[bufSize - 1] = '\0';
    // Officially initializing the grid
    for(int i = 0; i < dimensions; i++) {
        g->p[i] = (unsigned char *) malloc (sizeof(char) * dimensions);

        for (int j = 0; j < dimensions; j++) {
            // To read the file in order to initialize the grid

            bytesRead = read(fd, buffer, 1);
            scSuccessCheck("read",bytesRead);
            g->p[i][j] = buffer[0];

        } // for
        // After exit row for loop, buffer will hold '\n' every time; this is so it skips over it
        bytesRead = read(fd, buffer, 1);
        scSuccessCheck("read",bytesRead);

    } // for

    free(buffer);
    close(fd);
} // initializeGrid

void outputGridValueLeftInitialize(grid * input, grid * output, int rowStop, int colStop, int rowStart, int colStart) {
        while(rowStart <= rowStop && colStart <= colStop) {
        output->p[rowStart][colStart] = input->p[rowStart][colStart];
        rowStart++;
        colStart--;
    } // while

} // outputGridValueLeftInitialize

// Will have same logic as the rightDiagSums, but to the left instead
void leftDiagSums(grid * input, unsigned long s, grid * output) {
    unsigned int sum = 0;
    unsigned int nextVal;
    int bounds = input->n; // Indices cannot be >= this value
    int increment;
    for(int i = 0; i < bounds; i++) {
        for(int j =0; j < bounds; j++) {
            sum = input->p[i][j] - '0'; // Current position for now
            if(sum == s) { // If current position is the sum, go ahead and write it
                outputGridValueRightInitialize(input, output, i, j, i, j);
            } else if(i+1 < bounds && j-1 >=0){ // If next value would be in bounds
                increment = 1;
                nextVal = input->p[i + increment][j - increment] - '0'; // Next value
                sum = sum + nextVal;
                while(sum <= s && i + increment < bounds && j - increment >= 0) { // While both values added are <= the target sum and in bounds
                    if(sum == s) {
                        outputGridValueLeftInitialize(input, output, i+increment, j+increment, i, j);
                        sum++;
                    } else {
                        increment++;
                        if (i + increment < bounds && j - increment >= 0) {
                            nextVal = input->p[i + increment][j - increment] - '0'; // Next value
                            sum = sum + nextVal;
                        } // if
                    } // else
                } // if
            } // if
        } // for
    } // for


} // leftDiagSums

void outputGridValueRightInitialize(grid * input, grid * output, int rowStop, int colStop, int rowStart, int colStart) {
    while(rowStart <= rowStop && colStart <= colStop) {
        output->p[rowStart][colStart] = input->p[rowStart][colStart];
        rowStart++;
        colStart++;
    } // while

} // outputGridValueInitialization

void rightDiagSums(grid * input, unsigned long s, grid * output) {
    unsigned int sum = 0;
    unsigned int nextVal;
    int bounds = input->n; // Indices cannot be >= this value
    int increment;
    for(int i = 0; i < bounds; i++) {
        for(int j =0; j < bounds; j++) {
            sum = input->p[i][j] - '0'; // Current position for now
            if(sum == s) { // If current position is the sum, go ahead and write it
                outputGridValueRightInitialize(input, output, i, j, i, j);
            } else if(i+1 < bounds && j+1 < bounds){ // If next value would be in bounds
                increment = 1;
                nextVal = input->p[i + increment][j + increment] - '0'; // Next value
                sum = sum + nextVal;
                while(sum <= s && i + increment < bounds && j + increment < bounds) { // While both values added are <= the target sum and in bounds
                    if(sum == s) {
                        outputGridValueRightInitialize(input, output, i+increment, j+increment, i, j);
                        sum++;
                    } else {
                        increment++;
                        if (i + increment < bounds && j + increment < bounds) {
                            nextVal = input->p[i + increment][j + increment] - '0'; // Next value
                            sum = sum + nextVal;
                        } // if
                    } // else
                } // if
            } // if
        } // for
    } // for

} // rightDiagSums

// Function for second pthread; calculates the diagonal sums to the left
void * secondThread(void * args) {
    struct pthread_create_args * pArgs = (struct pthread_create_args *) args;
    leftDiagSums(pArgs->input, pArgs->s, pArgs->output);
    return NULL;
} // secondThread

// Function for third thread; calculates the diagonal sums to the right
void * thirdThread(void * args) {
    struct pthread_create_args * pArgs = (struct pthread_create_args *) args;
    rightDiagSums(pArgs->input, pArgs->s, pArgs->output);
    return NULL;
} // secondThread

void diagonalSums(grid * input, unsigned long s, grid * output, int t) {

    if(t == 1) { // If only ONE thread
        initializeOutputGrid(input, output);
        rightDiagSums(input, s, output);
        leftDiagSums(input, s, output);
    } else if(t > 1) {
        if(t == 2) {
            // MAIN thread
            initializeOutputGrid(input, output);
            rightDiagSums(input, s, output);
            struct pthread_create_args pArgs;
            pArgs.input = input;
            pArgs.output = output;
            pArgs.s = s;
            pthread_t pthread2;
            int x; // to check if thread successfully created
            x = pthread_create(&pthread2, NULL, secondThread, &pArgs);
            if(x != 0) {
                printf("Error creating thread\n");
            } // if
            int y;

            y = pthread_join(pthread2, NULL);
            if(y != 0) {
                printf("Error joining thread2 with the first thread\n");
            } // if
        } else if(t==3) {
            // MAIN thread initializes the output grid
            initializeOutputGrid(input, output);
    struct pthread_create_args pArgs;
            pArgs.input = input;
            pArgs.output = output;
            pArgs.s = s;
            pthread_t pthread2;
            int x; // to check if thread successfully created
            pthread_t pthread3;
            int z; // to check if thread successfully created

            // Creating threads
            x = pthread_create(&pthread2, NULL, secondThread, &pArgs);
            if(x != 0) {
                printf("Error creating thread\n");
            } // if
            z = pthread_create(&pthread3, NULL, thirdThread, &pArgs);
            if(z != 0) {
                printf("Error creating thread\n");
            } // if

            // Joining threads
            int y;
            y = pthread_join(pthread2, NULL);
            if(y !=0) {
                printf("Error joining thread2 with the first thread\n");
            } // if

            int a;
            a = pthread_join(pthread3, NULL);
            if(a !=0) {
                printf("Error joining thread3 with the first thread\n");

            } // if
        } // if
    }
} //diagonalSums

// input is the output grid and output file
// writing output from diagonal sums method into an actual file
void writeGrid(grid * g, char * fileName) {
    int fd;
    fd = open(fileName, O_RDWR | O_CREAT | O_TRUNC, 0600);
    scSuccessCheck("open", fd);

    // create buffer
    char * buf = (char *) malloc(sizeof(char) * 2);
    buf[1] = '\0'; // Must end with a null-terminator char
    int retVal; // return value of write
    for(int i = 0; i < g->n; i++) {
        for(int j = 0; j < g->n; j++) {
            buf[0] = g->p[i][j];
            retVal = write(fd, buf, 1);
            scSuccessCheck("write", retVal);
        } // for
        buf[0] = '\n';
        retVal = write(fd, buf, 1);
        scSuccessCheck("write", retVal);

    } // for

    // Freeing/closing
    retVal = close(fd);
    free(buf);
    scSuccessCheck("close", retVal);

} // writeGrid

void freeGrid(grid * g) {
    for(int i = 0; i < g->n; i++) {
            free(g->p[i]);
    } // for
    free(g->p);
} // freeGrid
