//
// Created by hewei05 on 15/12/1.
//

#ifndef COMMONTEST_JNISOCKET_H
#define COMMONTEST_JNISOCKET_H

#include <string>
// socket header
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
// htons, sockaddr_in
#include <netinet/in.h>
// inet_ntop
#include <arpa/inet.h>
// close, unlink
#include <unistd.h>
// offsetof
#include <stddef.h>
#include <jni.h>


using namespace std;
class jniSocket {

public:
    jniSocket();
    ~jniSocket();

    void setRequestUrl(string requestUrl);
    void setSavePath(string savePath);
    int  createTcpSocket(JNIEnv * env, jobject obj);

private:
    string mRequestUrl;
    string mSavePath;
    int    mSocketID;





};
#endif //COMMONTEST_JNISOCKET_H
