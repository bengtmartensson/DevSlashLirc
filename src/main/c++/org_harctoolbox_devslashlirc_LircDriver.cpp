#include "org_harctoolbox_devslashlirc_LircDriver.h"
#include "LircDriver.h"

// private helper function
static LircDriver* getLircDriver(JNIEnv *env, jobject object) {
    jclass clazz = env->GetObjectClass(object);
    jfieldID fidNumber = env->GetFieldID(clazz, "nativePointer", "J");
    if (fidNumber == NULL) {
        std::cerr << "Error: could not get fidNumber" << std::endl;
        return NULL;
    }
    return (LircDriver*) env->GetLongField(object, fidNumber);
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    setupLog
 * Signature: (Ljava/lang/String;IZ)V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_setupLog
  (JNIEnv *env, jclass clazz __attribute__((unused)), jstring fileName, jint logLevel, jboolean append) {
    const char *file = env->GetStringUTFChars(fileName, NULL);
    if (file == NULL)
        return;
    LircDriver::setupLog(file, (loglevel_t) logLevel, append);
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    closeLog
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_closeLog
  (JNIEnv *, jclass) {
    LircDriver::closeLog();
}



/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    setPluginDir
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_setPluginDir
  (JNIEnv *env, jclass clazz __attribute__((unused)), jstring path) {
    const char *pluginPath = env->GetStringUTFChars(path, NULL);
    if (pluginPath == NULL)
        return;

    LircDriver::setPlugindir(pluginPath);
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    openNative
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_openNative__Ljava_lang_String_2
  (JNIEnv *env, jobject object, jstring deviceName) {
    const char *devName = env->GetStringUTFChars(deviceName, NULL);
    if (devName == NULL)
        return false;
    LircDriver* lircDriver = getLircDriver(env, object);
    return lircDriver->open(devName);
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    openNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_openNative__
  (JNIEnv *, jobject) {
    return false;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    initNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_initNative
  (JNIEnv *env, jobject object) {
    LircDriver* lircDriver = getLircDriver(env, object);
    return lircDriver->init();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    deinitNative
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_deinitNative
  (JNIEnv *, jobject) {
    return false;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    isReady
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_isReady
  (JNIEnv *, jobject) {
    return false;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    canRec
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_canRec
  (JNIEnv* env, jobject object) {
    LircDriver* lircDriver = getLircDriver(env, object);
    return lircDriver->canRec();
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    canSend
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_canSend
  (JNIEnv *, jobject) {
    return false;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    canSetTransmitterMask
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_canSetTransmitterMask
  (JNIEnv *, jobject) {
    return false;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_close
  (JNIEnv *, jobject) {

}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    getNumberTransmitters
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_getNumberTransmitters
  (JNIEnv *, jobject) {
    return false;
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    getVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_getVersion
  (JNIEnv *env, jobject object) {
    LircDriver* lircDriver = getLircDriver(env, object);
    return env->NewStringUTF(lircDriver->getVersion());
}

/*
 * Class:     org_harctoolbox_devslashlirc_LircDriver
 * Method:    isValid
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_isValid
  (JNIEnv *, jobject) {
    return false;
}

///*
// * Class:     org_harctoolbox_devslashlirc_LircDriver
// * Method:    setTransmitterMask
// * Signature: (I)V
// */
//JNIEXPORT void JNICALL Java_org_harctoolbox_devslashlirc_LircDriver_setTransmitterMask
//  (JNIEnv *, jobject, jint) {
//
//}
