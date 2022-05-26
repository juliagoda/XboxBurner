#include "verificationstate.h"
#include "verification.h"

VerificationState::VerificationState(Verification* new_verification) :
    verification(new_verification)
{}

const QString VerificationState::getStateName()
{
    return typeid(*this).name();
}
