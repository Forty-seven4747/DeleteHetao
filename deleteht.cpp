#include<windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>
#include<cstdlib>
#include<io.h>      // _isatty, _fileno
#include<ctype.h>   // tolower
#include<cstdio>    // _fileno
#include "scan.h"
using namespace std;
vector<string> path;
vector<string> drives;
string comm;
bool deny=0;
bool debug=0;
vector<string> getdrives(){
    vector<string> drives;
    DWORD mask=GetLogicalDrives();
    for(char c='A';c<='Z';c++)
        if(mask&(1<<(c-'A')))
            drives.push_back(string(1,c)+":\\");
    return drives;
}
void trydel(string word){
	if(deny)return;
	cout<<"Scaning \""+word+"\"..."<<endl;
	path.clear();
	comm.clear();
	for(size_t i=0;i<drives.size();i++)ScanDisk(drives[i],word,path);
	if(path.empty())return;
    cout<<endl<<"Found "<<path.size()<<" folders"<<endl<<endl;
    for(size_t i=0;i<path.size();i++)cout<<'#'<<i+1<<": "<<path[i]<<endl;
    cout<<endl<<endl<<endl<<"Program will delete the following contents."<<endl<<"Are you sure? (y/n)";
    if(_getwch()!=0x79){deny=47;return;}
    cout<<endl; 
	for(size_t i=0;i<path.size();i++){
        cout<<"Deleting \""<<path[i]<<"\""<<endl;
        comm="rd /s /q \""+path[i]+"\"";
        system(comm.c_str());
    }
    cout<<"Done"<<endl;
	deny=47;
}
int main(int argc,char** argv){
	if(argc>1){
        drives.clear();
        for(int i=1;i<argc;i++) drives.push_back(argv[i]);
    }else drives=getdrives();
    cout<<"Ready for scanning"<<endl;
    if(_isatty(_fileno(stdin))){
        const DWORD total=6000; DWORD start=GetTickCount();
        while(GetTickCount()-start < total){
            cout<<"\rTo show scanned files, press any key within "
                << (int)((total-(GetTickCount()-start))/1000) <<" seconds...";
            if(_kbhit()){ _getch(); debug=47; cout<<endl; break; }
            Sleep(100);
        }
        cout<<endl;
    }
    trydel("hetao");
    trydel("hetaotProgram");
    trydel("creatingPlatform");
}
