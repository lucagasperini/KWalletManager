#include "kwalletmanagerinputentry.h"
#include "ui_kwalletmanagerinputentry.h"
#include <QComboBox>
#include <QLineEdit>

KWalletManagerInputEntry::KWalletManagerInputEntry(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::KWalletManagerInputEntry)
{
	ui->setupUi(this);
}

KWalletManagerInputEntry::~KWalletManagerInputEntry()
{
	delete ui;
}

void KWalletManagerInputEntry::on_buttonBox_accepted()
{
	switch(ui->comboBox->currentIndex()) {
	case 0:
		type = KWallet::Wallet::Password;
		break;
	case 1:
		type = KWallet::Wallet::Map;
		break;
	case 2:
		type = type = KWallet::Wallet::Stream;
		break;
	}
	if(ui->lineEdit->text().isEmpty())
		setResult(Error);
	else
		name = ui->lineEdit->text();
	setResult(Accept);
}

void KWalletManagerInputEntry::on_buttonBox_rejected()
{
	setResult(Reject);
}
