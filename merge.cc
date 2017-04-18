#include <iostream>
#include <stdlib.h>
#include "util.h"
#include "omp.h"
#include <stdio.h>
#include <time.h>
using namespace std;

int main(int argc, char *argv[]) {

	// Combine the fasta files in the directory
	// directory path must end with '/'
	// the combined file will be ./tmp.fa

	time_t t1,t2;
	t1=clock();
	string dir_path = "./data/";
	vector<string> files = listFastaFiles(dir_path);
	combineFastaFile(dir_path, files);
	t2=clock();
	cout<<(double)(t2-t1)/CLOCKS_PER_SEC<<endl;
return 0;
}	
