#include "durl.h"

#include "app/global.h"
#include "app/logutil.h"
#include "app/filemanagerapp.h"
#include "app/singleapplication.h"

#include "widgets/commandlinemanager.h"

#include "dialogs/dialogmanager.h"
#include "shutil/fileutils.h"
#include "shutil/mimesappsmanager.h"
#include "dialogs/openwithdialog.h"
#include "controllers/appcontroller.h"
#include "widgets/singleton.h"

#include <dthememanager.h>
#include <dwindow.h>

#include <QApplication>
#include <QDebug>
#include <QThreadPool>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>

#include "xdnd/xdndworkaround.h"

#ifdef ENABLE_PPROF
#include <gperftools/profiler.h>
#endif

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
#ifdef ENABLE_PPROF
    ProfilerStart("pprof.prof");
#endif
    SingleApplication::loadDXcbPlugin();
    SingleApplication::initSources();
    SingleApplication app(argc, argv);

    app.setOrganizationName(QMAKE_ORGANIZATION_NAME);
    app.setApplicationName(QMAKE_TARGET);
    app.setApplicationVersion(QMAKE_VERSION);

    LogUtil::registerLogger();

    CommandLineManager::instance()->process();

    DUrl commandlineUrl;

    if (CommandLineManager::instance()->positionalArguments().count() > 0){
        commandlineUrl = DUrl::fromUserInput(CommandLineManager::instance()->positionalArguments().at(0));
    } else {
        commandlineUrl = DUrl::fromLocalFile(QDir::homePath());
    }

    QString uniqueKey = app.applicationName();

    bool isSingleInstance  = app.setSingleInstance(uniqueKey);

    qDebug() << isSingleInstance << commandlineUrl;

    if (isSingleInstance){
        QTranslator translator;

        if (translator.load(APPSHAREDIR"/translations/" + app.applicationName() +"_" + QLocale::system().name()))
            app.installTranslator(&translator);

        QTranslator translator_qt;

        if (translator_qt.load(QLibraryInfo::location(QLibraryInfo::TranslationsPath) + "/qt_" + QLocale::system().name() + ".qm"))
            app.installTranslator(&translator_qt);

        DThemeManager::instance()->setTheme("light");

        /// fix Qt drag drop to google chrome bug
        new XdndWorkaround();

        app.setApplicationDisplayName(QObject::tr("File Manager"));
        fileManagerApp->show(commandlineUrl);
        dialogManager;
        appController->createGVfSManager();
        QThreadPool::globalInstance()->setMaxThreadCount(MAX_THREAD_COUNT);
        FileUtils::setDefaultFileManager();
#ifdef ENABLE_PPROF
        int request = app.exec();

        ProfilerStop();
        quick_exit(request);
#else
        quick_exit(app.exec());
#endif
    }else{
        SingleApplication::newClientProcess(uniqueKey);
        QWidget w;
        w.setWindowFlags(Qt::FramelessWindowHint);
        w.setAttribute(Qt::WA_TranslucentBackground);
        w.resize(0, 0);
        w.show();
    }
}