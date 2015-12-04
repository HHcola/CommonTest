# include <stdio.h>
# include <stdlib.h>
# include "./headers/libinetsocket.h"
# include <unistd.h>
# include <string.h>
#include <jni.h>
#ifndef _Included_com_example_test_commontest_jniDemo_HTTP
#define _Included_com_example_test_commontest_jniDemo_HTTP
#ifdef __cplusplus
extern "C" {
#endif
/*
 * This example is part of libsocket/libinetsocket
 * It may be used with the most HTTP servers.
 *
 * This is a almost complete HTTP client, simply
 * writing the data to stdout. There's no more code
 * with libsocket!
 */

int SocketTest(JNIEnv * env, jobject obj);

#ifdef __cplusplus
}
#endif
#endif
