#include "standardpath.h"
#include <QStandardPaths>
#include <QApplication>
#include <QDir>
#include <QDebug>

StandardPath::StandardPath()
{

}

StandardPath::~StandardPath()
{

}

QString StandardPath::getAppConfigPath()
{
    return APPSHAREDIR;
}

QString StandardPath::getHomePath()
{
    return QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0);
}

QString StandardPath::getDesktopPath()
{
    return QString("%1/%2").arg(getHomePath(), "Desktop");
}

QString StandardPath::getVideosPath()
{
    return QString("%1/%2").arg(getHomePath(), "Videos");
}

QString StandardPath::getMusicPath()
{
    return QString("%1/%2").arg(getHomePath(), "Music");
}

QString StandardPath::getPicturesPath()
{
    return QString("%1/%2").arg(getHomePath(), "Pictures");
}

QString StandardPath::getDocumentsPath()
{
    return QString("%1/%2").arg(getHomePath(), "Documents");
}

QString StandardPath::getDownloadsPath()
{
    return QString("%1/%2").arg(getHomePath(), "Downloads");
}

QString StandardPath::getCachePath()
{
    QString projectName = qApp->applicationName();
    QDir::home().mkpath(".cache");
    QDir::home().mkpath(QString("%1/%2/").arg(".cache", projectName));
    QString defaultPath = QString("%1/%2/%3").arg(QDir::homePath(), ".cache", projectName);
    return defaultPath;
}

QString StandardPath::getConfigPath()
{
    QString projectName = qApp->applicationName();
    QDir::home().mkpath(".config");
    QDir::home().mkpath(QString("%1/%2/").arg(".config", projectName));
    QString defaultPath = QString("%1/%2/%3").arg(QDir::homePath(), ".config", projectName);
    return defaultPath;
}

QString StandardPath::getTrashPath()
{
    QDir::home().mkpath(".local/");
    QDir::home().mkpath(".local/share/");
    QDir::home().mkpath( ".local/share/Trash/" );
    QDir::home().mkpath( ".local/share/Trash/files/" );
    QDir::home().mkpath( ".local/share/Trash/info/" );
    return QDir::homePath() + "/.local/share/Trash/";
}
