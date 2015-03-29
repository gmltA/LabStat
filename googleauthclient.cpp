#include "googleauthclient.h"

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#include <QDebug>

#include "GoogleDrive/driveapi.h"

void GoogleAuthClient::onTokenObtained(QString token)
{
    // Why? Multithreading, that's why!
    emit authCompleted(token);
}

static void fjpassToken(JNIEnv *env, jobject thiz, jstring str, jlong authClientAddress)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)
    QString token = QAndroidJniObject(str).toString();

    GoogleAuthClient* client = (GoogleAuthClient*)authClientAddress;
    client->tokenObtained(token);
}

static JNINativeMethod methods[] =
{
    {"passToken", "(Ljava/lang/String;J)V", (void*)(fjpassToken)}
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass javaClass = env->FindClass("org/qtproject/labstat/GoogleAuthClient");
    if (!javaClass)
        return JNI_ERR;

    if (env->RegisterNatives(javaClass, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

void GoogleAuthClient::processAuth()
{
    long thisAddress = (long)this;
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/labstat/GoogleAuthClient",
                                                                           "getActivity",
                                                                           "()Lorg/qtproject/labstat/GoogleAuthClient;");

    QAndroidJniObject res = activity.callObjectMethod("processDriveAuth", "(J)Ljava/lang/String;", (jlong)thisAddress);
}
