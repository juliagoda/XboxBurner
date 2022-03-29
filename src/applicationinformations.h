#pragma once

#include <QObject>
#include <QString>

class ApplicationInformations : public QObject {
    Q_OBJECT

public:
    explicit ApplicationInformations(QObject* parent = nullptr);
    QString getApplicationName();
    QString getOrganizationDomain();
    QString getOrganizationName();
    QString getApplicationVersion();
    QString getApplicationFullName();
};
