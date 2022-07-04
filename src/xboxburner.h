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

#pragma once

#include <QMainWindow>
#include <QtGui>
#include <QPointer>

#include "backup.h"
#include "constants/applicationinformations.h"
#include "settings/settings.h"
#include "verification/verification.h"
#include "burner/currentburningverificationstage.h"
#include "verification/burnerpathvalidation.h"

#ifdef Q_OS_WIN
  #include <stdio.h>
  #include <windows.h>
  #include <winioctl.h>
#endif

namespace Ui
{
  class XBoxBurner;
}

class XBoxBurner : public QMainWindow
{
    Q_OBJECT

public:
    explicit XBoxBurner(const ApplicationInformations& new_applications_informations,
                        QWidget* parent = 0);
    ~XBoxBurner();

protected:
    QMenu* createPopupMenu();
    void keyReleaseEvent(QKeyEvent* keyEvent);
    bool event(QEvent* event);

    Ui::XBoxBurner* ui;
    QFutureWatcher<bool> backup_future_watcher;
    QFutureWatcher<QString> image_future_watcher, dvd_future_watcher;
    QPointer<Settings> settings;
    QPointer<Backup> backup;
    QSharedPointer<BurnerWidgets> burner_widgets;

protected slots:
    void about_triggered();
    void burn_triggered();
    void exit_triggered();
    void log_triggered();
    void reset_triggered();
    void startBurn_triggered();
    void verify_triggered();

    void on_combo_box_dvd_format_currentIndexChanged(int index);
    void on_push_button_check_clicked();
    void on_push_button_copy_clicked();
    void on_push_button_logs_reset_clicked();
    void on_push_button_open_image_path_clicked();
    void on_push_button_save_clicked();

    void toolBar_toolButtonIconOnly();
    void toolBar_toolButtonTextOnly();
    void toolBar_toolButtonTextBesideIcon();
    void toolBar_toolButtonTextUnderIcon();

private:
    QSharedPointer<BurnerWidgets> createStructFromBurnerWidgets();
    QPointer<CurrentBurningVerificationStage> createBurnerSteps();

    void setPixmapForXboxLabel(int index);
    void updateDvdComboBoxForDvdRFormat(int index);
    void initializeSettingsSave();

    const ApplicationInformations applications_informations;
    QPointer<BurnerStage> start_burner_stage;
    QPointer<BurnerPathValidation> burner_path_validation;
    QPointer<Verification> verification;
};
