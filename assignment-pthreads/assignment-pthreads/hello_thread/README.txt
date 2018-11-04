For the sequential code the time taken is:

seq: starting bench at Sat Sep 22 01:15:57 EDT 2018
seq: ending bench at Sat Sep 22 01:25:11 EDT 2018



I tried running the hello_thread.cpp with input as 4 and it gives the following output:

I am 3 of 4
I am 3 of 4
I am 3 of 4
I am 3 of 4

The way the execution happens is:
1. We first go to i = 0, and start the thread.
2. By the time the thread executes the function string 'I am %d of %d', the iterations of the main function completes and we get the above output.

We can clearly see the parallelism when threads = 25

I am 3 of 25
I am 3 of 25
I am 7 of 25
I am 4 of 25
I am 5 of 25
I am 5 of 25
I am 8 of 25
I am 6 of 25
I am 9 of 25
and so on
