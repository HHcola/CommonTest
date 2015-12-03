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

void jniSocket::bindSocketToPort(JNIEnv *env, jobject obj, int sd, unsigned short port) {

    struct sockaddr_in address;

    // 清空结构体
    memset(&address, 0, sizeof(address));

    address.sin_family = PF_INET;

    // BIND to all address
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    // convert port to network byte order
    address.sin_port = htons(port);

    LogMessage(env, obj, "Binding to port % d", port);

    // sockaddr方便函数传递，sockaddr_in方便用户设定，所以需要的时候在这2者之间切换
    if (-1 == bind(sd, (struct sockaddr *) & address, sizeof(address))) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }
}

unsigned short jniSocket::getSocketPort(JNIEnv *env, jobject obj, int sd) {

    unsigned short port = 0;
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    if (-1 == getsockname(sd, (struct sockaddr *) &address, &addressLength)) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        port = ntohs(address.sin_port);
        LogMessage(env, obj, "Binding to the random port %d ", port);
    }
    return port;
}

void jniSocket::listenOnSocket(JNIEnv *env, jobject obj, int sd, int backlog) {
    LogMessage(env, obj,
               "Listening on socket with a baklog of  %d pending connections.",
               backlog);

    //listen()用来等待参数s 的socket 连线. 参数backlog 指定同时能处理的最大连接要求,
    //如果连接数目达此上限则client 端将收到ECONNREFUSED 的错误.
    //Listen()并未开始接收连线, 只是设置socket 为listen 模式, 真正接收client 端连线的是accept().
    //通常listen()会在socket(), bind()之后调用, 接着才调用accept().

    if (-1 == listen(sd, backlog)) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    }

}

void jniSocket::LogAddress(JNIEnv *env, jobject obj, const char *message,
                           const struct sockaddr_in *address) {
    char ip[INET_ADDRSTRLEN];

    if (NULL == inet_ntop(PF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN)) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        unsigned short port = ntohs(address->sin_port);
        LogMessage(env, obj, "%s %s:%hu", message, ip, port);
    }
}

int jniSocket::acceptOnSocket(JNIEnv *env, jobject obj, int sd) {

    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    LogMessage(env, obj, "Waiting for a client connection...");
    int clientSocket = accept(sd, (struct sockaddr*) &address, &addressLength);
    if (-1 == clientSocket) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        LogAddress(env, obj, "Client connection from ", &address);
    }
    return clientSocket;
}

ssize_t jniSocket::receiverFromSocket(JNIEnv *env, jobject obj, int sd, char *buffer, size_t bufferSize) {
    LogMessage(env, obj, "Receiving from the socket... ");
    ssize_t recvSize = recv(sd, buffer, bufferSize - 1, 0);

    if (-1 == recvSize) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        //字符串截断
        buffer[recvSize] = NULL;

        if (recvSize > 0) {
            //接收成功，打印
            LogMessage(env, obj, "Received %d bytes:%s", bufferSize, buffer);
        } else {
            LogMessage(env, obj, "Client disconnected.");
        }
    }

    return recvSize;
}

ssize_t jniSocket::sendToSocket(JNIEnv *env, jobject obj, int sd,
                                const char* buffer, size_t bufferSize) {

    LogMessage(env, obj, "Sending to the socket... ");
    ssize_t sentSize = send(sd, buffer, bufferSize, 0);

    if (-1 == sentSize) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        if (sentSize > 0) {
            LogMessage(env, obj, "Send %d bytes: %s", sentSize, buffer);
        } else {
            LogMessage(env, obj, "Client disconnected.");
        }
    }

    return sentSize;
}

void jniSocket::connectToAddress(JNIEnv *env, jobject obj, int sd, const char *ip,
                                 unsigned short port) {
    LogMessage(env, obj, "Connecting to %s:%hu...", ip, port);

    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    //转换ip
    if (0 == inet_aton(ip, &(address.sin_addr))) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        address.sin_port = htons(port);
    }

    if (-1 == connect(sd, (const sockaddr*) &address, sizeof(address))) {
        ThrowErrnoException(env, "java/io/IOException", errno);
    } else {
        LogMessage(env, obj, "Connected.");
    }
}
