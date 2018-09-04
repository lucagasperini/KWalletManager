/*
   Copyright (C) 2003-2005 George Staikos <staikos@kde.org>
   Copyright (C) 2005 Isaac Clerencia <isaac@warp.es>

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

#ifndef KWALLETEDITOR_H
#define KWALLETEDITOR_H

#include "ui_walletwidget.h"

#include <kwallet.h>
#include <kxmlguiwindow.h>
#include "registercreateactionmethod.h"
#include <QLabel>
#include <QPushButton>

class KActionCollection;
class QMenu;
class QTreeWidgetItem;
class QCheckBox;
class KWalletEntryList;
class KWMapEditor;
class KTreeWidgetSearchLine;

class KWalletEditor : public QWidget, public Ui::WalletWidget
{
    Q_OBJECT

public:
    explicit KWalletEditor(QWidget *parent, const QString &name = QString());
    virtual ~KWalletEditor();

    /**
     * @brief setWallet take KWallet::Wallet as input in order to using this @class
     * @param wallet pointer of KWallet::Wallet, it must be different of nullptr
     * @param isPath an boolean value, it will be stored in @private _nonLocal
     */
    void setWallet(KWallet::Wallet *wallet, bool isPath = false);
    /**
     * @brief isOpen check if wallet is open.
     * @return true if @private _w is different of nullptr, false it is not set.
     */
    bool isOpen() const;

    /**
     * @brief hasUnsavedChanges check if there are unsaved changes in the wallet
     * @return value of @private _hasUnsavedChanges
     */
    bool hasUnsavedChanges() const;
    /**
     * @brief setNewWallet @note not sure what is it.
     * @param newWallet set value of @private _newWallet
     */
    void setNewWallet(bool newWallet);

protected:
    /**
     * @brief hideEvent override to disconnect and disable actions
     */
    void hideEvent(QHideEvent *) override;
    /**
     * @brief showEvent override to connect and enable actions
     */
    void showEvent(QShowEvent *) override;

public slots:
    /**
     * @brief walletClosed set @private _w to nullptr and disable wallet action
     */
    void walletClosed();
    /**
     * @brief createFolder start the process to create a new folder
     */
    void createFolder();
    /**
     * @brief deleteFolder start the process to delete a existing folder
     */
    void deleteFolder();

private slots:
    /**
     * @brief updateFolderList trash existing folder in @gui to create new ones
     * @param checkEntries
     * @note not sure how it works
     */
    void updateFolderList(bool checkEntries = false);
    /**
     * @brief entrySelectionChanged manage the selection of an entry
     * @param item
     * @note not sure how it works, cannot be modular?
     */
    void entrySelectionChanged(QTreeWidgetItem *item);
    /**
     * @brief listItemChanged manage the form when entry is changed
     * @param item the entry pointer
     * @param column
     * @note check for improvement
     */
    void listItemChanged(QTreeWidgetItem *item, int column);
    /**
     * @brief listContextMenuRequested popup the context menu
     * @param pos is the position on widget
     */
    void listContextMenuRequested(const QPoint &pos);
    /**
     * @brief updateEntries update entry list in @gui
     * @param folder is the folder on the wallet selected
     * @note check code under the comment
     */
    void updateEntries(const QString &folder);

    void newEntry();
    void renameEntry();
    void deleteEntry();
    void entryEditted();
    void restoreEntry();
    void saveEntry();

    void changePassword();

    void walletOpened(bool success);
    void showContent(bool v);

    void exportXML();
    void importXML();
    void importWallet();

    void copyPassword();

    void onSearchTextChanged(const QString &);

signals:
    void enableWalletActions(bool enable);
    void enableFolderActions(bool enable);
    void enableContextFolderActions(bool enable);

public:
    QString _walletName;

private:
    static void createActions(KActionCollection *);
    void connectActions();
    void disconnectActions();
    KActionCollection *actionCollection();

    bool _nonLocal;
    KWallet::Wallet *_w;
    KWalletEntryList *_entryList;
    static RegisterCreateActionsMethod _registerCreateActionMethod;
    static QAction *_newFolderAction, *_deleteFolderAction;
    static QAction *_exportAction, *_saveAsAction, *_mergeAction, *_importAction;
    static QAction *_newEntryAction, *_renameEntryAction, *_deleteEntryAction;
    static QAction *_copyPassAction;
    QLabel *_details;
    QString _currentFolder;
    QMap<QString, QString> *_currentMap; // get memory by pointer
	QPushButton* _genPassword;
    bool _newWallet;
    QMenu *_contextMenu;
    QTreeWidgetItem *_displayedItem; // used to find old item when selection just changed
    KActionCollection *_actionCollection;
    QMenu *_controlMenu;
    QMenu *_walletSubmenu;
    KTreeWidgetSearchLine *_searchLine;
    bool _showContent;
    bool _hasUnsavedChanges;
};

#endif
