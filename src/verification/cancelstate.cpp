#include "cancelstate.h"
#include "startstate.h"
#include "verification.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"

CancelState::CancelState(Verification* new_verification) :
    VerificationState(new_verification)
{}

void CancelState::onTrigger()
{
    verification->changeState(QSharedPointer<StartState>(new StartState(verification)));
    verification->trigger();
}

void CancelState::onPrepareWidgetsBeforeCalculations()
{
   /* burner_widgets.data()->status_bar->showMessage(Messages::checksum_calculation_image, 0);
    burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(Messages::checksum_calculation_image);
    burner_widgets.data()->toolbar->actions().at(4)->setIcon(QIcon(":/images/cancel.png"));
    burner_widgets.data()->toolbar->actions().at(4)->setText(QObject::tr("&Start again"));

    auto burner_progress_bars_setup = QSharedPointer<BurnerProgressBarsSetup>(new BurnerProgressBarsSetup(burner_widgets));
    burner_progress_bars_setup.data()->restoreRingAndUnitProgressBarsValues();*/
}

QString CancelState::onCalculateMd5()
{

}

void CancelState::onCalculateMd5Hash()
{

}
