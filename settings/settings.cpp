#include "settings.h"

#include <QDir>
#include <QSettings>
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

QStringList Settings::childKeys(const QString& groupName) const
{
  QStringList result;
  if (groupName.isEmpty() == true) {
    result = settings_->childKeys();
  }
  else {
    settings_->beginGroup(groupName);
    result = settings_->childKeys();
    settings_->endGroup();
  }

  for (auto it = result.begin(), end = result.end(); it != end; ++it) {
    *it = QString("%1/%2").arg(groupName).arg(*it);
  }
  return result;
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
