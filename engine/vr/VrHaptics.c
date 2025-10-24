#include "VrBase.h"
#include "VrHaptics.h"
#include <jni.h>
#include <string.h>
#include <android/log.h>

static JavaVM* g_vm = NULL;
static jclass g_hapticsClass = NULL;

// Automatically called when library is loaded by JVM
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
		return JNI_ERR;
	}

	g_vm = vm;

	jclass localClass = (*env)->FindClass(env, "com/drbeef/externalhapticsservice/HapticsAPI");
	if (!localClass) {
		ALOGE("Failed to find com.drbeef.externalhapticsservice.HapticsAPI");
		return JNI_ERR;
	}

	g_hapticsClass = (*env)->NewGlobalRef(env, localClass);
	(*env)->DeleteLocalRef(env, localClass);

	ALOGV("HapticsAPI JNI_OnLoad initialized successfully");

	return JNI_VERSION_1_6;
}

// Utility: get JNIEnv for current thread (attach if needed)
static JNIEnv* getEnv() {
	if (!g_vm) return NULL;
	JNIEnv* env = NULL;
	if ((*g_vm)->GetEnv(g_vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
		if ((*g_vm)->AttachCurrentThread(g_vm, &env, NULL) != 0) {
			ALOGE("Failed to attach thread to JVM");
			return NULL;
		}
	}
	return env;
}

// Utility: call a static void method with varargs
static void callStaticVoidMethod(const char* name, const char* sig, ...) {
	JNIEnv* env = getEnv();
	if (!env || !g_hapticsClass) return;

	jmethodID mid = (*env)->GetStaticMethodID(env, g_hapticsClass, name, sig);
	if (!mid) {
		ALOGE("Static method not found: %s %s", name, sig);
		return;
	}

	va_list args;
	va_start(args, sig);
	(*env)->CallStaticVoidMethodV(env, g_hapticsClass, mid, args);
	va_end(args);
}

void VR_Haptics_OnCreate(jobject context) {
	JNIEnv* env = getEnv();
	if (!env || !g_hapticsClass) return;
	jmethodID mid = (*env)->GetStaticMethodID(env, g_hapticsClass, "onCreate", "(Landroid/content/Context;)V");
	if (!mid) return;
	(*env)->CallStaticVoidMethod(env, g_hapticsClass, mid, (jobject)context);
}

void VR_Haptics_OnDestroy() {
	callStaticVoidMethod("onDestroy", "()V");
}

void VR_Haptics_Disable() {
	callStaticVoidMethod("disable", "()V");
}

void VR_Haptics_Enable() {
	callStaticVoidMethod("enable", "()V");
}

void VR_Haptics_Event(const char *event, int position, int flags, int intensity, float angle, float yHeight) {
	JNIEnv* env = getEnv();
	if (!env) return;
	jmethodID mid = (*env)->GetStaticMethodID(env, g_hapticsClass, "event", "(Ljava/lang/String;IIIFF)V");
	if (!mid) return;
	jstring jEvent = (*env)->NewStringUTF(env, event);
	(*env)->CallStaticVoidMethod(env, g_hapticsClass, mid, jEvent, position, flags, intensity, angle, yHeight);
	(*env)->DeleteLocalRef(env, jEvent);
}

void VR_Haptics_UpdateEvent(const char *event, int intensity, float angle) {
	JNIEnv* env = getEnv();
	if (!env) return;
	jmethodID mid = (*env)->GetStaticMethodID(env, g_hapticsClass, "updateevent", "(Ljava/lang/String;IF)V");
	if (!mid) return;
	jstring jEvent = (*env)->NewStringUTF(env, event);
	(*env)->CallStaticVoidMethod(env, g_hapticsClass, mid, jEvent, intensity, angle);
	(*env)->DeleteLocalRef(env, jEvent);
}

void VR_Haptics_StopEvent(const char *event) {
	JNIEnv* env = getEnv();
	if (!env) return;
	jmethodID mid = (*env)->GetStaticMethodID(env, g_hapticsClass, "stopevent", "(Ljava/lang/String;)V");
	if (!mid) return;
	jstring jEvent = (*env)->NewStringUTF(env, event);
	(*env)->CallStaticVoidMethod(env, g_hapticsClass, mid, jEvent);
	(*env)->DeleteLocalRef(env, jEvent);
}

void VR_Haptics_EndFrame() {
	callStaticVoidMethod("endframe", "()V");
}
