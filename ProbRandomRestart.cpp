#include <fstream>
#include <iostream>
#include<vector>
#include<utility>
#include<ctime>
#include<algorithm>
#include <cstdlib> 
#include<iterator>
#include <random>
using namespace std;


#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
        readInInputFile(inputfilename);
        mapping= new int[z];
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

    // void SportsLayout::readOutputFile(string output_filename)
    // {
    //         fstream ipfile;
	//         ipfile.open(output_filename, ios::in);
    //         if (!ipfile) {
    //             cout << "No such file\n";
    //             exit( 0 );
    //         }
    //         else {
                
    //             vector<int> ip;

    //             while (1) {
    //                 int t;
    //                 ipfile >> t;
    //                 ip.push_back(t);
    //                 if (ipfile.eof())
    //                     break;
                    
    //             }
            
    //         if(ip.size()!=z)
    //         {
    //             cout<<"number of values not equal to number of zones, check output format\n";
    //             exit(0);
    //         }
    //         for(int i=0;i<z;i++)
    //         mapping[i]=ip[i];
    //     ipfile.close();

    //     if(!check_output_format())
    //         exit(0);
    //     cout<<"Read output file, format OK"<<endl;

    //         }
        
    // }

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

    void SportsLayout::readInInputFile(string inputfilename)
    {
            fstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                

                ipfile>> time_;
                ipfile >> z;
                ipfile >> l;

                if(z>l)
                {
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }
            time_ = 60*time_;
            int **tempT;
            int **tempN;

          tempT = new int*[l];
          shuff = new int [l];
          int seed = rand()%l +1;
         for (int i = 0; i < l; ++i,++seed){
            if (seed > l){seed = 1;}
            shuff[i] = seed;
            tempT[i] = new int[l];
            }
        
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
    outputFile <<'\n';
    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;

    }

    long long SportsLayout::cost_fn(int map[]){
        long long cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[map[i]-1][map[j]-1];
           }
        }
        return cost;
    }

    long long SportsLayout::cost_fn_change(int i, int j,int parent[]){
        long long change = 0;
        for(int k=0;k<z;k++)
        {
            if(k == j or k == i){
                continue;
            }
            change -= (long long)N[i][k]*(long long)(T[parent[i]-1][parent[k]-1] - T[parent[j]-1][parent[k]-1]);
            change -= (long long)N[k][i]*(long long)(T[parent[i]-1][parent[k]-1] - T[parent[j]-1][parent[k]-1]);
            if(j < z){
                change -= (long long)N[j][k]*(long long)(T[parent[j]-1][parent[k]-1] - T[parent[i]-1][parent[k]-1]);
                change -= (long long)N[k][j]*(long long)(T[parent[j]-1][parent[k]-1] - T[parent[i]-1][parent[k]-1]);
            }
        }
        return change;
    }
    
    long long SportsLayout::greedy_successor(int *parent){
        long long c_ = cost_fn(parent);
        long long min_c=0;
        int x=0,y=0;
        // cout<<z<<' '<<l<<'\n';
        for(int i =0;i<z;i++){
            for(int j=i+1;j<l;j++){
                // cout<<i<<' '<<j<<'\n';
                long long c = cost_fn_change(i,j,parent);
                // cout<<i<<' '<<j<<'\n';
                if (c < min_c){
                    x=i;y=j;
                    min_c = c;
                }
            }
        }
        swap(parent[x],parent[y]);
        return c_+min_c;
    }

    void SportsLayout::generate_random_successor (){
        int ra = rand();
        for(int i=0;i<l;i++){
            shuff[i] = ra%l+1;
            ra++;
        }
        if (rand()%l == 0){
            return;
        }
        random_device rd;  
        mt19937 g(rd());
        shuffle(shuff, shuff+l, g); 
        // random_shuffle(shuff,shuff+l);
    }
    
    void SportsLayout::compute_allocation()
    {
        time_t crnt_time = time(NULL);
        srand(time(0));
        int best [l];
        for(int i=0;i<l;i++){
        best[i]=i+1;
        }
        for(int i=0;i<z;i++){
            mapping[i]=best[i];
        }
        long long cost=cost_fn();
        int restart = 0;
        int iteration=0;
        while (true){
            restart++;
            generate_random_successor();
            double time_taken = double(time(NULL) - crnt_time);
            cout << fixed<< time_taken << setprecision(5);
            cout << " sec ";
            cout<< iteration<<" iteration "<< cost<<" cost\n";
            long long c=cost_fn(shuff);
            if (cost > c){
                cost = c;
                copy(shuff, shuff+z, best);
            }
            while (true)
            {
                if ((rand()%(l*l))< z){
                    generate_random_successor();
                    long long c=cost_fn(shuff);
                    if (cost > c){
                        cost = c;
                        copy(shuff, shuff+z, best);
                    }
                }
                iteration++;
                long long c_=greedy_successor(shuff);
                if ((c > c_)){
                    c = c_;
                }
                else{
                    break;
                }
                if ((time(NULL)-crnt_time) >= this->time_){
                    if (cost > c){
                        cost = c;
                        copy(shuff, shuff+z, best);
                    }
                    break;
                }
            }
            if (cost > c){
                cost = c;
                copy(shuff, shuff+z, best);
            }
            if ((time(NULL)-crnt_time) >= this->time_){
                    break;
            }
        }
        for(int i=0;i<z;i++){
        mapping[i]=best[i];
        }
        cout<<"Iter -> "<<iteration <<", Restart -> "<<restart<<'\n';
    }