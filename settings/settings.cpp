#include "settings.h"

#include <QDir>
#include <QSettings>
#include <QString>
#include <QStringList>

namespace {

QString settingsFileName() { return QString("%1/.garsale/garsale.conf").arg(QDir::homePath()); }

}

namespace garsale {

Settings::Settings() :
  settings_(new QSettings(::settingsFileName(), QSettings::IniFormat))
{
}

Settings::~Settings() = default;

bool Settings::contains(const QString& key) const
{
  return settings_->contains(key);
}

QString Settings::fileName() const
{
  return settings_->fileName();
}

QStringList Settings::childGroups() const
{
  return settings_->childGroups();
}

void Settings::setValue(const QString& key, const QVariant& value)
{
  settings_->setValue(key, value);
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue) const
{
  return settings_->value(key, defaultValue);
}

} // garsale
