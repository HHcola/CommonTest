//
// Created by hewei05 on 15/12/1.
//

#include "com_example_test_commontest_jniDemo_JniDemoActivity.h"
#include "file.h"
#include "jniSocket.h"
#include "jniUtils.h"
#include <android/log.h>


JNIEXPORT jstring JNICALL Java_com_example_test_commontest_jniDemo_JniDemoActivity_getStringFromJni
(JNIEnv * env, jobject obj)
{
    return env->NewStringUTF("This just a test for Android Studio NDK JNI developer!");
}

JNIEXPORT jstring JNICALL
Java_com_example_test_commontest_jniDemo_JniDemoActivity_createFile(JNIEnv *env, jobject instance,
                                                                    jstring fileName_) {
    const char *fileName = env->GetStringUTFChars(fileName_, 0);

    LogMessage(env, instance, "file name = %s ", fileName);
    JniFile jniFile(fileName);
    jniFile.createFile();

    env->ReleaseStringUTFChars(fileName_, fileName);

    return env->NewStringUTF("create file success");
}

JNIEXPORT void JNICALL
Java_com_example_test_commontest_jniDemo_JniDemoActivity_nativeStartTcpServer(JNIEnv *env,
                                                                              jobject instance,
                                                                              jstring host,
                                                                              jstring path,
                                                                              jint port,
                                                                              jstring jstrMsg,
                                                                              jobjectArray message) {


    const char * httpHost = env->GetStringUTFChars(host, NULL);
    const char * httpPath = env->GetStringUTFChars(path, NULL);
    HttpGet(httpHost, httpPath);

//    LogMessage(env, instance, "nagtiveStartTcpServer port");
//    jniSocket jniSocket;
//    int clientSocket = jniSocket.createTcpSocket(env, instance);
//
//    if (NULL == env->ExceptionOccurred()) {
//
//        const char * socketHost = env->GetStringUTFChars(host, NULL);
//        jniSocket.connectToAddress(env, instance, clientSocket, socketHost,(unsigned short )port);
//        string strHost(socketHost);
//        LogMessage(env, instance, "release host %s ", socketHost);
////        env->ReleaseStringUTFChars(host, socketHost);
//
//        // check connect exception
////        if (NULL != env->ExceptionOccurred()) {
////            goto exit;
////        }
//
////        const char * strMsgRequest = env->GetStringUTFChars(jstrMsg, NULL);
////        LogMessage(env, instance, "GetMsg is %s", strMsgRequest);
////        env->ReleaseStringUTFChars(jstrMsg, strMsgRequest);
//
//        LogMessage(env, instance, "Send msg to service begin");
//
////        string strMsg ;
////        setRequest(env, instance, message, strMsg);
////        LogMessage(env, instance, "reqeust msg %s",strMsg.c_str());
//
//        string requestMsg;
//        string CRLF = "\r\n";
//
//
//        const char * socketPath = env->GetStringUTFChars(path, NULL);
//        LogMessage(env, instance, "path is %s ", socketPath);
//        string strPath(socketPath);
//
//
//        requestMsg.append("GET " + strPath + " HTTP/1.0" + CRLF);
//        requestMsg.append("Accept: */*" + CRLF);
//        requestMsg.append("Host: " + strHost + CRLF);
//        requestMsg.append("Content-Type: application/octet-stream" + CRLF);
////        LogMessage(env, instance, "requestMsg msg %s",requestMsg.c_str());
//
//        char buf[2048];
//        memset(buf, 0 , 2048);
//        sprintf(buf,
//                "GET %s HTTP/1.0\r\n"
//                "Accept:*/*\r\n"
//                "Host: %s\r\n\r\n"
//                ,socketPath, socketHost
//        );
//
////        LogMessage(env, instance, "requestMsg msg %s", buf);
//        jniSocket.sendToSocket(env, instance, clientSocket, buf, strlen(buf));
////        if (NULL != env->ExceptionOccurred()) {
////            goto exit;
////        }
//
//        env->ReleaseStringUTFChars(path, socketPath);
//
//
//        char buffer[MAX_BUFFER_SIZE];
//        ssize_t receiverSize;
//        do {
//            receiverSize = jniSocket.receiverFromSocket(env, instance, clientSocket, buffer, MAX_BUFFER_SIZE);
//
//            // 调用java层
//            if (receiverSize != -1) {
//                Java_com_example_test_commontest_jniDemo_JniDemoActivity_CallByJni(env, instance, buffer, receiverSize);
//            }
//        } while (receiverSize != -1);
//    }
//
//
//    // 关闭socket
//    exit:
//        if (clientSocket > -1) {
//            LogMessage(env, instance, "nativeStartTcpServer error");
//            close(clientSocket);
//        }
}


void JNICALL Java_com_example_test_commontest_jniDemo_JniDemoActivity_CallByJni(JNIEnv *env,
                                                                                jobject obj,
                                                                                char * buffer, int length) {
    // find class
    jclass cls = env->FindClass("com/example/test/commontest/jniDemo/JniDemoActivity");

    // find method
    jmethodID mid = env->GetMethodID(cls, "callBackFromJni", "([B)V");

    if (mid == NULL) {
        LogMessage(env, obj, "method is null callBackFromJni");
        return;
    }

    // 获取数组长度
    env->CallVoidMethod(obj, mid, buffer, length);
}


/**
 * 设置请求头
 */
void setRequest(JNIEnv *env, jobject obj, jobjectArray messageObj, std::string message) {

    jstring jstr;
    jsize  len = env->GetArrayLength(messageObj);

    LogMessage(env, obj, "setRequest msg len %d ", len);
    int i = 0;
    for(; i < len; i ++) {
        jstr = (jstring) env->GetObjectArrayElement(messageObj, i);
        message += env->GetStringUTFChars(jstr, NULL);
    }

}



int GetHttpResponseHead(int sock,char *buf,int size)
{
    int i;
    char *code,*status;
    memset(buf,0,size);
    for(i=0; i<size-1; i++){
        if(recv(sock,buf+i,1,0)!=1){
            perror("recv error");
            return -1;
        }
        if(strstr(buf,"\r\n\r\n"))
            break;
    }
    if(i >= size-1){
        puts("Http response head too long.");
        return -2;
    }
    code=strstr(buf," 200 ");
    status=strstr(buf,"\r\n");
    if(!code || code>status){
        *status=0;
        printf("Bad http response:\"%s\"\n",buf);
        return -3;
    }
    return i;
}

int HttpGet(const char *server,const char *url)
{
    __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "Enter HttpGet function!");
    int sock=socket(PF_INET,SOCK_STREAM,0);
    __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "%d",sock);
    struct sockaddr_in peerAddr;
    char buf[2048];
    int ret;


    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    //转换ip
    if (0 == inet_aton(server, &(address.sin_addr))) {
        __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "inet_aton error!");
    }

    // set port
    address.sin_port = 80;

    ret == connect(sock, (const sockaddr*) &address, sizeof(address));
    if(ret != 0){
        __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "connect failed!");
        close(sock);
        return -1;
    }
    sprintf(buf,
            "GET %s HTTP/1.0\r\n"
                    "Accept: */*\r\n"
                    "Host: %s\r\n"
                    "Connection: Close\r\n\r\n",
            url,server);
    send(sock,buf,strlen(buf),0);
    if(GetHttpResponseHead(sock,buf,sizeof(buf))<1){
        close(sock);
        __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "GetHttpResponse error!");
        return -1;
    }

    __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "Enter HttpGet function's while!");
    while((ret=recv(sock,buf,sizeof(buf),0)) > 0)
    {
        //fwrite(buf,ret,1,fp);
        __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "hello, this is my number %s log message!", buf);
    }
    //}
    //fclose(fp);
    shutdown(sock,SHUT_RDWR);
    close(sock);
    return 0;
}

//"http://192.168.7.222/index.html"
int mmain(void)
{
    __android_log_print(ANDROID_LOG_INFO, "-----from--jni-------", "Enter mmain function!");
    char *head,*tail;
    char server[128]={0};

    head=strstr("http://220.181.111.148/index.html","//");
    if(!head){
        puts("Bad url format");
        return -1;
    }
    head+=2;
    tail=strchr(head,'/');
    if(!tail){
        return HttpGet(head,"/");
    }else if(tail-head>sizeof(server)-1){
        puts("Bad url format");
        return -1;
    }else{
        memcpy(server,head,tail-head);
        return HttpGet(server,tail);
    }
}
