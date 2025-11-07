#ifndef HAPTICS_API_H
#define HAPTICS_API_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialization
void VR_Haptics_OnCreate(jobject context);
void VR_Haptics_OnDestroy();
void VR_Haptics_Disable();
void VR_Haptics_Enable();

// Event methods
void VR_Haptics_Event(const char *event, int position, int flags, int intensity, float angle, float yHeight);
void VR_Haptics_UpdateEvent(const char *event, int intensity, float angle);
void VR_Haptics_StopEvent(const char *event);
void VR_Haptics_EndFrame();

#ifdef __cplusplus
}
#endif

#endif // HAPTICS_API_H
