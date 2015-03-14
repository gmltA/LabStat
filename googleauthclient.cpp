#include "googleauthclient.h"

#include <QAndroidJniObject>
#include <QtAndroid>
#include <QDebug>

#include "GoogleDrive/driveapi.h"

void GoogleAuthClient::onTokenObtained(QString token)
{
    IDataStore* dataStore = &GoogleDriveAPI::getInstance();
    dataStore->setToken(token);

    //dataStore->test();
    dataStore->createFile();
}

static void fjpassToken(JNIEnv *env, jobject thiz, jstring str)
{
    Q_UNUSED(env)
    Q_UNUSED(thiz)
    QString token = QAndroidJniObject(str).toString();

    GoogleAuthClient::getInstance().tokenObtained(token);
}

static JNINativeMethod methods[] =
{
    {"passToken", "(Ljava/lang/String;)V", (void*)(fjpassToken)}
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

    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/labstat/GoogleAuthClient",
                                                                           "getActivity",
                                                                           "()Lorg/qtproject/labstat/GoogleAuthClient;");

    QAndroidJniObject res = activity.callObjectMethod("processDriveAuth",
                                           "()Ljava/lang/String;");

    qDebug() << res.toString();
}
