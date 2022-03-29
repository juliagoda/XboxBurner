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

#include "global.h"

#ifdef Q_OS_WIN
#include <stdio.h>
#include <windows.h>
#include <winioctl.h>
#endif

namespace Ui {
class XBoxBurner;
}

class XBoxBurner : public QMainWindow {
    Q_OBJECT

    QString dvdrwmediainfo, growisofs, imageMd5sum, dvdMd5sum;
    QProcess *checkMediaProcess, *burnProcess;

    QStringList mediaInfo;
    qint64 md5ProgressMax;
    bool burning, verifying, cancel;
    int blockSize;

    void saveSettings();

    void startBurnProcess();

    QString layerBreak();
    void truncateImage();
    bool createBackup();
    void resizeImage();
    void verify();
    QString calculatingImageMD5();
    QString calculatingDvdMD5();

    void startBusy(const bool main = false);
    void stopBusy(const bool main = false);

    QString growisofsVersion();

public:
    explicit XBoxBurner(QWidget* parent = 0);
    ~XBoxBurner();

protected:
    QMenu* createPopupMenu();
    void keyReleaseEvent(QKeyEvent* keyEvent);
    bool event(QEvent* event);
    void loadSettings();
    void checkTools();
    void showStatusBarMessage(const QString& text);
    Ui::XBoxBurner* ui;
    QFutureWatcher<bool> backup_future_watcher;
    QFutureWatcher<QString> image_future_watcher, dvd_future_watcher;

protected slots:
    void burn_triggered();
    void log_triggered();

    void on_pushButton_openImagePath_clicked();
    void on_pushButton_check_clicked();
    void on_comboBox_dvdFormat_currentIndexChanged(int index);
    void startBurn_triggered();
    void verify_triggered();
    void reset_triggered();
    void about_triggered();
    void exit_triggered();
    void on_pushButton_copy_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_logReset_clicked();

    void getMediaInfo_readyReadStandardOutput();
    void getMediaInfo_finished(const int exitCode, const QProcess::ExitStatus exitStatus);

    void logBackupCreation();

    void burnProcess_readyReadStandardOutput();
    void burnProcess_finished(const int exitCode, const QProcess::ExitStatus exitStatus);

    void setMaximalPossibleMD5HashValue(qint64 maximal_value);
    void calculateMD5HashForProgressBar(qint64 value);
    void calculateMD5HashOfImage();
    void calculateMD5HashOfDVD();

    void toolBar_toolButtonIconOnly();
    void toolBar_toolButtonTextOnly();
    void toolBar_toolButtonTextBesideIcon();
    void toolBar_toolButtonTextUnderIcon();

signals:
    void md5SumMaximum(qint64);
    void md5SumProgress(qint64);
};
