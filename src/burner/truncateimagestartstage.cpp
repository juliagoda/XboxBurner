#include "truncateimagestartstage.h"
#include "xboxburner.h"

#include "messages/boxmessages.h"
#include "messages/messages.h"

#include <QMessageBox>
#include <QtConcurrent>

TruncateImageStartStage::TruncateImageStartStage(const BurnerWidgets& new_burner_widgets) :
    BurnerStage(new_burner_widgets)
{

}

bool TruncateImageStartStage::createBackup()
{
    QString file_name = burner_widgets.lineedit_image_path->text().simplified();
    QString source = file_name;
    QString destination = Messages::backupCreationDestination(file_name);

    return QFile::copy(source, destination);
}

bool TruncateImageStartStage::handle()
{
    if (burner_widgets.combo_box_dvd_format->currentIndex() != 3)
        return BurnerStage::handle();

    burner_widgets.status_bar->showMessage(QObject::tr("Truncating XGD3 image..."));
    qint64 size = Q_INT64_C(8547991552);
    QString fileName = burner_widgets.lineedit_image_path->text().simplified();
    QFile file(fileName);

    if (file.size() <= size || !burner_widgets.check_box_backup_creation->isChecked())
        return BurnerStage::handle();

    burner_widgets.status_bar->showMessage(Messages::backup_creation);
    burner_widgets.plain_text_edit_with_logs->appendPlainText(Messages::backup_creation);
    burner_progress_bars_setup.data()->resetRingAndUnitProgressBarsValues();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QFuture<bool> backupFuture = QtConcurrent::run(this, &TruncateImageStartStage::createBackup);
#else
    QFuture<bool> backupFuture = QtConcurrent::run(&TruncateImageStartStage::createBackup, this);
#endif
    burner_widgets.backup_future_watcher->setFuture(backupFuture);

}
