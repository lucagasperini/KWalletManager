/*
 *
 * This file is part of the KDE project.
 * Copyright (C) 2003-2005 George Staikos <staikos@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KBETTERTHANKDIALOGBASE_H
#define KBETTERTHANKDIALOGBASE_H

#include "ui_kbetterthankdialogbase.h"

class KBetterThanKDialogBase : public QDialog, private Ui_KBetterThanKDialogBase
{
    Q_OBJECT

public:
    explicit KBetterThanKDialogBase(QWidget *parent = nullptr);

public slots:
    virtual void setLabel(const QString &label);
    void accept() override;
    void reject() override;

private slots:
    virtual void clicked();
};

#endif
