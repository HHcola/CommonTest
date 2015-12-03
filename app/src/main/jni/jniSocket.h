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

    // bind socket
    void bindSocketToPort(JNIEnv * env, jobject obj, int sd, unsigned short port);

    // 获取当前socket的端口
    unsigned short getSocketPort(JNIEnv * env, jobject obj, int sd);

    // 监听
    void listenOnSocket(JNIEnv * env, jobject obj, int sd, int backlog);

    // 根据地址打印ip和端口
    void LogAddress(JNIEnv * env, jobject obj, const char * message, const struct sockaddr_in * address);

    // accept()
    int acceptOnSocket(JNIEnv * env, jobject obj, int sd);

    // 接受 recv()
    ssize_t receiverFromSocket(JNIEnv * env, jobject obj, int sd, char * buffer, size_t bufferSize);

    // 发送消息 send()
    ssize_t sendToSocket(JNIEnv *env, jobject obj, int sd,
                         const char* buffer, size_t bufferSize);

    // 链接服务器 connect()
    void connectToAddress(JNIEnv * env, jobject obj, int sd, const char * ip, unsigned short port);

private:
    string mRequestUrl;
    string mSavePath;
    int    mSocketID;





};
#endif //COMMONTEST_JNISOCKET_H
