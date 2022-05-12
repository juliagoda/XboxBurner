#ifndef BURNINGACCEPTANCESTAGE_H
#define BURNINGACCEPTANCESTAGE_H

#include "burnerstage.h"

#include <QPlainTextEdit>
#include <QToolBar>
#include <QProgressBar>

class BurningAcceptanceStage : public BurnerStage
{
public:
    BurningAcceptanceStage(const BurnerWidgets& new_burner_widgets);
    bool handle() override;
};

#endif // BURNINGACCEPTANCESTAGE_H
