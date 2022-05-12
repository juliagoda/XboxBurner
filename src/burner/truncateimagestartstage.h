#ifndef TRUNCATEIMAGESTARTSTAGE_H
#define TRUNCATEIMAGESTARTSTAGE_H

#include "burnerstage.h"


class TruncateImageStartStage : public BurnerStage
{
public:
    TruncateImageStartStage(const BurnerWidgets& new_burner_widgets);
    bool handle() override;

private:
    bool createBackup();
};

#endif // TRUNCATEIMAGESTARTSTAGE_H
