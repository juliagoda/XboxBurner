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
#include "verification/burnerpathvalidation.h"

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
    , burner_path_validation { new BurnerPathValidation(burner_widgets, start_burner_stage) }
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
    struct_burner_widgets.data()->label_info = ui->label_info;
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

    QString image_path = QFileDialog::getOpenFileName(this, tr("Open dvd image"), path, tr("DVD images (*.iso *.img *.cdr)"));

    if (image_path.isEmpty())
        return;

    ui->lineedit_image_path->setText(image_path);

    if (image_path.contains("xgd3", Qt::CaseInsensitive))
        ui->combo_box_dvd_format->setCurrentIndex(1);
}

void XBoxBurner::on_push_button_check_clicked()
{
    burner_path_validation->start();
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
    if (BoxMessages::resetAllMessage(this) != QMessageBox::Ok)
       return;

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
    ui->statusBar->showMessage(tr("Ready."), 0);
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
    QPointer<QClipboard> clipboard = QApplication::clipboard();
    clipboard->setText(ui->plain_text_edit_with_logs->toPlainText());
}

void XBoxBurner::on_push_button_save_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save log file"), QDir::homePath().append("/XBoxBurner.log"), tr("XBoxBurner log file (*.log)"));
    QFile file(file_name);

    if (!file_name.isEmpty() && file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream text_stream(&file);
        text_stream << ui->plain_text_edit_with_logs->toPlainText();
        file.close();
    }
}

void XBoxBurner::on_push_button_logs_reset_clicked()
{
    ui->plain_text_edit_with_logs->clear();
}

void XBoxBurner::on_combo_box_dvd_format_currentIndexChanged(int index)
{
    setPixmapForXboxLabel(index);
    updateDvdComboBoxForDvdRFormat(index);
}

void XBoxBurner::setPixmapForXboxLabel(int index)
{
    if (index >= 0)
        ui->label_xbox->setPixmap(QPixmap::fromImage(QImage(":/images/xbox.png")));

    if (index >= 2)
        ui->label_xbox->setPixmap(QPixmap::fromImage(QImage(":/images/xbox360.png")));

    if (index >= 5)
        ui->label_xbox->setPixmap(QPixmap::fromImage(QImage(":/images/dvdr.png")));
}

void XBoxBurner::updateDvdComboBoxForDvdRFormat(int index)
{
    if (index != 5)
      return;

    bool layer_break_value_correct;
    const int layer_break = QInputDialog::getInt(this, QCoreApplication::applicationName(), tr("Enter layer break:"), 0, 0, 4267015, 1, &layer_break_value_correct, Qt::Dialog);

    if (layer_break_value_correct && layer_break > 0)
    {
        ui->combo_box_dvd_format->setItemText(5, tr("Special format (%1)").arg(QString::number(layer_break)));
        return;
    }

    ui->combo_box_dvd_format->setItemText(5, tr("Special format (manual layer break)"));
    ui->combo_box_dvd_format->setCurrentIndex(0);
}

QMenu* XBoxBurner::createPopupMenu()
{
    QPointer<QMenu> menu = QMainWindow::createPopupMenu();

    QPointer<QAction> action_toolButtonIconOnly = new QAction(tr("Icon only"), menu);
    QPointer<QAction> action_toolButtonTextOnly = new QAction(tr("Text only"), menu);
    QPointer<QAction> action_toolButtonTextBesideIcon = new QAction(tr("Text beside icon"), menu);
    QPointer<QAction> action_toolBarTextUnderIcon = new QAction(tr("Text under icon"), menu);

    connect(action_toolButtonIconOnly, &QAction::triggered, this, &XBoxBurner::toolBar_toolButtonIconOnly);
    connect(action_toolButtonTextOnly, &QAction::triggered, this, &XBoxBurner::toolBar_toolButtonTextOnly);
    connect(action_toolButtonTextBesideIcon, &QAction::triggered, this, &XBoxBurner::toolBar_toolButtonTextBesideIcon);
    connect(action_toolBarTextUnderIcon, &QAction::triggered, this, &XBoxBurner::toolBar_toolButtonTextUnderIcon);

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
    settings.data()->setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonIconOnly);
}

void XBoxBurner::toolBar_toolButtonTextOnly()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    settings.data()->setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextOnly);
}

void XBoxBurner::toolBar_toolButtonTextBesideIcon()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    settings.data()->setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextBesideIcon);
}

void XBoxBurner::toolBar_toolButtonTextUnderIcon()
{
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settings.data()->setValue("MainWindow/ToolButtonStyle", Qt::ToolButtonTextUnderIcon);
}

void XBoxBurner::keyReleaseEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->modifiers() != Qt::ControlModifier)
        return;

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
