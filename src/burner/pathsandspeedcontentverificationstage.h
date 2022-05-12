#ifndef PATHSCONTENTVERIFICATIONSTAGE_H
#define PATHSCONTENTVERIFICATIONSTAGE_H

#include "burnerstage.h"

class PathsAndSpeedContentVerificationStage : public BurnerStage {
public:
    PathsAndSpeedContentVerificationStage(const BurnerWidgets& new_burner_widgets);
    bool handle() override;
};

#endif // PATHSCONTENTVERIFICATIONSTAGE_H
