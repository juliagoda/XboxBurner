#include "burningacceptancestage.h"
#include "messages/boxmessages.h"
#include "messages/messages.h"

#include <QMessageBox>

BurningAcceptanceStage::BurningAcceptanceStage(const BurnerWidgets& new_burner_widgets) :
    BurnerStage(new_burner_widgets)
{

}

bool BurningAcceptanceStage::handle()
{
    if (BoxMessages::startBurnProcessMessage(new QWidget) != QMessageBox::Ok)
        return false;

    burner_widgets.plain_text_edit_with_logs->appendPlainText(Messages::burn_process_start_with_date);
    burner_widgets.toolbar->actions().at(3)->setIcon(QIcon(":/images/cancel.png"));
    burner_widgets.toolbar->actions().at(3)->setText(QObject::tr("&Cancel"));
    burner_widgets.progress_bar_burn->setValue(0);
    burner_widgets.progress_bar_ring_buffer_unit->setValue(0);
    burner_widgets.progress_bar_unit_buffer_unit->setValue(0);

    return BurnerStage::handle();
}
