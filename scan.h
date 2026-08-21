#ifndef SCAN_H
#define SCAN_H
#include<string>
#include<vector>
void ScanDisk(const std::string& rootPath, const std::string& keyword,std::vector<std::string>& list, short depth = 0);
#endif
