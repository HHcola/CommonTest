//
// Created by hewei05 on 15/12/1.
//

#ifndef COMMONTEST_JNIUTILS_H
#define COMMONTEST_JNIUTILS_H

#include <jni.h>

#define MAX_LOG_MESSAGE_LENGTH 256
#define MAX_BUFFER_SIZE = 80


static void LogMessage(JNIEnv *env, jobject obj, const char * format, ...) {

    static jmethodID methodID = NULL;
    jclass clazz = env->GetObjectClass(obj);
    methodID = env->GetMethodID(clazz, "logMessage", "(Ljava/lang/String;)V");
    env->DeleteLocalRef(clazz);

    if (methodID != NULL) {
        char buffer[MAX_BUFFER_SIZE];

        // 将可变参数输出到字符数组中
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
        va_end(ap);

        // 转换成java字符串
        jstring message = env->NewStringUTF(buffer);
        if (message != NULL) {
            env->CallVoidMethod(obj, methodID, message);
            env->DeleteLocalRef(message);
        }
    }

}


static void ThrowException(JNIEnv *env, const char *className, const char *message) {
    jclass clazz = env->FindClass(className);
    if (clazz != NULL) {
        env->ThrowNew(clazz, message);
        env->DeleteLocalRef(clazz);
    }
}

static void ThrowErrnoException(JNIEnv * evn, const char * className, int errnum) {
    char buffer[MAX_BUFFER_SIZE];

    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH)) {
        strerror_r(errno, buffer, MAX_LOG_MESSAGE_LENGTH);
    }

    ThrowException(env, className, buffer);
}

#endif //COMMONTEST_JNIUTILS_H
