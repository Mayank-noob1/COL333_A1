
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include<iostream>
#include<vector>
#include<utility>
#include<cmath>
#include<algorithm>
#include<set>
#include<map>

using namespace std;

class SportsLayout{

    private:
    int z,l;
    int** T;
    int **N;
    int time_;
    int *mapping;
    int **beam;
    int *check;
    int sz;
    int *shuff;
    public:
    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();
    long long cost_fn(int arr[]);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);
    long long cost_fn_change(int,int,int*);
    void compute_allocation();
    void generate_random_successor (int);
    long long greedy_successor(int *parent);
};


#endif