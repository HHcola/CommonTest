#include "http.h"
#include <android/log.h>
#define  LOG_TAG "JNI-DEMO"
#define  LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, fmt, ##args)
/*
 * This example is part of libsocket/libinetsocket
 * It may be used with the most HTTP servers.
 *
 * This is a almost complete HTTP client, simply
 * writing the data to stdout. There's no more code
 * with libsocket!
 */

int SocketTest(JNIEnv *env, jobject obj) {
    int sfd, bytes, ret;
    char request[128], buf[32];

    buf[31] = 0;

    LOGD("create_inet_stream_socket");

    ret = sfd = create_inet_stream_socket("lbo.spheniscida.de", "80", LIBSOCKET_IPv4, 0);

    if (ret < 0) {
        perror(0);
        exit(1);
    }

    sprintf(request, "GET / HTTP/1.1\nHost: lbo.spheniscida.de\n\n");

    ret = write(sfd, request, strlen(request));

    if (ret < 0) {
        LOGD("write failed");
        perror(0);
        exit(1);
    }

    LOGD("write success");
    ret = shutdown_inet_stream_socket(sfd, LIBSOCKET_WRITE);

    if (ret < 0) {
        perror(0);
        exit(1);
    }

    while (0 < (bytes = read(sfd, buf, 31))) {
        write(1, buf, bytes);
        LOGD("read success");
    }


    ret = destroy_inet_socket(sfd);

    LOGD("close");
    if (ret < 0) {
        perror(0);
        exit(1);
    }

    return 0;
}

