#include "org_harctoolbox_devslashlirc_LircDevice.h"
#include "LircDevice.h"
#include <iostream>

using namespace std;

// private helper function
static LircDevice *getLircDevice(JNIEnv *env, jobject object) {
    jclass clazz = env->GetObjectClass(object);
    jfieldID fidNumber = env->GetFieldID(clazz, "nativePointer", "J");
    if (fidNumber == NULL) {
        std::cerr << "Error: could not get fidNumber" << std::endl;
        return NULL;
    }
    return (LircDevice*) env->GetLongField(object, fidNumber);
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_close
  (JNIEnv *env, jobject object) {
    //delete getLircDevice(env, object);
    getLircDevice(env, object)->close();
}

///*
// * Class:     org_harctoolbox_devslashlirc_LircDevice
// * Method:    isValid
// * Signature: ()Z
// */
//JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_isValid
//  (JNIEnv *env, jobject) {
//    return true;
//}

///*
// * Class:     org_harctoolbox_devslashlirc_LircDevice
// * Method:    sayHello
// * Signature: ()V
// */
//JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_sayHello
//  (JNIEnv *env __attribute__((unused)), jclass clazz __attribute__((unused))) {
//    cout << "Hello world" << endl;
//}

///*
// * Class:     org_harctoolbox_devslashlirc_LircDevice
// * Method:    finalize
// * Signature: ()V
// */
//JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_finalize
//  (JNIEnv *env, jobject object) {
//    LircDevice *p = getLircDevice(env, object);
//    delete p;
//    jclass clazz = env->GetObjectClass(object);
//    jfieldID fidNumber = env->GetFieldID(clazz, "nativePointer", "J");
//    if (fidNumber == NULL) {
//        std::cerr << "Error: could not get fidNumber" << std::endl;
//        return;
//    }
//    env->SetLongField(object, fidNumber, 0L);
//}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    getVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_getVersion
(JNIEnv *env, jobject object __attribute__((unused))) {
    return env->NewStringUTF(LircDevice::version);
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    setTransmitterMaskNative
 * Signature: (I)V
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_setTransmitterMaskNative
  (JNIEnv *env, jobject object, jint mask) {
    return getLircDevice(env, object)->setTransmitterMask(mask) == 0;

}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    getNumberTransmitters
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_getNumberTransmitters
  (JNIEnv *env, jobject object) {
    return getLircDevice(env, object)->getNumberTransmitters();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    canSend
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_canSend
  (JNIEnv *env, jobject object) {
    return getLircDevice(env, object)->canSend();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    canSetTransmitterMask
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_canSetTransmitterMask
  (JNIEnv *env, jobject object) {
    return getLircDevice(env, object)->canSetTransmitterMask();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    canRec
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_canRec
  (JNIEnv *env, jobject object) {
    return getLircDevice(env, object)->canRec();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    isValid
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_isValid
  (JNIEnv *env, jobject object) {
    return getLircDevice(env, object)->isValid();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    delete
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_delete
  (JNIEnv *env, jobject object) {
    LircDevice* d = getLircDevice(env, object);
    delete d;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDevice
 * Method:    setBeginTimeout
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDevice_setBeginTimeout
  (JNIEnv *env, jobject object, jint timeout) {
    LircDevice* d = getLircDevice(env, object);
    d->setBeginTimeout(timeout);
}
