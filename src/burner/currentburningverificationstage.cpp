#include "currentburningverificationstage.h"
#include "messages/boxmessages.h"

#include <QMessageBox>

CurrentBurningVerificationStage::CurrentBurningVerificationStage(const BurnerWidgets& new_burner_widgets) :
    BurnerStage(new_burner_widgets)
{

}

bool CurrentBurningVerificationStage::handle()
{
    if (burning)
    {
        if (BoxMessages::cancelBurnProcessMessage(new QWidget) == QMessageBox::Ok)
            burn_process->kill();
        return false;
    }

    return BurnerStage::handle();
}
