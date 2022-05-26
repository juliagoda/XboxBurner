#ifndef CANCELSTATE_H
#define CANCELSTATE_H

#include "verificationstate.h"

class CancelState : public VerificationState
{
public:
    CancelState(Verification* new_verification);
    void onTrigger() override;
    void onPrepareWidgetsBeforeCalculations() override;
    QString onCalculateMd5() override;
    void onCalculateMd5Hash() override;
};

#endif // CANCELSTATE_H
