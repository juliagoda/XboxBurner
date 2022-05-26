#ifndef VERIFICATIONSTATE_H
#define VERIFICATIONSTATE_H

#include "burner/burnerwidgets.h"
#include <QString>

class Verification;

class VerificationState
{
public:
    VerificationState(Verification* new_verification);
    virtual ~VerificationState() = default;
    virtual const QString getStateName();
    virtual void onTrigger() = 0;
    virtual void onPrepareWidgetsBeforeCalculations() = 0;
    virtual QString onCalculateMd5() = 0;
    virtual void onCalculateMd5Hash() = 0;

protected:
    Verification* verification;
};

#endif // VERIFICATIONSTATE_H
