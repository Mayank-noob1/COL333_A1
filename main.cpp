#include <iostream>
// #include <bits/stdc++.h>
#include <fstream>

#include "SportsLayoutRW.h"

using namespace std;

int main(int argc, char** argv )
{

     // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    clock_t start = clock();
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    
    SportsLayout *s  = new SportsLayout( inputfilename );
    s->compute_allocation(outputfilename);
    s->write_to_file(outputfilename);

    long long cost = s->cost_fn ();
    cout<< "cost:"<<cost<<endl;
    clock_t end = clock();
    double milliseconds = (double)(end - start) / (CLOCKS_PER_SEC / 1000.0);
    // cout<<milliseconds<<"\n";
    return 0;

}