#include "pathsandspeedcontentverificationstage.h"
#include "messages/boxmessages.h"

#include <QMessageBox>

PathsAndSpeedContentVerificationStage::PathsAndSpeedContentVerificationStage(const BurnerWidgets& new_burner_widgets) :
    BurnerStage(new_burner_widgets)

{

}

bool PathsAndSpeedContentVerificationStage::handle()
{
    if (burner_widgets.lineedit_image_path->text().isEmpty() ||
        burner_widgets.lineedit_burner_path->text().isEmpty() ||
        burner_widgets.combo_box_write_speed->currentText().isEmpty())
        return false;

    return BurnerStage::handle();
}
