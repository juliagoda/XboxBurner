/***************************************************************************
 *   Copyright (C) 2011 - 2012 Kai Heitkamp                                *
 *   dynup@ymail.com | dynup.de.vu                                         *
 *                                                                         *
 *   Based on qisoburn, Copyright (C) 2010 mycelo                          *
 *   http://code.google.com/p/qisoburn                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, go to http://www.gnu.org             *
 ***************************************************************************/

#include "xboxburner.h"
#include "ui_xboxburner.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QtConcurrent>
#include <QAction>
#include <QMenu>
#include <QCompleter>
#include <QFileSystemModel>

XBoxBurner::XBoxBurner(QWidget *parent) : QMainWindow(parent), ui(new Ui::XBoxBurner) {
    ui->setupUi(this);

    burning = false;
    verifying = false;
    cancel = false;

    blockSize = 2048;
    md5ProgressMax = 0;
    imageMd5sum = "";
    dvdMd5sum = "";

    connect(&backupFutureWatcher, SIGNAL(finished()), this, SLOT(createBackup_finished()));
    connect(&imageFutureWatcher, SIGNAL(finished()), this, SLOT(calculatingImageMD5_finished()));
    connect(&dvdFutureWatcher, SIGNAL(finished()), this, SLOT(calculatingDvdMD5_finished()));
    connect(this, SIGNAL(md5SumMaximum(qint64)), this, SLOT(calculatingMD5_setMaximum(qint64)));
    connect(this, SIGNAL(md5SumProgress(qint64)), this, SLOT(calculatingMD5_progress(qint64)));

    QCompleter *imagePathCompleter = new QCompleter(this);
    QFileSystemModel* cd_format_model = new QFileSystemModel(imagePathCompleter);
    cd_format_model->setNameFilters(QStringList() << "*.iso" << "*.img" << "*.cdr");
    cd_format_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    imagePathCompleter->setModel(cd_format_model);
    imagePathCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    imagePathCompleter->setMaxVisibleItems(12);
    ui->lineEdit_imagePath->setCompleter(imagePathCompleter);

    QCompleter *burnerPathCompleter = new QCompleter(this);
    QFileSystemModel* buner_paths_model = new QFileSystemModel(burnerPathCompleter);
    cd_format_model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    burnerPathCompleter->setModel(buner_paths_model);
    burnerPathCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    burnerPathCompleter->setMaxVisibleItems(12);
    ui->lineEdit_burnerPath->setCompleter(burnerPathCompleter);

    QFont font = ui->label_info->font();
    font.setBold(true);
    ui->label_info->setFont(font);

    loadSettings();
    setWindowTitle("XBoxBurner " + QCoreApplication::applicationVersion());
    showStatusBarMessage(tr("Ready."));

    checkTools();
}

void XBoxBurner::loadSettings() {
    if (AppSettings.contains("MainWindow/x")) {
        QRect rect;
        rect.setX(AppSettings.value("MainWindow/x", QVariant(0)).toInt());
        rect.setY(AppSettings.value("MainWindow/y", QVariant(0)).toInt());
        rect.setWidth(AppSettings.value("MainWindow/width", QVariant(550)).toInt());
        rect.setHeight(AppSettings.value("MainWindow/height", QVariant(400)).toInt());

        this->setGeometry(rect);
    }

    ui->toolBar->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(AppSettings.value("MainWindow/ToolButtonStyle", 2).toInt()));

    ui->lineEdit_imagePath->setText(AppSettings.value("ImagePath", QVariant("")).toString());
    ui->lineEdit_burnerPath->setText(AppSettings.value("BurnerPath", QVariant("/dev/cdrom")).toString());

    ui->comboBox_dvdFormat->setCurrentIndex(AppSettings.value("Format", QVariant(0)).toInt());

    ui->checkBox_dao->setChecked(AppSettings.value("DAO", QVariant(true)).toBool());
    ui->checkBox_dvdCompat->setChecked(AppSettings.value("DVDCompat", QVariant(true)).toBool());
    ui->checkBox_truncateBackup->setChecked(AppSettings.value("TruncateBackup", QVariant(true)).toBool());

    #ifdef Q_OS_WIN
        ui->toolBar->actions().at(4)->setEnabled(false);
        ui->checkBox_verify->setChecked(false);
        ui->checkBox_verify->setEnabled(false);
    #else
        ui->checkBox_verify->setChecked(AppSettings.value("Verify", QVariant(true)).toBool());
    #endif

    ui->checkBox_dryRun->setChecked(AppSettings.value("DryRun", QVariant(false)).toBool());
}

void XBoxBurner::saveSettings() {
    AppSettings.setValue("MainWindow/x", this->geometry().x());
    AppSettings.setValue("MainWindow/y", this->geometry().y());
    AppSettings.setValue("MainWindow/width", this->geometry().width());
    AppSettings.setValue("MainWindow/height", this->geometry().height());

    AppSettings.setValue("ImagePath", ui->lineEdit_imagePath->text());
    AppSettings.setValue("BurnerPath", ui->lineEdit_burnerPath->text());

    AppSettings.setValue("Format", ui->comboBox_dvdFormat->currentIndex());

    AppSettings.setValue("DAO", ui->checkBox_dao->checkState());
    AppSettings.setValue("DVDCompat", ui->checkBox_dvdCompat->checkState());
    AppSettings.setValue("TruncateBackup", ui->checkBox_truncateBackup->checkState());
    AppSettings.setValue("Verify", ui->checkBox_verify->checkState());
    AppSettings.setValue("DryRun", ui->checkBox_dryRun->checkState());
}

void XBoxBurner::burn_triggered() {
    ui->stackedWidget->setCurrentIndex(0);
}

void XBoxBurner::log_triggered() {
    ui->stackedWidget->setCurrentIndex(1);
}

void XBoxBurner::on_pushButton_openImagePath_clicked() {
    QString path = QDir::homePath();

    if (!ui->lineEdit_imagePath->text().isEmpty()) {
        path = ui->lineEdit_imagePath->text().mid(0, ui->lineEdit_imagePath->text().lastIndexOf("/"));
    }

    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open dvd image"), path, tr("DVD images (*.iso *.img *.cdr)"));

    if (!imagePath.isEmpty()) {
        ui->lineEdit_imagePath->setText(imagePath);

        if (imagePath.contains("xgd3", Qt::CaseInsensitive)) {
            ui->comboBox_dvdFormat->setCurrentIndex(1);
        }
    }
}

void XBoxBurner::on_pushButton_check_clicked() {
    if (!ui->lineEdit_burnerPath->text().isEmpty()) {
        showStatusBarMessage(tr("Reading burner and media info..."));

        checkMediaProcess = new QProcess();

        qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
        connect(checkMediaProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getMediaInfo_readyReadStandardOutput()));
        connect(checkMediaProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(getMediaInfo_finished(int, QProcess::ExitStatus)));

        mediaInfo.clear();

        checkMediaProcess->start(dvdrwmediainfo, QStringList() << ui->lineEdit_burnerPath->text().simplified());
    }
}

void XBoxBurner::startBurn_triggered() {
    if (burning) {
        if (QMessageBox::question(this, tr("Cancel burn process"), tr("Are you sure that you want to cancel the burn process?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok) {
            burnProcess->kill();
        }
    }
    else {
        if (!ui->lineEdit_imagePath->text().isEmpty() && !ui->lineEdit_burnerPath->text().isEmpty() && !ui->comboBox_writeSpeed->currentText().isEmpty()) {
            if (QMessageBox::question(this, tr("Start burn process"), tr("Are you sure that you want to burn the image?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok) {
                ui->plainTextEdit_log->appendPlainText("Burn process started at " + QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate));

                ui->toolBar->actions().at(3)->setIcon(QIcon(":/images/cancel.png"));
                ui->toolBar->actions().at(3)->setText(tr("&Cancel"));

                ui->progressBar_burn->setValue(0);
                ui->progressBar_rbu->setValue(0);
                ui->progressBar_ubu->setValue(0);

                if (ui->comboBox_dvdFormat->currentIndex() == 3) {
                    showStatusBarMessage(tr("Truncating XGD3 image..."));
                    truncateImage();
                }
                else {
                    startBurnProcess();
                }
            }
        }
    }
}

void XBoxBurner::verify_triggered() {
    if (verifying) {
        cancel = true;
    }
    else {
        if (!ui->lineEdit_imagePath->text().isEmpty() && !ui->lineEdit_burnerPath->text().isEmpty()) {
            verify();
        }
    }
}

void XBoxBurner::reset_triggered() {
    if (QMessageBox::question(this, tr("Reset all"), tr("Are you sure to reset all?\n\nCaution:\nRunning burn process will canceled!"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok) {
        if (burning) {
            burnProcess->kill();
        }

        burning = false;
        verifying = false;
        cancel = false;
        md5ProgressMax = 0;
        imageMd5sum.clear();
        dvdMd5sum.clear();

        ui->label_info->setStyleSheet("");

        ui->comboBox_writeSpeed->clear();

        ui->label_info->clear();

        ui->progressBar_burn->setValue(0);
        ui->progressBar_rbu->setValue(0);
        ui->progressBar_ubu->setValue(0);

        showStatusBarMessage(tr("Ready."));
    }
}

void XBoxBurner::about_triggered() {
    QMessageBox::about(this, tr("About XBoxBurner"),
        QString("<h2>XBoxBurner %1</h2>").arg(QCoreApplication::applicationVersion()) +
        "<p><b><i>Easy to use burner for XBox and XBox360 game images</b></i>"
        "<p>Development and Copyright &copy; 2011 - 2012 Kai Heitkamp"
        "<br>Based on <a href='http://code.google.com/p/qisoburn'>qisoburn</a>, Copyright &copy; 2010 mycelo"
        "<p><a href='mailto:dynup<dynup@ymail.com>?subject=XBoxBurner%20feedback'>dynup@ymail.com</a>"
        " | <a href='dynup.de.vu'>dynup.de.vu</a>"
        "<p>XBoxBurner needs <i>dvd+rw-mediainfo</i> and <i>growisofs</i> to run! Both are parts of the <a href='http://fy.chalmers.se/~appro/linux/DVD+RW/'>dvd+rw-tools</a>! "
        "They are available for <a href='http://fy.chalmers.se/~appro/linux/DVD+RW/tools/?M=D'>Linux</a> and <a href='http://fy.chalmers.se/~appro/linux/DVD+RW/tools/win32/'>Windows</a>, "
        "for MacOSX via <a href='http://finkproject.org'>Fink</a> or <a href='http://www.macports.org'>MacPorts</a>! The tools must be in PATH or in the same directory of XBoxBurner!"
        "<p>Big thanks to the trolls at Trolltech Norway for his excellent Qt toolkit, the guys at Nokia for the continuation and all people at the Qt Project for the open source development of Qt!"
        "<p>" + QCoreApplication::applicationName() + " is licensed under the GNU General Public License v3 (<a href='http://www.gnu.org/licenses/gpl-3.0.txt'>GPLv3</a>)."
        "<p><font color='red'>I don't support piracy! If you copy games with this software, you must have the original and it's for your private use only!</font color>");
}

void XBoxBurner::exit_triggered() {
    qApp->quit();
}

void XBoxBurner::on_pushButton_copy_clicked() {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->plainTextEdit_log->toPlainText());
}

void XBoxBurner::on_pushButton_save_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save log file"), QDir::homePath().append("/XBoxBurner.log"), tr("XBoxBurner log file (*.log)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream textStream(&file);
            textStream << ui->plainTextEdit_log->toPlainText();
        }

        file.close();
    }
}

void XBoxBurner::on_pushButton_logReset_clicked() {
    ui->plainTextEdit_log->clear();
}

void XBoxBurner::on_comboBox_dvdFormat_currentIndexChanged(int index) {
    QString name;

    switch (index) {
        case 0:
        case 1:
            name = ":/images/xbox.png";
            break;
        case 2:
        case 3:
        case 4:
            name = ":/images/xbox360.png";
            break;
        case 5:
            name = ":/images/dvdr.png";
            break;
    }

    ui->label_xbox->setPixmap(QPixmap::fromImage(QImage(name)));

    if (index == 5) {
        bool ok;
        int layerBreak = QInputDialog::getInt(this, QCoreApplication::applicationName(), tr("Enter layer break:"), 0, 0, 4267015, 1, &ok, Qt::Dialog);

        if (ok && layerBreak > 0) {
            ui->comboBox_dvdFormat->setItemText(5, tr("Special format (%1)").arg(QString::number(layerBreak)));
        }
        else {
            ui->comboBox_dvdFormat->setItemText(5, tr("Special format (manual layer break)"));
            ui->comboBox_dvdFormat->setCurrentIndex(0);
        }
    }
}

void XBoxBurner::getMediaInfo_readyReadStandardOutput() {
    QString data = checkMediaProcess->readAllStandardOutput().data();

    ui->plainTextEdit_log->appendPlainText(data);
    mediaInfo.append(data.split("\n"));
}

void XBoxBurner::getMediaInfo_finished(const int exitCode, const QProcess::ExitStatus exitStatus) {
    QRegExp burner_line("INQUIRY:\\s*(.+)");
    QRegExp media_line("(?:MOUNTED MEDIA|MEDIA BOOK TYPE|MEDIA ID):\\s*(.+)");
    QRegExp speed_line("WRITE SPEED\\s*\\#\\d\\:\\s*((?:[0-9]|[,.])+)[xX].*");
    QString burner_txt;
    QStringList media_txt;

    ui->comboBox_writeSpeed->clear();
    ui->label_info->clear();

    if (exitCode >= 0 && exitStatus == 0) {
        for (int i = 0; i < mediaInfo.size(); i++) {
            if (burner_line.exactMatch(mediaInfo.at(i).simplified().toUpper())) {
                    burner_txt = burner_line.capturedTexts().at(1).simplified();
            }
            else if (media_line.exactMatch(mediaInfo.at(i).simplified().toUpper())) {
                    media_txt.append(media_line.capturedTexts().at(1).simplified().split(","));
            }
            else if (speed_line.exactMatch(mediaInfo.at(i).simplified().toUpper())) {
                ui->comboBox_writeSpeed->addItem(QIcon(":/images/cdrom.png"), speed_line.capturedTexts().at(1).simplified());
            }
        }

        if (burner_txt.simplified().isEmpty()) {
            burner_txt = tr("Not available!");
        }

        if (media_txt.filter(QRegExp(".*\\S.*")).isEmpty()) {
            media_txt.append(tr("Not available!"));
        }

        showStatusBarMessage(tr("Ready."));
    }
    else {
        QString errorMessage = tr("Error: ") + burnProcess->errorString() + " (" + QString::number(exitCode) + ", " + QString::number(exitStatus) + ")";

        ui->plainTextEdit_log->appendPlainText(errorMessage);
        showStatusBarMessage(errorMessage);
    }

    ui->label_info->setText(tr("Burner: <font color=grey>%1</font><br>Media: <font color=grey>%2</font>").arg(burner_txt).arg(media_txt.filter(QRegExp(".*\\S.*")).join(",").simplified().replace(", ", ",")));

    delete checkMediaProcess;
}

void XBoxBurner::startBurnProcess() {
    showStatusBarMessage(tr("Starting burn process..."));

    burning = true;

    burnProcess = new QProcess();

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    connect(burnProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(burnProcess_readyReadStandardOutput()));
    connect(burnProcess, SIGNAL(readyReadStandardError()), this, SLOT(burnProcess_readyReadStandardOutput()));
    connect(burnProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(burnProcess_finished(int, QProcess::ExitStatus)));

    QStringList arguments;
    QString layerbreak = layerBreak();

    if (!layerbreak.isEmpty()) {
        arguments.append("-use-the-force-luke=break:" + layerbreak);
    }

    if (ui->comboBox_dvdFormat->currentIndex() != 4) {
        if (ui->checkBox_dao->isChecked()) {
            arguments.append("-use-the-force-luke=dao");
        }

        if (ui->checkBox_dvdCompat->isChecked()) {
            arguments.append("-dvd-compat");
        }
    }

    if (ui->checkBox_dryRun->isChecked()) {
        arguments.append("-dry-run");
    }

    arguments.append("-speed=" + ui->comboBox_writeSpeed->currentText().simplified());
    arguments.append("-Z");
    arguments.append(ui->lineEdit_burnerPath->text().simplified() + "=" + ui->lineEdit_imagePath->text().simplified());

    burnProcess->setProcessChannelMode(QProcess::MergedChannels);
    burnProcess->start(growisofs, arguments);
}

QString XBoxBurner::layerBreak() {
    QString result("");

    switch (ui->comboBox_dvdFormat->currentIndex()) {
        case 0:
            result = "";
            break;
        case 1:
            result = "1913776";
            break;
        case 2:
            result = "1913760";
            break;
        case 3:
            result = "2086912";
            break;
        case 4:
            result = "2133520";
            break;
        case 5:
            QString tmp = ui->comboBox_dvdFormat->itemText(5);
            result = tmp.mid(tmp.indexOf("(") + 1, tmp.indexOf(")") - tmp.indexOf("(") - 1);
            break;
    }

    return result;
}

void XBoxBurner::truncateImage() {
    qint64 size = Q_INT64_C(8547991552);
    QString fileName = ui->lineEdit_imagePath->text().simplified();
    QFile file(fileName);

    if (file.size() > size && ui->checkBox_truncateBackup->isChecked()) {
        showStatusBarMessage(tr("Creating backup of game image..."));
        ui->plainTextEdit_log->appendPlainText("Creating backup of game image...");

        startBusy();

        QFuture<bool> backupFuture = QtConcurrent::run(this, &XBoxBurner::createBackup);
        backupFutureWatcher.setFuture(backupFuture);
    }
    else {
        startBurnProcess();
    }
}

bool XBoxBurner::createBackup() {
    QString fileName = ui->lineEdit_imagePath->text().simplified();
    QString source = fileName;
    QString destination = fileName.mid(0, fileName.lastIndexOf(".")) + "_backup" + fileName.mid(fileName.lastIndexOf("."), fileName.length() - fileName.lastIndexOf("."));

    return QFile::copy(source, destination);
}

void XBoxBurner::createBackup_finished() {
    bool result = backupFutureWatcher.result();

    stopBusy();

    if (result) {
        showStatusBarMessage(tr("Creation backup of game image successfully!"));
        ui->plainTextEdit_log->appendPlainText("Creation backup of game image successfully!");

        resizeImage();
    }
    else {
        showStatusBarMessage(tr("Creation backup of game image failed!"));
        ui->plainTextEdit_log->appendPlainText("Creation backup of game image failed! Burn process stopped!");
    }
}

void XBoxBurner::resizeImage() {
    qint64 size = Q_INT64_C(8547991552);
    QFile file(ui->lineEdit_imagePath->text().simplified());

    if (file.resize(size)) {
        startBurnProcess();
    }
    else {
        showStatusBarMessage(tr("Resize game image failed!"));
        ui->plainTextEdit_log->appendPlainText("Resize game image failed! Burn process stopped!");
    }
}

void XBoxBurner::burnProcess_readyReadStandardOutput() {
    int burnProgress, rbuProgress, ubuProgress;
    QString growisofsData;
    QRegExp progressLine("\\d+/\\d+\\s*\\(\\s*((?:[0-9]|[.,])+)\\%\\s*\\)\\s*@((?:[0-9]|[.,])+)[xX],\\s+\\w+\\s+((?:[0-9]|[:?])+)\\s+\\w+\\s+((?:[0-9]|[.,])+)\\%\\s+\\w+\\s+((?:[0-9]|[.,])+).*");

    while (burnProcess->canReadLine()) {
        growisofsData = burnProcess->readAll().simplified().data();

        if (!growisofsData.isEmpty()) {
            ui->plainTextEdit_log->appendPlainText(growisofsData);
        }

        if (growisofsData.contains("flushing cache")) {
            ui->progressBar_rbu->setValue(0);
            ui->progressBar_ubu->setValue(0);

            startBusy(true);

            showStatusBarMessage(tr("Fluching cache..."));
        }
        else if (growisofsData.contains("closing track")) {
            showStatusBarMessage(tr("Closing track..."));
        }
        else if (growisofsData.contains("closing disc")) {
            showStatusBarMessage(tr("Closing disc..."));
        }
        else if (growisofsData.contains("reloading tray")) {
            showStatusBarMessage(tr("Reloading tray..."));
        }
        else if (!growisofsData.isEmpty()) {
            if (progressLine.exactMatch(growisofsData)) {
                burnProgress = progressLine.capturedTexts().at(1).simplified().toFloat();
                rbuProgress = progressLine.capturedTexts().at(4).simplified().toFloat();
                ubuProgress = progressLine.capturedTexts().at(5).simplified().toFloat();

                if (burnProgress > 0 && burnProgress <= 100) {
                    ui->progressBar_burn->setValue(burnProgress);
                }

                if (rbuProgress > 0 && rbuProgress <= 100) {
                    ui->progressBar_rbu->setValue(rbuProgress);
                }

                if (ubuProgress > 0 && ubuProgress <= 100) {
                    ui->progressBar_ubu->setValue(ubuProgress);
                }

                const QString image = ui->lineEdit_imagePath->text().simplified().mid(ui->lineEdit_imagePath->text().simplified().lastIndexOf("/") + 1, ui->lineEdit_imagePath->text().simplified().length() - ui->lineEdit_imagePath->text().simplified().lastIndexOf("/"));
                const QString drive = ui->lineEdit_burnerPath->text().simplified();
                const QString speed = progressLine.capturedTexts().at(2).simplified();
                const QString eta = progressLine.capturedTexts().at(3).simplified();

                if (eta == "??:??") {
                    showStatusBarMessage(tr("Preparing burn process..."));
                }
                else {
                    showStatusBarMessage(tr("Burning '%1' on %2 at %3x -> ETA %4").arg(image, drive, speed, eta));
                }
            }
        }
    }
}

void XBoxBurner::burnProcess_finished(const int exitCode, const QProcess::ExitStatus exitStatus) {
    stopBusy(true);

    if (exitCode == 0 && exitStatus == 1) {
        ui->progressBar_burn->setValue(0);
        ui->progressBar_rbu->setValue(0);
        ui->progressBar_ubu->setValue(0);

        showStatusBarMessage(tr("Burn process canceled."));
    }
    else if (exitCode == 0 && exitStatus == 0) {
        ui->progressBar_burn->setValue(100);
        showStatusBarMessage(tr("Burn process successfully."));
    }
    else {
        QString errorMessage = tr("Error: ") + burnProcess->errorString() + " (" + QString::number(exitCode) + ", " + QString::number(exitStatus) + ")";

        ui->plainTextEdit_log->appendPlainText(errorMessage);
        showStatusBarMessage(errorMessage);
    }

    ui->toolBar->actions().at(3)->setIcon(QIcon(":/images/burn.png"));
    ui->toolBar->actions().at(3)->setText(tr("&Burn"));

    burning = false;

    delete burnProcess;

    if (ui->checkBox_verify->isChecked()) {
        verify();
    }
}

void XBoxBurner::verify() {
    showStatusBarMessage(tr("Calculating checksum of image..."));
    ui->plainTextEdit_log->appendPlainText("Calculating checksum of image...");
    ui->toolBar->actions().at(4)->setIcon(QIcon(":/images/cancel.png"));
    ui->toolBar->actions().at(4)->setText(tr("&Cancel"));

    startBusy();

    if (imageMd5sum.isEmpty()) {
        QFuture<QString> imageFuture = QtConcurrent::run(this, &XBoxBurner::calculatingImageMD5);
        imageFutureWatcher.setFuture(imageFuture);
    }
    else {
        showStatusBarMessage(tr("Calculating checksum of DVD..."));
        ui->plainTextEdit_log->appendPlainText("Checksum of image already exists.");
        ui->plainTextEdit_log->appendPlainText("Calculating checksum of DVD...");

        ui->progressBar_burn->setValue(0);

        QFuture<QString> dvdFuture = QtConcurrent::run(this, &XBoxBurner::calculatingDvdMD5);
        dvdFutureWatcher.setFuture(dvdFuture);
    }
}

void XBoxBurner::calculatingMD5_setMaximum(qint64 max) {
    md5ProgressMax = max;
}

void XBoxBurner::calculatingMD5_progress(qint64 value) {
    ui->progressBar_burn->setValue(value * 100 / md5ProgressMax);
}

QString XBoxBurner::calculatingImageMD5() {
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile image(ui->lineEdit_imagePath->text().simplified());
    int blockCount = image.size() / blockSize;

    verifying = true;

    emit md5SumMaximum(blockCount);

    if (image.open(QFile::ReadOnly)) {
        for (int block = 0; block < blockCount; block++) {
            hash.addData(image.read(blockSize));

            emit md5SumProgress(block + 1);

            if (cancel) {
                break;
            }
        }

        image.close();
        return hash.result().toHex();
    }
    else {
        return "";
    }
}

void XBoxBurner::calculatingImageMD5_finished() {
    verifying = false;

    if (cancel) {
        ui->toolBar->actions().at(4)->setIcon(QIcon(":/images/verify.png"));
        ui->toolBar->actions().at(4)->setText(tr("&Verify"));

        showStatusBarMessage(tr("Calculation of image checksum canceled!"));
        ui->plainTextEdit_log->appendPlainText("Calculation of image checksum canceled!");

        stopBusy();

        cancel = false;

        return;
    }

    QString result = imageFutureWatcher.result();

    if (result.isEmpty()) {
        showStatusBarMessage(tr("Calculation of image checksum failed!"));
        ui->plainTextEdit_log->appendPlainText("Calculation of image checksum failed!");
    }
    else {
        imageMd5sum = result;
        ui->plainTextEdit_log->appendPlainText("Checksum of image: " + result);
        showStatusBarMessage(tr("Calculating checksum of DVD..."));
        ui->plainTextEdit_log->appendPlainText("Calculating checksum of DVD...");

        ui->progressBar_burn->setValue(0);

        QFuture<QString> dvdFuture = QtConcurrent::run(this, &XBoxBurner::calculatingDvdMD5);
        dvdFutureWatcher.setFuture(dvdFuture);
    }
}

QString XBoxBurner::calculatingDvdMD5() {
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile device(ui->lineEdit_burnerPath->text().simplified());
    QFile image(ui->lineEdit_imagePath->text().simplified());
    int blockCount = image.size() / blockSize;

    verifying = true;

    emit md5SumMaximum(blockCount);

    if (device.open(QIODevice::ReadOnly)) {
        char buf[blockSize];

        for (int block = 0; block < blockCount; block++) {
            device.read(buf, blockSize);
            hash.addData(buf, blockSize);

            emit md5SumProgress(block + 1);

            if (cancel) {
                break;
            }
        }

        device.close();
        return hash.result().toHex();
    }
    else {
        return "";
    }
}

void XBoxBurner::calculatingDvdMD5_finished() {
    verifying = false;

    ui->toolBar->actions().at(4)->setIcon(QIcon(":/images/verify.png"));
    ui->toolBar->actions().at(4)->setText(tr("&Verify"));

    stopBusy();

    if (cancel) {
        showStatusBarMessage(tr("Calculation of DVD checksum canceled!"));
        ui->plainTextEdit_log->appendPlainText("Calculation of DVD checksum canceled!");
        cancel = false;
    }
    else {
        QString result = dvdFutureWatcher.result();

        if (result.isEmpty()) {
            showStatusBarMessage(tr("Calculation of DVD checksum failed!"));
            ui->plainTextEdit_log->appendPlainText("Calculation of DVD checksum failed!");
        }
        else {
            dvdMd5sum = result;
            ui->plainTextEdit_log->appendPlainText("Checksum of DVD: " + result);

            if (imageMd5sum == dvdMd5sum) {
                ui->frame_1->setStyleSheet("QFrame { background-color : green; }");

                showStatusBarMessage(tr("Verification successfully!"));
                ui->plainTextEdit_log->appendPlainText("Verification successfully!");
            }
            else {
                ui->frame_1->setStyleSheet("QFrame { background-color : red; }");

                showStatusBarMessage(tr("Verification failed!"));
                ui->plainTextEdit_log->appendPlainText("Verification failed!");
            }

            imageMd5sum.clear();
            dvdMd5sum.clear();
        }
    }
}

void XBoxBurner::startBusy(const bool main) {
    if (main) {
        ui->progressBar_burn->setMinimum(0);
        ui->progressBar_burn->setMaximum(0);
        ui->progressBar_burn->setValue(-1);
    }
    else {
        ui->progressBar_rbu->setMinimum(0);
        ui->progressBar_rbu->setMaximum(0);
        ui->progressBar_rbu->setValue(-1);

        ui->progressBar_ubu->setMinimum(0);
        ui->progressBar_ubu->setMaximum(0);
        ui->progressBar_ubu->setValue(-1);
    }
}

void XBoxBurner::stopBusy(const bool main) {
    if (main) {
        ui->progressBar_burn->setMinimum(0);
        ui->progressBar_burn->setMaximum(100);
        ui->progressBar_burn->setValue(0);
    }
    else {
        ui->progressBar_rbu->setMinimum(0);
        ui->progressBar_rbu->setMaximum(100);
        ui->progressBar_rbu->setValue(0);

        ui->progressBar_ubu->setMinimum(0);
        ui->progressBar_ubu->setMaximum(100);
        ui->progressBar_ubu->setValue(0);
    }
}

void XBoxBurner::showStatusBarMessage(const QString &text) {
    ui->statusBar->showMessage(text, 0);
}

void XBoxBurner::checkTools() {
    ui->plainTextEdit_log->appendPlainText(tr("(%1) XBoxBurner %2 started.").arg(QDateTime::currentDateTime().toString(), QCoreApplication::applicationVersion()));

    #ifdef Q_OS_WIN
        QString dvdrwtools_dvdrwmediainfo = "dvdrwtools:dvd+rw-mediainfo.exe";
        QString dvdrwtools_growisofs = "dvdrwtools:growisofs.exe";
    #else
        QString dvdrwtools_dvdrwmediainfo = "dvdrwtools:dvd+rw-mediainfo";
        QString dvdrwtools_growisofs = "dvdrwtools:growisofs";
    #endif

    QDir::setSearchPaths("dvdrwtools", QStringList() << QDir::currentPath() << QDir::currentPath() + "/XBoxBurner.app/Contents/MacOS" << QString(getenv("PATH")).split(":"));
    QFile dvdrwmediainfo_file(dvdrwtools_dvdrwmediainfo);
    QFile growisofs_file(dvdrwtools_growisofs);

    if (!dvdrwmediainfo_file.exists()) {
        dvdrwmediainfo = "";
        ui->plainTextEdit_log->appendPlainText(tr("Error: dvd+rw-mediainfo not found!"));
        showStatusBarMessage(tr("Error: dvd+rw-mediainfo not found!"));
    }
    else {
        dvdrwmediainfo = dvdrwmediainfo_file.fileName();
        ui->plainTextEdit_log->appendPlainText(tr("Info: dvd+rw-mediainfo found in %1").arg(dvdrwmediainfo.mid(0, dvdrwmediainfo.lastIndexOf("/"))));
    }

    if (!growisofs_file.exists()) {
        growisofs = "";
        ui->plainTextEdit_log->appendPlainText(tr("Error: growisofs not found!"));
        showStatusBarMessage(tr("Error: growisofs not found!"));
    }
    else {
        growisofs = growisofs_file.fileName();
        ui->plainTextEdit_log->appendPlainText(tr("Info: growisofs found in %1").arg(growisofs.mid(0, growisofs.lastIndexOf("/"))));
        ui->plainTextEdit_log->appendPlainText(growisofsVersion());
    }
}

QString XBoxBurner::growisofsVersion() {
    QProcess growisofsProcess;
    growisofsProcess.start(growisofs, QStringList() << "-version");

    growisofsProcess.waitForFinished(-1);

    return QString(growisofsProcess.readAll());
}

QMenu* XBoxBurner::createPopupMenu() {
    QMenu *menu = QMainWindow::createPopupMenu();

    QAction *action_toolButtonIconOnly = new QAction(tr("Icon only"), menu);
    QAction *action_toolButtonTextOnly = new QAction(tr("Text only"), menu);
    QAction *action_toolButtonTextBesideIcon = new QAction(tr("Text beside icon"), menu);
    QAction *action_toolBarTextUnderIcon = new QAction(tr("Text under icon"), menu);

    connect(action_toolButtonIconOnly, SIGNAL(triggered()), this, SLOT(toolBar_toolButtonIconOnly()));
    connect(action_toolButtonTextOnly, SIGNAL(triggered()), this, SLOT(toolBar_toolButtonTextOnly()));
    connect(action_toolButtonTextBesideIcon, SIGNAL(triggered()), this, SLOT(toolBar_toolButtonTextBesideIcon()));
    connect(action_toolBarTextUnderIcon, SIGNAL(triggered()), this, SLOT(toolBar_toolButtonTextUnderIcon()));

    menu->addSeparator();
    menu->addAction(action_toolButtonIconOnly);
    menu->addAction(action_toolButtonTextOnly);
    menu->addAction(action_toolButtonTextBesideIcon);
    menu->addAction(action_toolBarTextUnderIcon);

    return menu;
}

void XBoxBurner::toolBar_toolButtonIconOnly() {
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonIconOnly);
}

void XBoxBurner::toolBar_toolButtonTextOnly() {
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextOnly);
}

void XBoxBurner::toolBar_toolButtonTextBesideIcon() {
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextBesideIcon);
}

void XBoxBurner::toolBar_toolButtonTextUnderIcon() {
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextUnderIcon);
}

void XBoxBurner::keyReleaseEvent(QKeyEvent *keyEvent) {
    if (keyEvent->modifiers() == Qt::ControlModifier) {
        switch (keyEvent->key()) {
            case Qt::Key_B: burn_triggered();       break;
            case Qt::Key_L: log_triggered();        break;
            case Qt::Key_S: startBurn_triggered();  break;
            case Qt::Key_Y: verify_triggered();     break;
            case Qt::Key_R: reset_triggered();      break;
            case Qt::Key_Q: exit_triggered();       break;
        }
    }
}

bool XBoxBurner::event(QEvent *event) {
    if (event->type() == QEvent::StatusTip) {
        return false;
    }

    if (event->type() == QEvent::Close) {
        saveSettings();
    }

    return QMainWindow::event(event);
}

XBoxBurner::~XBoxBurner() {
    delete ui;
}
