#ifndef BACKUP_H
#define BACKUP_H

#include "burner/burnerstage.h"


class Backup : public QObject
{
    Q_OBJECT

public:
    explicit Backup(const BurnerWidgets& new_burner_widgets);

public slots:
    bool create();
    void log();

private:
    void resizeImage();

    BurnerWidgets burner_widgets;
    BurnerProgressBarsSetup burner_progress_bars_setup;
};

#endif // BACKUP_H
