#include <fstream>
#include <iostream>
#include <random>

using namespace std;

#include "SportsLayoutRW.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
        readInInputFile(inputfilename);
        mapping= new int[l];
        revMapping = new int[l];
    }
    bool SportsLayout::check_output_format()
    {
        vector<bool> visited(l,false);
        for(int i=0;i<z;i++)
        {
            if((mapping[i]>=1 && mapping[i]<=l))
            {
                if(!visited[mapping[i]-1])
                visited[mapping[i]-1]=true;
                else
                {
                    cout<<"Repeated locations, check format\n";
                    return false;
                }
            }
            else
            {
                cout<<"Invalid location, check format\n";
                return false;
            }
        }

        return true;

    }
    long long SportsLayout::cost_fn(){
        long long cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
           }
        }
        return cost;
    }
    long long SportsLayout::cost_fn_change(int i, int j, int* Nmapping){
        long long change = 0;
        for(int k=0;k<z;k++)
        {
            if(k == j or k == i){
                continue;
            }
            change -= (long long)N[i][k]*(long long)(T[Nmapping[i]-1][Nmapping[k]-1] - T[Nmapping[j]-1][Nmapping[k]-1]);
            change -= (long long)N[k][i]*(long long)(T[Nmapping[i]-1][Nmapping[k]-1] - T[Nmapping[j]-1][Nmapping[k]-1]);
            if(j < z){
                change -= (long long)N[j][k]*(long long)(T[Nmapping[j]-1][Nmapping[k]-1] - T[Nmapping[i]-1][Nmapping[k]-1]);
                change -= (long long)N[k][j]*(long long)(T[Nmapping[j]-1][Nmapping[k]-1] - T[Nmapping[i]-1][Nmapping[k]-1]);
            }
        }
        return change;
    }
    void SportsLayout::readInInputFile(string inputfilename)
    {
            fstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                ipfile>> time;
                ipfile >> z;
                ipfile >> l;

                if(z>l)
                {
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }
            int **tempT;
            int **tempN;
          tempT = new int*[l];
         for (int i = 0; i < l; ++i)
            tempT[i] = new int[l];
        
        tempN = new int*[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new int[z];

        for(int i=0;i<z;i++)
        {
            for(int j=0;j<z;j++)
            ipfile>>tempN[i][j];
        }

        for(int i=0;i<l;i++)
        {
            for(int j=0;j<l;j++)
            ipfile>>tempT[i][j];
        }

        ipfile.close();

        T= tempT;
        N= tempN;
            }

    }

    void SportsLayout::write_to_file(string outputfilename){

         // Open the file for writing
    ofstream outputFile(outputfilename);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    for(int i=0;i<z;i++)
    outputFile<<mapping[i]<<" ";

    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;

    }

    void SportsLayout::compute_allocation(string outputfilename)
    {
        double timeMilli = time*60000 - 2000;
        random_device rd;
        mt19937 generator(rd());
        clock_t start = clock();
        int high_index = 0;
        int pmapping[l];
        long long optim_cost = LLONG_MAX;
        uniform_int_distribution<int> distribution(0, l - 1);
        int index = 0;
        while(true){
            if(index >= 10000){
            int ran[l];
            for (int i = 0; i < l; i++)
            {
                ran[i] = pmapping[i];
            }
            int e = distribution(generator);
            shuffle(ran + e, ran + min(l, e + l / 4), generator);
            for(int i = 0; i < l; i++){
                mapping[i] = ran[i];
            }}else{
                int ran[l];
            for (int i = 0; i < l; i++)
            {
                ran[i] = i + 1;
            }
            shuffle(ran , ran + l, generator);
            for(int i = 0; i < l; i++){
                mapping[i] = ran[i];
            }
            }
            long long temp_cost = cost_fn();
            if(temp_cost < optim_cost){
                for (int o = 0; o < l; o++)
                {
                    pmapping[o] = mapping[o];
                }
                optim_cost = temp_cost;
            }

            while (true)
            {
                long long res_cost = temp_cost;
                bool found = false;
                index += 1;
                int m = -1, n = -1;
                for(int i = 0; i < z and not found; i++){
                    for (int j = i + 1; j < l; j++){
                        long long u = cost_fn_change(i, j, mapping);
                        if(temp_cost >  u + res_cost){
                            m = i;
                            n = j;
                            found = true;
                            temp_cost =  u + res_cost;
                            if(temp_cost < optim_cost){
                                for (int o = 0; o < l; o++)
                                {
                                    pmapping[o] = mapping[o];
                                }
                                swap(pmapping[i], pmapping[j]);
                                optim_cost = temp_cost;
                            }
                        }
                    }
                }
                index += 1;
                clock_t end = clock();
                double milliseconds = (double)(end - start) / (CLOCKS_PER_SEC / 1000.0);
                high_index = max(high_index, index);
                if(milliseconds > timeMilli){
                    for (int o = 0; o < l; o++){
                        mapping[o] = pmapping[o];
                    }
                    return;
                }
                if(m == -1 and n == -1){
                    break;
                }else{
                    swap(mapping[m], mapping[n]);
                }
            }
            index += 1;
        }

    }