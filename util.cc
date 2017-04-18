#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include "util.h"
using namespace std;


inline bool ends_with(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

vector<string> listFastaFiles(string dir_path) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path.c_str());
    if (dp == NULL) {
        perror("opendir: Path does not exist or could not be read.");
        exit(-1);
    }

    vector<string> fasta_files;

    while ((entry = readdir(dp))) {
        if(ends_with(entry->d_name, "fa") || ends_with(entry->d_name, "fasta"))
            fasta_files.push_back(entry->d_name);
    }
    closedir(dp);
    return fasta_files;
}



void readFastaFile(string path, map<string, string> *seqs_ptr) {
    //map<string, string> seqs = *seqs_ptr;

    string buff, line, title;

    ifstream file;
    file.open(path.c_str());
    assert(file);

    while(getline(file, buff)) {
        if(buff.empty() || buff[0] == '>') {
            if(!line.empty()) {
                (*seqs_ptr)[title] = line;
                //seqs.push_back(line);
                //titles.push_back(title);
            }
            if(buff[0] == '>')
                title = buff;
            line = "";
            continue;
        } else {
            line += buff;
        }
    }

    if(!line.empty() && !title.empty()) // last sequence
        (*seqs_ptr)[title] = line;

    file.close();
}

void writeFastaFile(const char* path, map<string,string> seqs) {
    ofstream file(path);
    if(file.is_open()) {
        map<string, string>::iterator it;
        for(it = seqs.begin(); it != seqs.end(); ++it) {
            string title = it->first;
            string content = it->second;
            file<<title<<endl;
            int lines = content.size() / 60;        // 60个字符一行
            lines = content.size() % 60 == 0 ? lines : lines+1;
            for(int k = 0; k < lines; k++)
                file<<content.substr(k*60, 60)<<endl;
        }
    }
    file.close();
}


void combineFastaFile(string dir_path, vector<string> files) {
    cout<<"combne file, size: "<<files.size()<<endl;
    map<string, string> seqs;
    for(int i = 0; i < files.size(); i++) {
        readFastaFile(dir_path+files[i], &seqs);
        cout<<files[i]<<" seqs size:"<<seqs.size()<<endl;
    }

    writeFastaFile("./tmp.fa", seqs);
}
