//
// Created by hewei05 on 15/12/1.
//

#include "com_example_test_commontest_jniDemo_JniDemoActivity.h"
#include "file.h"
#include "jniSocket.h"
#include "jniUtils.h"
#include <android/log.h>

#define ENABLE_DEBUG 1

#if ENABLE_DEBUG
#define  LOG_TAG "JNI-DEMO"
#define  LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, fmt, ##args)
#define DEBUG_PRINT(format,args...) \
    LOGD(format, ##args)
#else
#define DEBUG_PRINT(format,args...)
#endif


JNIEXPORT jstring JNICALL Java_com_example_test_commontest_jniDemo_JniDemoActivity_getStringFromJni
(JNIEnv * env, jobject obj)
{
    return env->NewStringUTF("This just a test for Android Studio NDK JNI developer!");
}

JNIEXPORT jstring JNICALL
Java_com_example_test_commontest_jniDemo_JniDemoActivity_createFile(JNIEnv *env, jobject instance,
                                                                    jstring fileName_) {
    const char *fileName = env->GetStringUTFChars(fileName_, 0);

    LOGD("file name = %s ", fileName);
    JniFile jniFile(fileName);
    jniFile.createFile();

    env->ReleaseStringUTFChars(fileName_, fileName);

    return env->NewStringUTF("create file success");
}

JNIEXPORT void JNICALL
Java_com_example_test_commontest_jniDemo_JniDemoActivity_nativeStartTcpServer(JNIEnv *env,
                                                                              jobject instance,
                                                                              jstring ip,
                                                                              jint port,
                                                                              jstring message) {
    jniSocket jniSocket;
    int clientSocket = jniSocket.createTcpSocket(env, instance);

    if (NULL == env->ExceptionOccurred()) {
        const char * ipAddress = env->GetStringUTFChars(ip, NULL);

        if (NULL == ipAddress) {
            goto exit;
        }

        jniSocket.connectToAddress(env, instance, clientSocket, ipAddress, (unsigned short) port);

        // 释放ip
        env->ReleaseStringUTFChars(ip, ipAddress);

        // check connect exception
        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        // send message to server
        const char * messageText = env->GetStringUTFChars(message, NULL);

        //
        jsize messageSize = env->GetStringLength(message);
        jniSocket.sendToSocket(env, instance, clientSocket, messageText, messageSize);

        env->ReleaseStringUTFChars(message, messageText);

        if (NULL != env->ExceptionOccurred()) {
            goto exit;
        }

        char buffer[MAX_BUFFER_SIZE];

        jniSocket.receiverFromSocket(env, instance, clientSocket, buffer, MAX_BUFFER_SIZE);

    }


    // 关闭socket
    exit:
        if (clientSocket > -1) {
            close(clientSocket);
        }

}