# CSCI 1730 Systems Programming @UGA Project 4 - Multithreaded Diagonal Sums

## Project Description
This project will find all of the diagonal sums equal to an input *s*, an unsigned long value, in an input text file consisting of a 2D grid of *n*-by-*n* digits (1 through 9), where *n*>=1, using *t* threads, where 1<=*t*<=3. Program will write the output of all the diagonal sums equal to *s* to an output file. 

'example' directory provided for input grids, as well as what the correct output should be (as long as the target sum is correct as well).

## Compiling and Running
To compile: `make`
To run: `./proj4.out [name of input txt file containing 2D grid] [name of output txt file] [integer target sum] [thread count]; [additional args]`

## Example Case
To test with `in1.txt` and compare output to `correctOut1.txt`, target sum should = 10.

To test with `in2.txt` and compare output to `correctOut2.txt`, target sum should = 19.

To test with `in3.txt` and compare output to `correctOut3.txt`, target sum should = 3.

To test with `in4.txt` and compare output to `correctOut4.txt`, target sum should = 13.

To test with `in5.txt` and compare output to `correctOut5.txt`, target sum should = 1222.

### Input that will compare the content of the output file to another file that contains what the resulting output file should hold.

`./proj4.out in4.txt out4.txt 13 2; diff out4.txt correctOut4.txt | wc -c;`

### Output (elapsed time will differ among test cases)
`Computing the diagonal sums equal to 13 in a 2778-by-2778 grid using 2 thread(s).`

`Elapsed time for computing the diagonal sums using 2 thread(s): 0.215538 seconds.`

`Writing the diagonal sums equal to 13 to the file out4.txt`

`Program is complete. Goodbye!`

`0 `
