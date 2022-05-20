#ifndef TRUNCATEIMAGESTARTSTAGE_H
#define TRUNCATEIMAGESTARTSTAGE_H

#include <QPointer>

#include "burnerstage.h"
#include "backup.h"

class TruncateImageStartStage : public BurnerStage
{
public:
    TruncateImageStartStage(const BurnerWidgets& new_burner_widgets,
                            QPointer<Backup> new_backup);
    bool handle() override;

private:
    QPointer<Backup> backup;
};

#endif // TRUNCATEIMAGESTARTSTAGE_H
