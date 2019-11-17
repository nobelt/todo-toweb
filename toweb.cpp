/* 
 * The following script reformats Leasot TODO list (.txt)
 * into .html with style encoding and links to referenced
 * line of code in github. 
 * 
 * Command line arguments: -i [source file] -o [output file] 
 *     -d [github destination](optional) -h (help, optional)
 * 
 * Nobel Truong 2019
 *
 */

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <chrono>
#include <ctime>
#include <cstring>

using namespace std;

string timeStamp();
void genHTML(string inArg, string outArg, string dest, string root);
void prtUsage();

int main(int argc, char **argv)
{
    string inArg, outArg, dest, root;
    
    int option;
    
    while (1){
        option = getopt(argc, argv, ":i:o:d:r:h");
        if (option == -1)
            break;

        switch (option) {
        case 'i':
            if (optarg) inArg = optarg;
            break;
        case 'o':
            if (optarg) outArg = optarg;
            break;
        case 'd':
            if (optarg) dest = optarg;
            break;
        case 'r':
            if (optarg) root = optarg;
            break;
        case 'h':
            prtUsage();
            break;
        case '?':
            fprintf(stderr, "Error: Erroneous option character passed.");
            prtUsage();
            exit(1);
        case ':':
            fprintf(stderr, "Error: Argument missing for option passed.");
            prtUsage();
            exit(1);
        default:
            fprintf(stderr, "Error: Error encountered when command line arguments");
            prtUsage();
            exit(1);
        }
            
    }

    genHTML(inArg, outArg, dest, root);
}

string timeStamp(){
    auto start = chrono::system_clock::now();
    auto end = chrono::system_clock::now();
    time_t end_time = chrono::system_clock::to_time_t(end);
    return ctime(&end_time);
}
    
void genHTML(string inArg, string outArg, string dest, string root){
    ifstream inFile(inArg);
    ofstream outFile(outArg, ios::out | ios::trunc);
                
    string link;
    string line;

    outFile <<"<link href=\"style.css\" rel=\"stylesheet\" type=\"text/css\" />"; 
    outFile << "<table><col width=\"80\"><col width=\"70\">";
    outFile << "<tr><td colspan=\"3\">Last updated at " << timeStamp() << "</td></tr>";
    outFile << "<tr><td colspan=\"3\"></td></tr>";
        
    while (getline(inFile, line)){
        
        if ((line.find("todos/fixmes") != string::npos) || (line.find("todo/fixme") != string::npos)){
            outFile << "<tr><td colspan=\"3\"><font color=red><b>" << line << "</font></b></td></tr><tr><td colspan=\"3\"></td></tr>";
        }
        else if ((line.find("line") != string::npos) && (line.find("TODO") != string::npos)){
            
            size_t pos1 = line.find("line");
            size_t pos2 = line.find("TODO");
            if (pos1 != string::npos){
            
                string num = line.substr(pos1+5,pos2-9);
                string res = line.substr(pos2+6, line.length());

                outFile << "<tr><td> <a href=\"" << dest << "/blob/master" << link << "#L" << num << "\" target=\"_blank\" ><b>line " << num << "</a></b></td>";
                    outFile << "<td>TODO</td>";
                    outFile << "<td>" << res << "</td></tr>";
            }
        }
        else {
            size_t linkPos = line.find(root);
            
            if (linkPos != string::npos){
                link = line.substr(linkPos+ root.length(), line.length());                      outFile << "<tr><td colspan=\"3\">" << link <<"</td></tr>" ;
            }
            else {
                outFile << "<tr><td colspan=\"3\">" << line <<"</td></tr>" ;
            }
        }

    }

    outFile << "</table>";
               
    inFile.close();
    outFile.close();
}

 void prtUsage(){
     fprintf(stderr, "Usage: [executable] -i [path-to-input-file] -o [path-to-output-file] -d [github root link] -r [root path]");
 }
