#include "settings.h"

#include "listsettingswidget.h"

#include <QRect>
#include <QWidget>

Settings::Settings(QSharedPointer<ListSettingsWidget> new_widgets_list, const ApplicationInformations& applications_informations, QObject* parent)
    : QSettings { applications_informations.getApplicationStorageFormat(), QSettings::UserScope, applications_informations.getOrganizationName(), applications_informations.getApplicationName().toLower(), parent }
    , widgets_list { new_widgets_list }
{
}

void Settings::loadSettings()
{
    QListIterator<QSharedPointer<WidgetDataSaveStrategy>> widgets_list_iterator(*widgets_list.data()->getListWidgetStrategies());

    while (widgets_list_iterator.hasNext())
    {
        auto widget_pointer = widgets_list_iterator.next();
        auto widget = widget_pointer.data();
        widget->loadData(this);
    }
}

void Settings::saveSettings()
{
    QListIterator<QSharedPointer<WidgetDataSaveStrategy>> widgets_list_iterator(*widgets_list.data()->getListWidgetStrategies());

    while (widgets_list_iterator.hasNext())
    {
        auto widget_pointer = widgets_list_iterator.next();
        auto widget = widget_pointer.data();
        widget->saveData(this);
    }
}
