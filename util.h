#ifndef _UTIL_H_
#define _UTIL_H_
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

vector<string> listFastaFiles(string dir_path);
void combineFastaFile(string dir_path, vector<string> files);



/**
 * 显示帮助信息
 */
void displayUsage();


#endif
