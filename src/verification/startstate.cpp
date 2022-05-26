#include "startstate.h"
#include "cancelstate.h"
#include "verification.h"
#include "imageverificationstate.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"

StartState::StartState(Verification* new_verification) :
    VerificationState(new_verification)
{}

void StartState::onTrigger()
{
    if (verification->notEmptyPaths() || verification->verificationChecked())
        verification->prepareWidgetsBeforeCalculations();
}

void StartState::onPrepareWidgetsBeforeCalculations()
{
   /* burner_widgets.data()->status_bar->showMessage(Messages::checksum_calculation_image, 0);
    burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(Messages::checksum_calculation_image);
    burner_widgets.data()->toolbar->actions().at(4)->setIcon(QIcon(":/images/cancel.png"));
    burner_widgets.data()->toolbar->actions().at(4)->setText(QObject::tr("&Cancel"));

    auto burner_progress_bars_setup = QSharedPointer<BurnerProgressBarsSetup>(new BurnerProgressBarsSetup(burner_widgets));
    burner_progress_bars_setup.data()->resetRingAndUnitProgressBarsValues();

    verification->changeState(QSharedPointer<ImageVerificationState>(new ImageVerificationState(verification)));*/
}

QString StartState::onCalculateMd5()
{
   // it's start state, so we start again from beginning
    verification->trigger();
}

void StartState::onCalculateMd5Hash()
{
  // it's start state, so we start again from beginning
    verification->trigger();
}
