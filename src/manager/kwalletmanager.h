/*
   Copyright (C) 2003,2004 George Staikos <staikos@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KWALLETMANAGER_H
#define KWALLETMANAGER_H
#include <QObject>
#include <kxmlguiwindow.h>
#include <QPushButton>

class KWalletManagerWidget;
class KStatusNotifierItem;
class QListWidgetItem;
class OrgKdeKWalletInterface;
class QAction;

class KWalletManager : public KXmlGuiWindow
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", "org.kde.kwallet.kwalletmanager")

public:
    explicit KWalletManager(QWidget *parent = nullptr, const QString &name = QString(), Qt::WindowFlags f = 0);
    ~KWalletManager() override;

    void kwalletdLaunch();
    bool hasUnsavedChanges(const QString& name = QString()) const;
    bool canIgnoreUnsavedChanges();

public slots:
    void createWallet();
    void deleteWallet();
    void closeWallet(const QString &walletName);
    void changeWalletPassword(const QString &walletName);
    void openWallet(const QString &walletName);
    void openWalletFile(const QString &path);
//         void openWallet(QListWidgetItem *item);
    void contextMenu(const QPoint &pos);
    void walletCreated(const QString &walletName);

protected:
    bool queryClose() override;

private:
public Q_SLOTS: //dbus
    Q_SCRIPTABLE void allWalletsClosed();
    Q_SCRIPTABLE void updateWalletDisplay();
    Q_SCRIPTABLE void aWalletWasOpened();

private slots:
    void shuttingDown();
    void possiblyQuit();
    void editorClosed(KXmlGuiWindow *e);
    void possiblyRescan(const QString &app, const QString &, const QString &);
    void setupWallet();
    void openWallet();
    void closeAllWallets();
    void exportWallets();
    void importWallets();
    void beginConfiguration();
    void configUI();

private:
    KStatusNotifierItem *_tray;
    bool _shuttingDown;
    KWalletManagerWidget *_managerWidget;
    OrgKdeKWalletInterface *m_kwalletdModule;
    QList<KXmlGuiWindow *> _windows;
    bool _kwalletdLaunch;
    QAction *_walletsExportAction = nullptr;
};

#endif
