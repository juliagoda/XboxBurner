#include "imageverificationstate.h"
#include "dvdverificationstate.h"
#include "verification.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"
#include "cancelstate.h"

#include <QtConcurrent>

ImageVerificationState::ImageVerificationState(Verification* new_verification) :
    VerificationState(new_verification),
    burner_progress_bars_setup{ QSharedPointer<BurnerProgressBarsSetup>(new BurnerProgressBarsSetup(verification->getBurnerWidgets())) }
{
    QObject::connect(verification->getBurnerWidgets()->image_future_watcher, &QFutureWatcher<QString>::finished, verification, &Verification::calculateMd5Hash);
}

void ImageVerificationState::onTrigger()
{
    verification->changeState(QSharedPointer<CancelState>(new CancelState(verification)));
    verification->trigger();
}

void ImageVerificationState::onPrepareWidgetsBeforeCalculations()
{
    verification->getBurnerWidgets()->status_bar->showMessage(Messages::checksum_calculation_image, 0);
    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText(Messages::checksum_calculation_image);
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setIcon(QIcon(":/images/cancel.png"));
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setText(QObject::tr("&Cancel verification"));

    burner_progress_bars_setup.data()->resetRingAndUnitProgressBarsValues();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QFuture<QString> imageFuture = QtConcurrent::run(this, &ImageVerificationState::onCalculateMd5);
#else
        QFuture<QString> imageFuture = QtConcurrent::run(&ImageVerificationState::onCalculateMd5, this);
#endif
        verification->getBurnerWidgets()->image_future_watcher->setFuture(imageFuture);
}

QString ImageVerificationState::onCalculateMd5()
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile image(verification->getBurnerWidgets()->lineedit_image_path->text().simplified());
    int block_count = image.size() / verification->getBlockSize();
    verification->setMaximalPossibleMD5HashValue(block_count);

    if (image.open(QFile::ReadOnly))
    {
        for (int block_number = 0; block_number < block_count; block_number++)
        {
            hash.addData(image.read(verification->getBlockSize()));
            verification->calculateMD5HashForProgressBar(block_number + 1);
        }

        image.close();
        return hash.result().toHex();
    }

    return QString();
}

void ImageVerificationState::onCalculateMd5Hash()
{
    QString md5_hex_result = verification->getBurnerWidgets()->image_future_watcher->result();

    if (md5_hex_result.isEmpty())
    {
        verification->getBurnerWidgets()->status_bar->showMessage(Messages::checksum_calculation_image_fail);
        verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText(Messages::checksum_calculation_image_fail);
        verification->changeState(QSharedPointer<CancelState>(new CancelState(verification)));
        return;
    }

    verification->updateMd5Checksum(md5_hex_result);
    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText("Checksum of image: " + md5_hex_result);
    verification->getBurnerWidgets()->progress_bar_burn->setValue(0);

    verification->changeState(QSharedPointer<DvdVerificationState>(new DvdVerificationState(verification)));
    verification->prepareWidgetsBeforeCalculations();
}
