#include "org_harctoolbox_devslashlirc_Mode2LircDriver.h"
#include "Mode2LircDriver.h"

// private helper function
static Mode2LircDriver* getMode2LircDriver(JNIEnv *env, jobject object) {
    jclass clazz = env->GetObjectClass(object);
    jfieldID fidNumber = env->GetFieldID(clazz, "nativePointer", "J");
    if (fidNumber == NULL) {
        std::cerr << "Error: could not get fidNumber" << std::endl;
        return NULL;
    }
    return (Mode2LircDriver*) env->GetLongField(object, fidNumber);
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    newInstance
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_newInstance__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *env, jclass clazz __attribute__((unused)), jstring name, jstring path) {
    const char *driver_name = env->GetStringUTFChars(name, NULL);
    if (driver_name == NULL)
        return 0L;

    const char *path_name = env->GetStringUTFChars(path, NULL);
    if (path_name == NULL)
        return 0L;

    Mode2LircDriver *p = Mode2LircDriver::newInstance(driver_name, path_name);
    //std::cerr << (long) p;
    return (long) p;
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    newInstance
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_newInstance__Ljava_lang_String_2
(JNIEnv * env, jclass clazz __attribute__((unused)), jstring name) {
    const char *driver_name = env->GetStringUTFChars(name, NULL);
    if (driver_name == NULL)
        return 0L;

    Mode2LircDriver *p = Mode2LircDriver::newInstance(driver_name);
    return (long) p;
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    canGetRecResolution
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_canGetRecResolution
  (JNIEnv *env, jobject object) {
    Mode2LircDriver* driver = getMode2LircDriver(env, object);
    return driver->canGetRecResolution();
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    canSetSendCarrier
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_canSetSendCarrier
  (JNIEnv *env, jobject object) {
    Mode2LircDriver* driver = getMode2LircDriver(env, object);
    return driver->canGetRecResolution();
}

///*
// * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
// * Method:    setSendCarrierNative
// * Signature: (I)V
// */
//JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_setSendCarrierNative
//  (JNIEnv *, jobject, jint) {
//
//}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    getResolution
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_getRecResolution
  (JNIEnv *env, jobject object) {
    Mode2LircDriver* driver = getMode2LircDriver(env, object);
    return driver->getRecResolution();
}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    receiveNative
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_receiveNative
  (JNIEnv *env, jobject object) {
    Mode2LircDriver* driver = getMode2LircDriver(env, object);
    IrSequence* irSequence = driver->receive();
    if (irSequence == NULL)
        return NULL;
    jintArray data = env->NewIntArray(irSequence->getLength());
    if (data == NULL)
        return NULL;
    env->SetIntArrayRegion(data, 0, irSequence->getLength(),
            (const jint*) irSequence->getDurations());
    return data;
}

///*
// * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
// * Method:    report
// * Signature: ()V
// */
//JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_report
//  (JNIEnv *env, jobject object) {
//    Mode2LircDriver* driver = getMode2LircDriver(env, object);
//    driver->report();
//}

/*
 * Class:     org_harctoolbox_devslashlirc_Mode2LircDriver
 * Method:    sendNative
 * Signature: ([II)Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_Mode2LircDriver_sendNative
  (JNIEnv *, jobject, jintArray, jint) {
    return false;
}
