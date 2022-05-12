#ifndef CURRENTBURNINGVERIFICATIONSTAGE_H
#define CURRENTBURNINGVERIFICATIONSTAGE_H

#include "burnerstage.h"

#include <QProcess>

class CurrentBurningVerificationStage : public BurnerStage {
public:
    CurrentBurningVerificationStage(const BurnerWidgets& new_burner_widgets);
    bool handle() override;
};

#endif // CURRENTBURNINGVERIFICATIONSTAGE_H
