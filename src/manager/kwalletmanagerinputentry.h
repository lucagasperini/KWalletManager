#ifndef KWALLETMANAGERINPUTENTRY_H
#define KWALLETMANAGERINPUTENTRY_H

#include <QDialog>
#include <kwallet.h>

namespace Ui {
class KWalletManagerInputEntry;
}

enum KWalletManagerInputResult {Reject = QDialog::Rejected, Accept = QDialog::Accepted, Error};

class KWalletManagerInputEntry : public QDialog
{
	Q_OBJECT

public:
    KWalletManagerInputEntry(QWidget *parent = 0);
	~KWalletManagerInputEntry();

    KWallet::Wallet::EntryType entryType() const { return type; }
    QString entryName() const { return name; }
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
	Ui::KWalletManagerInputEntry *ui;
        KWallet::Wallet::EntryType type;
        QString name;
};

#endif // KWALLETMANAGERINPUTENTRY_H
