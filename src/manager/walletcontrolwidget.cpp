/*
   Copyright (C) 2013 Valentin Rusu <kde@rusu.info>

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

#include "walletcontrolwidget.h"
#include "kwalleteditor.h"
#include "applicationsmanager.h"
#include "kwalletmanager_debug.h"
#include "wingenerate.h"

#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <qmenu.h>
#include <kwallet.h>


#include <QTimer>

WalletControlWidget::WalletControlWidget(QWidget *parent, const QString &walletName):
    QWidget(parent),
    _walletName(walletName),
    _wallet(nullptr),
    _walletEditor(nullptr),
    _applicationsManager(nullptr)
{
    setupUi(this);
    onSetupWidget();

    QTimer::singleShot(1, this, &WalletControlWidget::onSetupWidget);
}

bool WalletControlWidget::openWallet()
{
    bool result = false;
    if (_wallet && _wallet->isOpen()) {
        result = true; // already opened
    } else {
        _wallet = KWallet::Wallet::openWallet(_walletName, effectiveWinId());
        result = _wallet != nullptr;
        onSetupWidget();
    }
    return result;
}

void WalletControlWidget::onSetupWidget()
{
    if (KWallet::Wallet::isOpen(_walletName)) {
        if (nullptr == _wallet) {
            _wallet = KWallet::Wallet::openWallet(_walletName, effectiveWinId());
            if (nullptr == _wallet) {
                qCDebug(KWALLETMANAGER_LOG) << "Weird situation: wallet could not be opened when setting-up the widget.";
            }
        }
    }

    if (_wallet) {
        connect(_wallet, &KWallet::Wallet::walletClosed, this, &WalletControlWidget::onWalletClosed);
        _openClose->setText(i18n("&Close"));

        if (nullptr == _walletEditor) {
            _walletEditor = new KWalletEditor(_editorFrame);
            _editorFrameLayout->addWidget(_walletEditor);
            _walletEditor->setVisible(true);
        }
        _walletEditor->setWallet(_wallet);

        if (nullptr == _applicationsManager) {
            _applicationsManager = new ApplicationsManager(_applicationsFrame);
            _applicationsFrameLayout->addWidget(_applicationsManager);
            _applicationsManager->setVisible(true);
        }
        _applicationsManager->setWallet(_wallet);

        _changePassword->setEnabled(true);
        _stateLabel->setText(i18nc("the 'kdewallet' is currently open (e.g. %1 will be replaced with current wallet name)", "The '%1' wallet is currently open", _walletName));
        _tabs->setTabIcon(0, QIcon::fromTheme(QLatin1String("wallet-open")).pixmap(16));
    } else {
        _openClose->setText(i18n("&Open..."));

        if (_walletEditor) {
            _walletEditor->setVisible(false);
            delete _walletEditor;
            _walletEditor = nullptr;
        }

        if (_applicationsManager) {
            _applicationsManager->setVisible(false);
            delete _applicationsManager;
            _applicationsManager = nullptr;
        }
        _changePassword->setEnabled(false);
        _stateLabel->setText(i18n("The wallet is currently closed"));
        _tabs->setTabIcon(0, QIcon::fromTheme(QStringLiteral("wallet-closed")).pixmap(16));
    }
}

void WalletControlWidget::onOpenClose()
{
    // TODO create some fancy animation here to make _walletEditor appear or dissapear in a fancy way
    if (_wallet) {
        if (hasUnsavedChanges()) {
            int choice = KMessageBox::warningYesNo(this, i18n("Ignore unsaved changes?"));
            if (choice == KMessageBox::No) {
                return;
            }
        }
        // Wallet is open, attempt close it
        int rc = KWallet::Wallet::closeWallet(_walletName, false);
        if (rc != 0) {
            rc = KMessageBox::warningYesNo(this, i18n("Unable to close wallet cleanly. It is probably in use by other applications. Do you wish to force it closed?"), QString(), KGuiItem(i18n("Force Closure")), KGuiItem(i18n("Do Not Force")));
            if (rc == KMessageBox::Yes) {
                rc = KWallet::Wallet::closeWallet(_walletName, true);
                if (rc != 0) {
                    KMessageBox::sorry(this, i18n("Unable to force the wallet closed. Error code was %1.", rc));
                } else {
                    _wallet = nullptr;
                }
            }
        } else {
            _wallet = nullptr;
        }
    } else {
        _wallet = KWallet::Wallet::openWallet(_walletName, window()->winId());
    }
    onSetupWidget();
}

void WalletControlWidget::onWalletClosed()
{
    _wallet = nullptr;
    onSetupWidget();
}

void WalletControlWidget::updateWalletDisplay()
{
//     QList<QAction*> existingActions = _disconnect->actions();
//     QList<QAction*>::const_iterator i = existingActions.constBegin();
//     QList<QAction*>::const_iterator ie = existingActions.constEnd();
//     for ( ; i != ie; i++ ) {
//         _disconnect->removeAction(*i);
//     }
//
}

void WalletControlWidget::onDisconnectApplication()
{
    QAction *a = qobject_cast<QAction *>(sender());
    Q_ASSERT(a);
    if (a)  {
        KWallet::Wallet::disconnectApplication(_walletName, a->data().toString());
    }
}

void WalletControlWidget::onChangePassword()
{
    KWallet::Wallet::changePassword(_walletName, effectiveWinId());
}

bool WalletControlWidget::hasUnsavedChanges() const
{
    return (_walletEditor ? _walletEditor->hasUnsavedChanges() : false);
}

void WalletControlWidget::hideEvent(QHideEvent *)
{
}

void WalletControlWidget::showEvent(QShowEvent *)
{
}

void WalletControlWidget::onRemoveWallet() //REPEATED
{
	if (_walletName.isEmpty()) {
		return;
	}
	int rc = KMessageBox::warningContinueCancel(this, i18n("Are you sure you wish to delete the wallet '%1'?", _walletName), QString(), KStandardGuiItem::del());
	if (rc != KMessageBox::Continue) {
		return;
	}
	rc = KWallet::Wallet::deleteWallet(_walletName);
	if (rc != 0) {
		KMessageBox::sorry(this, i18n("Unable to delete the wallet. Error code was %1.", rc));
	}
}

void WalletControlWidget::onGeneratePassword()
{
	winGenerate* win = new winGenerate(this);
	win->show();
}
