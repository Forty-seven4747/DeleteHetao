#include "scan.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
using namespace std;
extern bool debug;
bool isSkipped(const string& name){
    static const char* skip[] = {
        "Windows","Program Files","Program Files (x86)","ProgramData",
        "$Recycle.Bin","System Volume Information","Recovery","AppData",
        "PerfLogs","PCL","Steam","SteamLibrary",NULL
    };
    for(int i=0; skip[i]; i++)
        if(_stricmp(name.c_str(), skip[i])==0) return true;
    return false;
}
void ScanDisk(const string& rootPath,const string& keyword,vector<string>& list,short depth){
    if(depth>=8)return;
    string searchPath=rootPath+"\\*";
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fd);
    if(hFind==INVALID_HANDLE_VALUE) return;
    do{
        if(strcmp(fd.cFileName, ".")==0)continue;
        if(strcmp(fd.cFileName, "..")==0)continue;
        string fullPath=rootPath+ "\\"+fd.cFileName;
        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            if(debug)cout<<fullPath<<endl;
            if(_stricmp(fd.cFileName, keyword.c_str())==0)list.push_back(fullPath);
            if(!isSkipped(fd.cFileName))
                ScanDisk(fullPath,keyword,list,depth+1);
        }
    }while (FindNextFileA(hFind, &fd));
    FindClose(hFind);
}
