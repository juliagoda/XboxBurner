#ifndef STARTSTATE_H
#define STARTSTATE_H

#include "verificationstate.h"

class StartState : public VerificationState
{
public:
    StartState(Verification* new_verification);
    void onTrigger() override;
    void onPrepareWidgetsBeforeCalculations() override;
    QString onCalculateMd5() override;
    void onCalculateMd5Hash() override;
};

#endif // STARTSTATE_H
