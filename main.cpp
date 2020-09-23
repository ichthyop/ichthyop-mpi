/*
 * Rudimentary applications to launch batch of Ichthyop simulations with MPI.
 * The program takes as argument the path of a text file. The text file contains
 * the list of Java commands for launching Ichthyop simulations.
 * 
 * Example:
 * mpirun -np 4 ichthyopmpi list.txt
 * with list.txt:
 * java -jar ichthyop.jar config-ichthyop.xml.1 > out.1
 * java -jar ichthyop.jar config-ichthyop.xml.2 > out.2
 * java -jar ichthyop.jar config-ichthyop.xml.3 > out.3
 * java -jar ichthyop.jar config-ichthyop.xml.4 > out.4
 * etc.
 * 
 * File:   main.cpp
 * Author: P. Verley (philippe.verley@ird.fr
 */

#include <cstdlib>
#include <mpi.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/* Print details in the console */
const bool DEBUG=false;

/* Functions headers */
void readFile(const char *filename, vector<string> &files);
void checkFileExist(const char *filename);

// MPI rank
int rank;

/*
 * 
 */
int main(int argc, char** argv)
    {

    int rank= -1, size=0;

    if (argc!=2)
        {
        stringstream msg;
        msg<<"You must provide the path of the text file containing the list of Ichthyop configuration files."<<endl;
        cerr<<msg.str();
        return 1;
        }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (DEBUG)
        {
        stringstream msg;
        msg<<"Rank "<<rank<<" | Ichthyop configuration files read from "<<argv[1]<<endl;
        cout<<msg.str();
        }
    vector<string> files=vector<string>();
    readFile(argv[1], files);

    if (files.size()<size)
        {
        stringstream msg;
        msg<<"Rank "<<rank<<" | Number of configuration files in "<<argv[1]<<" must be >= the number of MPI procs. nFile "<<files.size()<<" nProc "<<size<<endl;
        cerr<<msg.str();
        return 1;
        }

    for (int iSimu=0; iSimu<files.size(); iSimu++)
        {
        if ((iSimu%size)==rank)
            {
            //            stringstream msg;
            //            msg<<"Rank "<<rank<<" | Running Ichthyop simulation from line "<<iSimu<<" ..."<<endl;
            //            cout<<msg.str();
            int err=system(files.at(iSimu).c_str());
            if (err!=0)
                {
                stringstream msg;
                msg<<"Rank "<<rank<<" | Failed to run Ichthyop simulation:"<<endl<<files.at(iSimu).c_str()<<endl;
                cerr<<msg.str();
                }
            else
                {
                stringstream msg;
                msg<<"Rank "<<rank<<" | Completed Ichthyop simulation:"<<endl<<files.at(iSimu).c_str()<<endl;
                cout<<msg.str();
                }
            }
        }

    MPI_Finalize();

    return 0;
    }

void readFile(const char *filename, vector<string> &files)
    {

    checkFileExist(filename);
    stringstream msg;

    ifstream file(filename, ios::in);
    if (file)
        {
        string line;
        while (getline(file, line))
            {
            if (line.empty()||line[0]=='#') continue;
            files.push_back(line);
            if (DEBUG)
                {
                msg<<"Rank "<<rank<<" | readFile() Adding command: "<<line<<" [OK]"<<endl;
                cout<<msg.str();
                }
            }
        file.close();
        }
    else
        {
        msg<<"Rank "<<rank<<" | readFile() Failed to read file "<<filename<<endl;
        cerr<<msg.str();
        }
    }

void checkFileExist(const char *filename)
    {
    FILE* f;
    if (!(f=fopen(filename, "r")))
        {
        cout<<"Rank "<<rank<<" | checkFileExist() Failed to open file: "<<filename<<'\n';
        exit(1);
        }
    fclose(f);
    }

