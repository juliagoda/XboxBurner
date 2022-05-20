#ifndef BURNERSTARTSTAGE_H
#define BURNERSTARTSTAGE_H

#include "burnerstage.h"


class BurnerStartStage : public BurnerStage
{
public:
    BurnerStartStage(const BurnerWidgets& new_burner_widgets);
    bool handle() override;

private slots:
    void burnProcess_finished(const int exitCode,
                              const QProcess::ExitStatus exitStatus);
    void burnProcess_readyReadStandardOutput();

private:
    QString layerBreak();
};

#endif // BURNERSTARTSTAGE_H
