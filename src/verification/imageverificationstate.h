#ifndef IMAGEVERIFICATIONSTATE_H
#define IMAGEVERIFICATIONSTATE_H

#include "verificationstate.h"
#include "burner/burnerprogressbarssetup.h"

class ImageVerificationState : public VerificationState
{
public:
    ImageVerificationState(Verification* new_verification);
    void onTrigger() override;
    void onPrepareWidgetsBeforeCalculations() override;
    QString onCalculateMd5() override;
    void onCalculateMd5Hash() override;

private:
    QSharedPointer<BurnerProgressBarsSetup> burner_progress_bars_setup;
};

#endif // IMAGEVERIFICATIONSTATE_H
