//
// Created by hewei05 on 15/12/1.
//

#include "jniSocket.h"
#include "jniUtils.h"

jniSocket::jniSocket() {

}

jniSocket::~jniSocket() {

}

void jniSocket::setRequestUrl(string requestUrl) {
    mRequestUrl = requestUrl;
}

void jniSocket::setSavePath(string savePath) {
    mSavePath = savePath;
}


int jniSocket::createTcpSocket(JNIEnv *env, jobject obj) {

    LogMessage(env, obj, "Create new TCP socket ...");

    mSocketID = socket(PF_INET, SOCK_STREAM, 0);

    if (-1 == mSocketID) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }

    return mSocketID;
}
