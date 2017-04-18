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

	double m1,m2;
	time_t t1,t2;
	t1=clock();
	string dir_path = "./data/x90/";
	vector<string> files = listFastaFiles(dir_path);
	combineFastaFile(dir_path, files);
	t2=clock();
	cout<<t2-t1<<endl;
	
	t1=clock();
	// 1. Call CUDA ClustalW to generate a guide tree for the combined fasta file
	// start = omp_get_wtime();
	   system("./cuda_clustalw2 -QUIET -INFILE=./tmp.fa -NEWTREE=./combined.dnd");
//	system("./clustalw2 -QUIET -INFILE=./tmp.fa -NEWTREE=./combined.dnd");  
	t2 = clock();
	m1=(double)(t2-t1)/CLOCKS_PER_SEC;
	cout<<"time 1and 2:"<<(double)(t2-t1)/CLOCKS_PER_SEC<<endl;  
	//end = omp_get_wtime();
	//  printf("time step 1(cuda): %f\n", end-start);

	// 2. Call ClustalW to do the rest steps of MSA
	//  start = omp_get_wtime();
	t1=clock();
	//    #pragma omp parallel for
	for(int i = 0; i < files.size(); i++) {
		string cmd = "./clustalw2 -ALIGN -QUIET -USETREE=./combined.dnd -INFILE="+dir_path +files[i];
		system(cmd.c_str());
	}
	t2=clock();
	m2=(double)(t2-t1)/CLOCKS_PER_SEC;
	cout<<"time 3:"<<(double)(t2-t1)/CLOCKS_PER_SEC<<endl;
	cout<<"total time:"<<m1+m2<<endl;
	// end = omp_get_wtime();
	// printf("time step 3(openmp): %f\n", end-start);

	return 0;
}
