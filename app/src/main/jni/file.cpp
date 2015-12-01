//
// Created by hewei05 on 15/12/1.
//

#include "file.h"
#include <stdio.h>

JniFile::JniFile(std::string fileName) {
    mFileName = fileName;
}

void JniFile::createFile() {
    if (mFileName.empty()) {
        return;
    }

    FILE * file = fopen(mFileName.c_str(), "w+");

    if (file != NULL) {
        fputs("for test", file);
        fflush(file);
        fclose(file);
    }
    return;
}
