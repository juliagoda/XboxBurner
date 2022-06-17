/***************************************************************************
 *   Copyright (C) 2011 - 2012 Kai Heitkamp                                *
 *   dynup@ymail.com | dynup.de.vu                                         *
 *   Copyright (C) 2022 Jagoda "juliagoda" Górska                          *
 *   juliagoda.pl@protonmail.com                                           *
 *
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

#include "burner/burnerprogressbarssetup.h"
#include "burner/burnerstartstage.h"
#include "burner/burningacceptancestage.h"
#include "burner/currentburningverificationstage.h"
#include "burner/pathsandspeedcontentverificationstage.h"
#include "burner/truncateimagestartstage.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"
#include "dependencies/dvdrwtoolstooldependencyfactory.h"

#include <QAction>
#include <QCompleter>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QtConcurrent>
#include <QtGlobal>


XBoxBurner::XBoxBurner(const ApplicationInformations& new_applications_informations,
                       QWidget* parent)
    : QMainWindow(parent)
    , ui { new Ui::XBoxBurner }
    , backup { new Backup(createStructFromBurnerWidgets()) }
    , burner_widgets { createStructFromBurnerWidgets() }
    , applications_informations { new_applications_informations }
    , verification { new Verification(burner_widgets) }
    , start_burner_stage { createBurnerSteps() }
    , imageMd5sum { QString("") }
    , dvdMd5sum { QString("") }
    , md5ProgressMax { 0 }
{
    ui->setupUi(this);
}

QSharedPointer<BurnerWidgets> XBoxBurner::createStructFromBurnerWidgets()
{
    QSharedPointer<BurnerWidgets> struct_burner_widgets = QSharedPointer<BurnerWidgets>(new BurnerWidgets);
    struct_burner_widgets.data()->combo_box_dvd_format = ui->combo_box_dvd_format;
    struct_burner_widgets.data()->check_box_dao_mode = ui->check_box_dao_mode;
    struct_burner_widgets.data()->check_box_dvd_compat = ui->check_box_dvd_compat;
    struct_burner_widgets.data()->check_box_dry_run = ui->check_box_dry_run;
    struct_burner_widgets.data()->check_box_data_verification = ui->check_box_data_verification;
    struct_burner_widgets.data()->combo_box_write_speed = ui->combo_box_write_speed;
    struct_burner_widgets.data()->lineedit_burner_path = ui->lineedit_burner_path;
    struct_burner_widgets.data()->lineedit_image_path = ui->lineedit_image_path;
    struct_burner_widgets.data()->plain_text_edit_with_logs = ui->plain_text_edit_with_logs;
    struct_burner_widgets.data()->toolbar = ui->toolBar;
    struct_burner_widgets.data()->progress_bar_burn = ui->progress_bar_burn;
    struct_burner_widgets.data()->progress_bar_ring_buffer_unit = ui->progress_bar_ring_buffer_unit;
    struct_burner_widgets.data()->progress_bar_unit_buffer_unit = ui->progress_bar_unit_buffer_unit;
    struct_burner_widgets.data()->backup_future_watcher = &backup_future_watcher;
    struct_burner_widgets.data()->image_future_watcher = &image_future_watcher;
    struct_burner_widgets.data()->dvd_future_watcher = &dvd_future_watcher;
    struct_burner_widgets.data()->check_box_backup_creation = ui->check_box_backup_creation;
    struct_burner_widgets.data()->status_bar = ui->statusBar;
    struct_burner_widgets.data()->status_frame = ui->frame_1;
    return struct_burner_widgets;
}

QPointer<CurrentBurningVerificationStage> XBoxBurner::createBurnerSteps()
{
    QPointer<CurrentBurningVerificationStage> verification_stage = new CurrentBurningVerificationStage(burner_widgets);
    QPointer<PathsAndSpeedContentVerificationStage> paths_and_speed_content_verification_stage = new PathsAndSpeedContentVerificationStage(burner_widgets);
    QPointer<BurningAcceptanceStage> burning_acceptance_stage = new BurningAcceptanceStage(burner_widgets);
    QPointer<TruncateImageStartStage> truncate_image_start_stage = new TruncateImageStartStage(burner_widgets, backup);
    QPointer<BurnerStartStage> burner_start_stage = new BurnerStartStage(burner_widgets);

    QPointer<CurrentBurningVerificationStage> start_stage = verification_stage;
    verification_stage->next(paths_and_speed_content_verification_stage)->next(burning_acceptance_stage)->next(truncate_image_start_stage)->next(burner_start_stage);
    return start_stage;
}

void XBoxBurner::initializeSettingsSave()
{
    settings->saveSettings();
}

void XBoxBurner::burn_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void XBoxBurner::log_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void XBoxBurner::on_push_button_open_image_path_clicked()
{
    QString path = QDir::homePath();

    if (!ui->lineedit_image_path->text().isEmpty())
        path = ui->lineedit_image_path->text().mid(0, ui->lineedit_image_path->text().lastIndexOf("/"));

    QString imagePath = QFileDialog::getOpenFileName(this, tr("Open dvd image"), path, tr("DVD images (*.iso *.img *.cdr)"));

    if (!imagePath.isEmpty())
    {
        ui->lineedit_image_path->setText(imagePath);
        if (imagePath.contains("xgd3", Qt::CaseInsensitive)) {
            ui->combo_box_dvd_format->setCurrentIndex(1);
        }
    }
}

void XBoxBurner::on_push_button_check_clicked()
{
    if (!ui->lineedit_burner_path->text().isEmpty())
    {
        showStatusBarMessage(Messages::reading_info);
        checkMediaProcess = new QProcess();

        qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
        connect(checkMediaProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getMediaInfo_readyReadStandardOutput()));
        connect(checkMediaProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(getMediaInfo_finished(int, QProcess::ExitStatus)));

        mediaInfo.clear();
        checkMediaProcess->start(dvdrwmediainfo, QStringList() << ui->lineedit_burner_path->text().simplified());
    }
}

void XBoxBurner::startBurn_triggered()
{
    start_burner_stage->handle();

    QObject::connect(start_burner_stage, &BurnerStage::finishedBurning, this, &XBoxBurner::verify_triggered);
}

void XBoxBurner::verify_triggered()
{
    verification->trigger();
}

void XBoxBurner::reset_triggered()
{
    if (BoxMessages::resetAllMessage(this) == QMessageBox::Ok)
    {
        if (start_burner_stage->isRunning())
            start_burner_stage->kill();

        if (verification->getCurrentState() != VerificationState::CurrentState::Cancelled)
            verification->cancel();

        ui->label_info->setStyleSheet("");
        ui->combo_box_write_speed->clear();
        ui->label_info->clear();
        ui->progress_bar_burn->setValue(0);
        ui->progress_bar_ring_buffer_unit->setValue(0);
        ui->progress_bar_unit_buffer_unit->setValue(0);

        showStatusBarMessage(tr("Ready."));
    }
}

void XBoxBurner::about_triggered()
{
    BoxMessages::aboutMessage(this);
}

void XBoxBurner::exit_triggered()
{
    qApp->quit();
}

void XBoxBurner::on_push_button_copy_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->plain_text_edit_with_logs->toPlainText());
}

void XBoxBurner::on_push_button_save_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save log file"), QDir::homePath().append("/XBoxBurner.log"), tr("XBoxBurner log file (*.log)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream textStream(&file);
            textStream << ui->plain_text_edit_with_logs->toPlainText();
        }

        file.close();
    }
}

void XBoxBurner::on_push_button_logs_reset_clicked()
{
    ui->plain_text_edit_with_logs->clear();
}

void XBoxBurner::on_combo_box_dvd_format_currentIndexChanged(int index)
{
    QString name = QString();

    switch (index)
    {
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

    if (index == 5)
    {
        bool ok;
        int layerBreak = QInputDialog::getInt(this, QCoreApplication::applicationName(), tr("Enter layer break:"), 0, 0, 4267015, 1, &ok, Qt::Dialog);

        if (ok && layerBreak > 0)
        {
            ui->combo_box_dvd_format->setItemText(5, tr("Special format (%1)").arg(QString::number(layerBreak)));
        }
        else
        {
            ui->combo_box_dvd_format->setItemText(5, tr("Special format (manual layer break)"));
            ui->combo_box_dvd_format->setCurrentIndex(0);
        }
    }
}

void XBoxBurner::getMediaInfo_readyReadStandardOutput()
{
    QString media_process_data = checkMediaProcess->readAllStandardOutput().data();
    ui->plain_text_edit_with_logs->appendPlainText(media_process_data);
    mediaInfo.append(media_process_data.split("\n"));
}

void XBoxBurner::getMediaInfo_finished(const int exitCode,
                                       const QProcess::ExitStatus exitStatus)
{
    QRegularExpression burner_line("INQUIRY:\\s*(.+)");
    QRegularExpression media_line("(?:MOUNTED MEDIA|MEDIA BOOK TYPE|MEDIA ID):\\s*(.+)");
    QRegularExpression speed_line("WRITE SPEED\\s*\\#\\d\\:\\s*((?:[0-9]|[,.])+)[xX].*");
    QString burner_txt;
    QStringList media_txt;

    ui->combo_box_write_speed->clear();
    ui->label_info->clear();

    if (exitCode >= 0 && exitStatus == 0)
    {
        for (int i = 0; i < mediaInfo.size(); i++)
        {
            QRegularExpressionMatch burner_line_match = burner_line.match(mediaInfo.at(i).simplified().toUpper());
            QRegularExpressionMatch media_line_match = media_line.match(mediaInfo.at(i).simplified().toUpper());
            QRegularExpressionMatch speed_line_match = speed_line.match(mediaInfo.at(i).simplified().toUpper());

            if (burner_line_match.hasMatch())
                burner_txt = burner_line_match.captured(0).simplified();
            else if (media_line_match.hasMatch())
                media_txt.append(media_line_match.captured(0).simplified().split(","));
            else if (speed_line_match.hasMatch())
                ui->combo_box_write_speed->addItem(QIcon(":/images/cdrom.png"), speed_line_match.captured(0).simplified());
        }

        if (burner_txt.simplified().isEmpty())
            burner_txt = tr("Not available!");

        if (media_txt.filter(QRegularExpression(".*\\S.*")).isEmpty())
            media_txt.append(tr("Not available!"));

        showStatusBarMessage(tr("Ready."));
    }
    else
    {
        QString errorMessage = Messages::burningErrorMessage(start_burner_stage->getProcess()->errorString(), exitCode, exitStatus);
        ui->plain_text_edit_with_logs->appendPlainText(errorMessage);
        showStatusBarMessage(errorMessage);
    }

    ui->label_info->setText(Messages::burnerMediaAvailability(burner_txt, media_txt));
    delete checkMediaProcess;
}

void XBoxBurner::showStatusBarMessage(const QString& text)
{
    ui->statusBar->showMessage(text, 0);
}

QMenu* XBoxBurner::createPopupMenu()
{
    QMenu* menu = QMainWindow::createPopupMenu();

    QAction* action_toolButtonIconOnly = new QAction(tr("Icon only"), menu);
    QAction* action_toolButtonTextOnly = new QAction(tr("Text only"), menu);
    QAction* action_toolButtonTextBesideIcon = new QAction(tr("Text beside icon"), menu);
    QAction* action_toolBarTextUnderIcon = new QAction(tr("Text under icon"), menu);

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

void XBoxBurner::toolBar_toolButtonIconOnly()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    // AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonIconOnly);
}

void XBoxBurner::toolBar_toolButtonTextOnly()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    // AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextOnly);
}

void XBoxBurner::toolBar_toolButtonTextBesideIcon()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    // AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextBesideIcon);
}

void XBoxBurner::toolBar_toolButtonTextUnderIcon()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //  AppSettings.setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextUnderIcon);
}

void XBoxBurner::keyReleaseEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->modifiers() == Qt::ControlModifier)
    {
        switch (keyEvent->key())
        {
        case Qt::Key_B:
            burn_triggered();
            break;
        case Qt::Key_L:
            log_triggered();
            break;
        case Qt::Key_S:
            startBurn_triggered();
            break;
        case Qt::Key_Y:
            verify_triggered();
            break;
        case Qt::Key_R:
            reset_triggered();
            break;
        case Qt::Key_Q:
            exit_triggered();
            break;
        }
    }
}

bool XBoxBurner::event(QEvent* event)
{
    if (event->type() == QEvent::StatusTip)
        return false;

    if (event->type() == QEvent::Close)
        initializeSettingsSave();

    return QMainWindow::event(event);
}

XBoxBurner::~XBoxBurner()
{
    delete ui;
}
