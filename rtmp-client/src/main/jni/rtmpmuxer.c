#include <jni.h>
#include "flvmuxer/xiecc_rtmp.h"
#include "librtmp/log.h"


/**
 * if it returns bigger than 0 it is successfull
 */
JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_open(JNIEnv *env, jobject thiz, jstring url_) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp != NULL) {
        return -50;
    }
    const char *url = (*env)->GetStringUTFChars(env, url_, 0);

    int result = rtmp_open_for_write(url, &rtmp);

    (*env)->ReleaseStringUTFChars(env, url_, url);
    raw_rtmp = (jlong)rtmp;
    (*env)->SetLongField(env, thiz, fid, raw_rtmp);
    return result;
}


JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_writeAudio(JNIEnv *env, jobject thiz,
                                                       jbyteArray data_, jint offset, jint length,
                                                       jint timestamp) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    jbyte *data = (*env)->GetByteArrayElements(env, data_, NULL);

    jint result = rtmp_sender_write_audio_frame(rtmp, data, length, timestamp, 0);

    (*env)->ReleaseByteArrayElements(env, data_, data, 0);
    return result;
}

JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_writeVideo(JNIEnv *env, jobject thiz,
                                                       jbyteArray data_, jint offset, jint length,
                                                       jint timestamp) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    jbyte *data = (*env)->GetByteArrayElements(env, data_, NULL);

    jint result = rtmp_sender_write_video_frame(rtmp, data, length, timestamp, 0, 0);

    (*env)->ReleaseByteArrayElements(env, data_, data, 0);

    return result;
}

JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_close(JNIEnv *env, jobject thiz) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    rtmp_close(rtmp);
    raw_rtmp = 0;
    (*env)->SetLongField(env, thiz, fid, raw_rtmp);
    return 0;

}

JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_setChunkSize(JNIEnv *env, jobject thiz, jint size) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    return rtmp_sender_set_chunk_size(rtmp, size);
}

JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_isConnected(JNIEnv *env, jobject thiz) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    return rtmp_is_connected(rtmp);
}

JNIEXPORT void JNICALL
Java_net_butterflytv_rtmp_1client_RTMPMuxer_setLogLevel(JNIEnv *env, jclass clazz, jint level) {
    RTMP_debuglevel = level;
    return;
}
