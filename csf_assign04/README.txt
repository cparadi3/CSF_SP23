JHED: cparadi3 jzahner4
Names: CJ Paradis Joe Zahner

CONTRIBUTIONS
Both members contributed equally.

REPORT
Test run with threshold 2097152

real    0m0.355s
user    0m0.340s
sys     0m0.013s
Test run with threshold 1048576

real    0m0.222s
user    0m0.362s
sys     0m0.031s
Test run with threshold 524288

real    0m0.187s
user    0m0.414s
sys     0m0.044s
Test run with threshold 262144

real    0m0.163s
user    0m0.512s
sys     0m0.067s
Test run with threshold 131072

real    0m0.147s
user    0m0.510s
sys     0m0.077s
Test run with threshold 65536

real    0m0.138s
user    0m0.524s
sys     0m0.094s
Test run with threshold 32768

real    0m0.184s
user    0m0.572s
sys     0m0.142s
Test run with threshold 16384

real    0m0.163s
user    0m0.605s
sys     0m0.216s

Part 2:

The more subprocesses there are, the more they need to communicate with each other and 
the OS kernel. After a certain point (somewhere around the threshold of 65536) there are 
so many of these communications that it is inefficient to split the operations any more. 
Before this point, the simultaneous processes save time because multiple arrays can be 
sequentially sorted at the same time, and this speed advantage outweighs the cost of these
communications with the OS kernel. 