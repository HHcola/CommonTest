//
// Created by hewei05 on 15/12/1.
//

#ifndef COMMONTEST_JNIUTILS_H
#define COMMONTEST_JNIUTILS_H

#include <jni.h>
#include <android/log.h>
#include <errno.h>

#define MAX_LOG_MESSAGE_LENGTH 256
#define MAX_BUFFER_SIZE  80

#define  LOG_TAG "JNI-DEMO"
#define  LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, fmt, ##args)

static void LogMessage(JNIEnv *env, jobject obj, const char * format, ...) {

    char buffer[MAX_BUFFER_SIZE];

    // 将可变参数输出到字符数组中
    va_list ap;
    va_start(ap, format);
    vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
    va_end(ap);

    LOGD(format, buffer);

}


static void ThrowException(JNIEnv *env, const char *className, const char *message) {
    jclass clazz = env->FindClass(className);
    if (clazz != NULL) {
        env->ThrowNew(clazz, message);
        env->DeleteLocalRef(clazz);
    }
}

static void ThrowErrnoException(JNIEnv * env, const char * className, int errnum) {
    char buffer[MAX_BUFFER_SIZE];

    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH)) {
        strerror_r(errno, buffer, MAX_LOG_MESSAGE_LENGTH);
    }

    ThrowException(env, className, buffer);
}

#endif //COMMONTEST_JNIUTILS_H
