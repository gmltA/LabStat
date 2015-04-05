#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>

#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras>
#endif

#include "GoogleDrive/driveapi.h"
#include "googleauthclient.h"
#include "googledesktopauthclient.h"
#include "synchandler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("LabStat");
    app.setOrganizationName("BSUIR");

    QScreen *screen = qApp->primaryScreen();
    int dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
    bool isMobile = false;

#if defined(Q_OS_IOS)
    // iOS integration of scaling (retina, non-retina, 4K) does itself.
    dpi = screen->physicalDotsPerInch();
    isMobile = true;
#elif defined(Q_OS_ANDROID)
    // https://bugreports.qt-project.org/browse/QTBUG-35701
    // recalculate dpi for Android

    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject resources = activity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();

        return EXIT_FAILURE;
    }

    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();

        return EXIT_FAILURE;
    }
    dpi = displayMetrics.getField<int>("densityDpi");
    isMobile = true;
#else
    // standard dpi
    dpi = screen->logicalDotsPerInch() * screen->devicePixelRatio();
#endif

    // now calculate the dp ratio
    qreal dp = dpi / 160.f;

    SyncHandler::init();
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("dp", dp);
    engine.rootContext()->setContextProperty("isMobile", isMobile);

    engine.load(QUrl("qrc:/QML/main.qml"));

    IAuthClient* authClient;
#if defined(Q_OS_ANDROID)
    authClient = new GoogleAuthClient();
#else
    authClient = new GoogleDesktopAuthClient();
#endif
    GoogleDriveAPI* drive = new GoogleDriveAPI(authClient, "LabStat");


    SyncHandler::getInstance()->registerProcessor(drive);

    return app.exec();
}
