Linpack Xtreme v1.1.3 by Regeneration

Linpack Xtreme is a console front-end with the latest build of Linpack
(Intel Math Kernel Library Benchmarks 2018.3.011). Linpack is a benchmark
and the most aggressive stress testing software available today. Best used
to test stability of overclocked PCs. Linpack tends to crash unstable PCs
in a shorter period of time compared to other stress testing applications.

Linpack solves a dense (real*8) system of linear equations (Ax=b), measures
the amount of time it takes to factor and solve the system, converts that
time into a performance rate, and tests the results for accuracy. The
generalization is in the number of equations (N) it can solve, which is
not limited to 1000. Linpack uses partial pivoting to assure the accuracy
of the results.

Linpack Xtreme was created because Prime95 is no longer effective like it
used to be. LinX, IntelBurnTest, OCCT use outdated Linpack binaries from
2012. Modern hardware requires modern stress testing methodology with
support for the latest instructions sets.

Make sure to keep an eye on the temperatures as Linpack generates excessive
amount of stress like never seen before.

Instructions:
Edit the file 'settings' to customize the stress test parameters. Define
the number of runs, set problem size and leading dimensions according to
the desired amount of RAM to be used:

15825 for 2GB
22611 for 4GB
27818 for 6GB
32209 for 8GB
35000 for 9.6GB

Set chmod +x on the run scripts (type 'chmod +x run*' in terminal) and
execute:

./run_benchmark
./run_stress_test

Output will be saved in a file named results.txt.

Website:
https://www.ngohq.com/linpack-xtreme.html
