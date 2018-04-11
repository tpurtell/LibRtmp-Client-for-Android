#include <malloc.h>
#include "librtmp-jni.h"
#include "rtmp.h"
//
// Created by faraklit on 01.01.2016.
//

/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    open
 * Signature: (Ljava/lang/String;Z)I
 */
JNIEXPORT jint JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_open
        (JNIEnv * env, jobject thiz, jstring url_, jboolean isPublishMode) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp != NULL) {
        return -5;
    }

    const char *url = (*env)->GetStringUTFChars(env, url_, 0);
    rtmp = RTMP_Alloc();
    if (rtmp == NULL) {
        return -1;
    }

	RTMP_Init(rtmp);
	int ret = RTMP_SetupURL(rtmp, url);

    if (!ret) {
        RTMP_Free(rtmp);
        return -2;
    }
    if (isPublishMode) {
        RTMP_EnableWrite(rtmp);
    }

	ret = RTMP_Connect(rtmp, NULL);
    if (!ret) {
        RTMP_Free(rtmp);
        return -3;
    }
	ret = RTMP_ConnectStream(rtmp, 0);

    if (!ret) {
        return -4;
    }
    (*env)->ReleaseStringUTFChars(env, url_, url);
    raw_rtmp = (jlong)rtmp;
    (*env)->SetLongField(env, thiz, fid, raw_rtmp);
    return 1;
}



/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    read
 * Signature: ([CI)I
 */
JNIEXPORT jint JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_read
        (JNIEnv * env, jobject thiz, jbyteArray data_, jint offset, jint size) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    char* data = malloc(size*sizeof(char));

    int readCount = RTMP_Read(rtmp, data, size);

    if (readCount > 0) {
        (*env)->SetByteArrayRegion(env, data_, offset, readCount, data);  // copy
    }
    free(data);

 	return readCount;
}

/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    write
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_write
        (JNIEnv * env, jobject thiz, jbyteArray data, jint size) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }
    jboolean isCopy;
    jbyte *elements = (*env)->GetByteArrayElements(env, data, &isCopy);

    jint result = -1;
    if (elements) {
        result = RTMP_Write(rtmp, elements, size);
        (*env)->ReleaseByteArrayElements(env, data, elements, JNI_ABORT);
    }
    return result;
}

/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    seek
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_seek
        (JNIEnv * env, jobject thiz, jint seekTime) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    return 0;
}

/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    pause
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_pause
        (JNIEnv * env, jobject thiz, jint pauseTime) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

    return RTMP_Pause(rtmp, pauseTime);
}

/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_close
        (JNIEnv * env, jobject thiz) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

	RTMP_Close(rtmp);
	RTMP_Free(rtmp);
    raw_rtmp = 0;
    (*env)->SetLongField(env, thiz, fid, raw_rtmp);
    return 0;
}


JNIEXPORT jint JNICALL
Java_net_butterflytv_rtmp_1client_RtmpClient_isConnected(JNIEnv *env, jobject thiz) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);

    if(rtmp == NULL) {
        return -10000;
    }

     int connected = RTMP_IsConnected(rtmp);
     if (connected) {
        return 1;
     }
     else {
        return 0;
     }
}

/*
 * Class:     net_butterflytv_rtmp_client_RtmpClient
 * Method:    serverIP
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_butterflytv_rtmp_1client_RtmpClient_serverIP
        (JNIEnv * env, jobject thiz) {
    jfieldID fid = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, thiz), "rtmp", "J");
    jlong raw_rtmp =  (*env)->GetLongField(env, thiz, fid);
    RTMP *rtmp = (RTMP*)(*(void**)&raw_rtmp);
    if (rtmp == NULL) {
        return NULL;
    }
    return (*env)->NewStringUTF(env, RTMP_ServerIP(rtmp));
}
