/***************************************************************************
 *   Copyright (C) 2022 Jagoda "juliagoda" Górska                          *
 *   juliagoda.pl@protonmail.com                                           *
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

#include "boxmessages.h"
#include "applicationinformations.h"

#include <QMessageBox>
#include <QObject>

int BoxMessages::cancelBurnProcessMessage(QWidget* parent)
{
    return QMessageBox::question(parent, QObject::tr("Cancel burn process"), QObject::tr("Are you sure that you want to cancel the burn process?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
}

int BoxMessages::startBurnProcessMessage(QWidget* parent)
{
    return QMessageBox::question(parent, QObject::tr("Start burn process"), QObject::tr("Are you sure that you want to burn the image?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
}

int BoxMessages::resetAllMessage(QWidget* parent)
{
    return QMessageBox::question(parent, QObject::tr("Reset all"), QObject::tr("Are you sure to reset all?\n\nCaution:\nRunning burn process will canceled!"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
}

void BoxMessages::aboutMessage(QWidget* parent)
{
    ApplicationInformations applications_informations;

    QMessageBox::about(parent, QObject::tr("About XBoxBurner"),
        QObject::tr("<h2>%1</h2>").arg(applications_informations.getApplicationFullName()) + "<p><b><i>Easy to use burner for XBox and XBox360 game images</i></b></p>"
                                                                                             "<p>Development and Copyright &copy; 2022 Jagoda \"juliagoda\" Górska</p>"
                                                                                             "<p><a href='mailto:juliagoda.pl@protonmail.com>?subject=XBoxBurner%20feedback'>juliagoda.pl@protonmail.com</a></p>"
                                                                                             "<p>Development and Copyright &copy; 2011 - 2012 Kai Heitkamp</p>"
                                                                                             "<br/>Based on <a href='http://code.google.com/p/qisoburn'>qisoburn</a>, Copyright &copy; 2010 mycelo"
                                                                                             "<p><a href='mailto:dynup<dynup@ymail.com>?subject=XBoxBurner%20feedback'>dynup@ymail.com</a></p>"
                                                                                             " | <a href='dynup.de.vu'>dynup.de.vu</a>"
                                                                                             "<p>XBoxBurner needs <i>dvd+rw-mediainfo</i> and <i>growisofs</i> to run! Both are parts of the <a href='http://fy.chalmers.se/~appro/linux/DVD+RW/'>dvd+rw-tools</a>!</p> "
                                                                                             "They are available for <a href='http://fy.chalmers.se/~appro/linux/DVD+RW/tools/?M=D'>Linux</a> and <a href='http://fy.chalmers.se/~appro/linux/DVD+RW/tools/win32/'>Windows</a>, "
                                                                                             "for MacOSX via <a href='http://finkproject.org'>Fink</a> or <a href='http://www.macports.org'>MacPorts</a>! The tools must be in PATH or in the same directory of XBoxBurner!"
                                                                                             "<p>Big thanks to the trolls at Trolltech Norway for his excellent Qt toolkit, the guys at Nokia for the continuation and all people at the Qt Project for the open source development of Qt!</p>"
                                                                                             "<br/>"
            + applications_informations.getApplicationFullName() + " is licensed under the GNU General Public License v3 (<a href='http://www.gnu.org/licenses/gpl-3.0.txt'>GPLv3</a>)."
                                                                   "<p><font color='red'>I don't support piracy! If you copy games with this software, you must have the original and it's for your private use only!</font color>");
}
