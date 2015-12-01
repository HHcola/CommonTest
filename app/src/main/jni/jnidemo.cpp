//
// Created by hewei05 on 15/12/1.
//

#include "com_example_test_commontest_jniDemo_JniDemoActivity.h"
#include "file.h"
#include "jniSocket.h"
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
                                                                              jint port) {

    jniSocket jniSocket;
    jniSocket.createTcpSocket(env, instance);

}