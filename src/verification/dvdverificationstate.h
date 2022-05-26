#ifndef DVDVERIFICATIONSTATE_H
#define DVDVERIFICATIONSTATE_H

#include "verificationstate.h"
#include "burner/burnerprogressbarssetup.h"

class DvdVerificationState : public VerificationState
{
public:
    DvdVerificationState(Verification* new_verification);
    void onTrigger() override;
    void onPrepareWidgetsBeforeCalculations() override;
    QString onCalculateMd5() override;
    void onCalculateMd5Hash() override;

private:
    QSharedPointer<BurnerProgressBarsSetup> burner_progress_bars_setup;
};

#endif // DVDVERIFICATIONSTATE_H
