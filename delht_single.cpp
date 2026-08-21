#include<windows.h>
#include<iostream>
#include<string>
#include<vector>
#include<conio.h>
#include<cstdlib>
#include<io.h>      // _isatty, _fileno
#include<ctype.h>   // tolower
#include<cstdio>    // _fileno
using namespace std;
vector<string> path;
vector<string> drives;
string comm;
bool deny=0;
bool debug=0;
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
vector<string> getdrives(){
    vector<string> drives;
    DWORD mask=GetLogicalDrives();
    for(char c='A';c<='Z';c++)
        if(mask&(1<<(c-'A')))
            drives.push_back(string(1,c)+":\\");
    return drives;
}
void ScanDisk(const string& rootPath,const string& keyword,vector<string>& list,short depth=0){
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
    }else{
        drives=getdrives();
    }
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
    trydel("hetaoProgram"); 
    trydel("creatingPlatform");
}
