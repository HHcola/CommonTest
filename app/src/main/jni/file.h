//
// Created by hewei05 on 15/12/1.
//

#ifndef COMMONTEST_FILE_H
#define COMMONTEST_FILE_H

#include <string>

class JniFile
{
private:
    std::string mFileName;
public:
    JniFile(std::string fileName);
    void setFileName(std::string fileName);
    void createFile();
};
#endif //COMMONTEST_FILE_H
