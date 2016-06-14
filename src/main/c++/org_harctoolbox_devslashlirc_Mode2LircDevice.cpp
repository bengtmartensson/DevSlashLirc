#include "org_harctoolbox_devslashlirc_Mode2LircDevice.h"
#include "Mode2LircDevice.h"
#include <iostream>

// private helper function
static Mode2LircDevice *getMode2LircDevice(JNIEnv *env, jobject object) {
    jclass clazz = env->GetObjectClass(object);
    jfieldID fidNumber = env->GetFieldID(clazz, "nativePointer", "J");
    if (fidNumber == NULL) {
        std::cerr << "Error: could not get fidNumber" << std::endl;
        return NULL;
    }
    return (Mode2LircDevice*) env->GetLongField(object, fidNumber);
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    newMode2LircDevice
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_newMode2LircDevice
(JNIEnv *env, jclass, jstring deviceName) {
    const char *device_name = env->GetStringUTFChars(deviceName, NULL);
    if (device_name == NULL)
        return 0L;

    Mode2LircDevice *p = new Mode2LircDevice(device_name);
    //std::cerr << (long) p;
    return (long) p;
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    canSetSendCarrier
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_canSetSendCarrier
  (JNIEnv *env, jobject object) {
    return getMode2LircDevice(env, object)->canSetSendCarrier();
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    report
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_report
  (JNIEnv *env, jobject object) {
    getMode2LircDevice(env, object)->report();
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    sendNative
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_sendNative
(JNIEnv *env, jobject object, jintArray data) {
    jint *c_data = env->GetIntArrayElements(data, NULL);
    if (c_data == NULL)
        return;
    jsize length = env->GetArrayLength(data);
    Mode2LircDevice *p = getMode2LircDevice(env, object);
    p->send((lirc_t*) c_data, length);
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    setSendCarrier
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_setSendCarrierNative
  (JNIEnv *env, jobject object, jint frequency) {
    getMode2LircDevice(env, object)->setSendCarrier(frequency);
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    getResolutionNative
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_getRecResolutionNative
  (JNIEnv *env, jobject object) {
    return getMode2LircDevice(env, object)->getRecResolution();
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    readNative
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_readNative
  (JNIEnv *env, jobject object) {
    return getMode2LircDevice(env, object)->read();
}
/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    receiveNative
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_receiveNative
(JNIEnv *env, jobject object) {
    IrSequence *irSequence = getMode2LircDevice(env, object)->receive();
    jintArray data = env->NewIntArray(irSequence->getLength());
    if (data == NULL)
        return NULL;
    env->SetIntArrayRegion(data, 0, irSequence->getLength(),
            (const jint*) irSequence->getDurations());
    return data;
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    canGetRecResolution
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_canGetRecResolution
  (JNIEnv *env, jobject object) {
    return getMode2LircDevice(env, object)->canGetRecResolution();
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDevice
 * Method:    openNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDevice_openNative
  (JNIEnv *env, jobject object) {
    return getMode2LircDevice(env, object)->open();
}
