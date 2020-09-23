========================
IchthyopMPI readme file
================================
http://www.ichthyop.org

Copyright (c) Philippe VERLEY 2016

IchthyopMPI is a rudimentary MPI wrapper to launch batch of Ichthyop simulations with MPI.
The program takes as argument the path of a text file. The text file contains the list of the Java commands to run Ichthyop configuration files.
configuration files.

Example:
# compile
make
# run
mpirun -np 4 ichthyopmpi list.txt
with list.txt:
java -jar ichthyop.jar config-ichthyop.xml.1 > out.1
java -jar ichthyop.jar config-ichthyop.xml.2 > out.2
java -jar ichthyop.jar config-ichthyop.xml.3 > out.3
java -jar ichthyop.jar config-ichthyop.xml.4 > out.4

