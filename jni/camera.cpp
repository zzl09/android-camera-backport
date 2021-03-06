#define SK_BUILD_FOR_ANDROID

#include "jni.h"
#include "skia/SkCamera.h"

static jfieldID gNativeInstanceFieldID;
static jfieldID gNativeMatrixFieldID;

static void Camera_constructor(JNIEnv* env, jobject obj) {
    Sk3DView* view = new Sk3DView;
    SkMatrix* m = new SkMatrix;
    env->SetIntField(obj, gNativeInstanceFieldID, (int) view);
    env->SetIntField(obj, gNativeMatrixFieldID, (int) m);
}

static void Camera_destructor(JNIEnv* env, jobject obj) {
    delete (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
}

static void Camera_save(JNIEnv* env, jobject obj) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->save();
}

static void Camera_restore(JNIEnv* env, jobject obj) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->restore();
}

static void Camera_translate(JNIEnv* env, jobject obj, float dx, float dy,
        float dz) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->translate(SkFloatToScalar(dx), SkFloatToScalar(dy), SkFloatToScalar(dz));
}

static void Camera_rotateX(JNIEnv* env, jobject obj, float degrees) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->rotateX(SkFloatToScalar(degrees));
}

static void Camera_rotateY(JNIEnv* env, jobject obj, float degrees) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->rotateY(SkFloatToScalar(degrees));
}

static void Camera_rotateZ(JNIEnv* env, jobject obj, float degrees) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->rotateZ(SkFloatToScalar(degrees));
}

static void Camera_rotate(JNIEnv* env, jobject obj, jfloat x, jfloat y,
        jfloat z) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->rotateX(SkFloatToScalar(x));
    v->rotateY(SkFloatToScalar(y));
    v->rotateZ(SkFloatToScalar(z));
}

static void Camera_setLocation(JNIEnv* env, jobject obj, jfloat x, jfloat y,
        jfloat z) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    v->setCameraLocation(SkFloatToScalar(x), SkFloatToScalar(y),
            SkFloatToScalar(z));
}

static jfloat Camera_getLocationX(JNIEnv* env, jobject obj) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    return SkScalarToFloat(v->getCameraLocationX());
}

static jfloat Camera_getLocationY(JNIEnv* env, jobject obj) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    return SkScalarToFloat(v->getCameraLocationY());
}

static jfloat Camera_getLocationZ(JNIEnv* env, jobject obj) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    return SkScalarToFloat(v->getCameraLocationZ());
}

static void Camera_getMatrix(JNIEnv* env, jobject obj, jfloatArray values) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    SkMatrix* m = (SkMatrix*) env->GetIntField(obj, gNativeMatrixFieldID);
    v->getMatrix(m);
    env->SetFloatArrayRegion(values, 0, 9, m->fMat);
}

static float Camera_dotWithNormal(JNIEnv* env, jobject obj, float x, float y,
        float z) {
    Sk3DView* v = (Sk3DView*) env->GetIntField(obj, gNativeInstanceFieldID);
    SkScalar dot = v->dotWithNormal(SkFloatToScalar(x), SkFloatToScalar(y),
            SkFloatToScalar(z));
    return SkScalarToFloat(dot);
}

/*
 * JNI registration.
 */
static JNINativeMethod gCameraMethods[] = {
    /* name, signature, funcPtr */
    { "nativeConstructor",  "()V",      (void*) Camera_constructor   },
    { "nativeDestructor",   "()V",      (void*) Camera_destructor    },
    { "save",               "()V",      (void*) Camera_save          },
    { "restore",            "()V",      (void*) Camera_restore       },
    { "translate",          "(FFF)V",   (void*) Camera_translate     },
    { "rotateX",            "(F)V",     (void*) Camera_rotateX       },
    { "rotateY",            "(F)V",     (void*) Camera_rotateY       },
    { "rotateZ",            "(F)V",     (void*) Camera_rotateZ       },
    { "rotate",             "(FFF)V",   (void*) Camera_rotate        },
    { "setLocation",        "(FFF)V",   (void*) Camera_setLocation   },
    { "getLocationX",       "()F",      (void*) Camera_getLocationX  },
    { "getLocationY",       "()F",      (void*) Camera_getLocationY  },
    { "getLocationZ",       "()F",      (void*) Camera_getLocationZ  },
    { "nativeGetMatrix",    "([F)V",    (void*) Camera_getMatrix     },
    { "dotWithNormal",      "(FFF)F",   (void*) Camera_dotWithNormal }
};

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    jclass clazz = env->FindClass(
            "com/zhcnxf/android/backports/graphics/Camera");
    if (clazz == 0) {
        return JNI_ERR;
    }

    gNativeInstanceFieldID = env->GetFieldID(clazz, "native_instance", "I");
    if (gNativeInstanceFieldID == 0) {
        return JNI_ERR;
    }

    gNativeMatrixFieldID = env->GetFieldID(clazz, "native_matrix", "I");
    if (gNativeMatrixFieldID == 0) {
        return JNI_ERR;
    }

    int len = sizeof(gCameraMethods) / sizeof(gCameraMethods[0]);
    if (env->RegisterNatives(clazz, gCameraMethods, len) < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
