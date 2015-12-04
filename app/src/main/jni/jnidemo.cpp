//
// Created by hewei05 on 15/12/1.
//

#include "com_example_test_commontest_jniDemo_JniDemoActivity.h"
#include "file.h"
#include "jniUtils.h"
#include "http.h"


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


//    const char * httpHost = env->GetStringUTFChars(host, NULL);
//    const char * httpPath = env->GetStringUTFChars(path, NULL);
//    HttpGet(httpHost, httpPath);
    SocketTest(env, instance);

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

