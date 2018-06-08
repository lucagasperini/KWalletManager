#include "wingenerate.h"
#include "ui_wingenerate.h"
#include <QCheckBox>

#define MAX_UNICODE 65536
#define MAX_ASCII 255

winGenerate::winGenerate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::winGenerate)
{
    ui->setupUi(this);
}

winGenerate::~winGenerate()
{
    delete ui;
}

QString winGenerate::generatePassword(int length, bool symbols, bool spaces, bool unicode, bool numbers, bool lowers, bool uppers)
{
	QChar tmp;
	QString offset;
	bool pass = false;
	srand(time(0));

	while(offset.length() < length)
	{
		if(unicode)
			tmp = (unsigned short)rand() % MAX_UNICODE;
		else
			tmp = (char)rand() % MAX_ASCII;
		if(tmp.isUpper() && uppers)
			pass = true;
		else if(tmp.isLower() && lowers)
			pass = true;
		else if(tmp.isDigit() && numbers)
			pass = true;
		else if(tmp.isSpace() && spaces)
			pass = true;
		else if(tmp.isSymbol() && symbols)
			pass = true;
		else
			pass = false;

		if(pass)
			offset.append(tmp);

	}

	return offset;
}

void winGenerate::on_pushGenerate_clicked() //TODO: lock button when working and show time elapsed
{
	ui->lineEdit->setText(generatePassword(ui->spinLength->value(), ui->checkSymb->isChecked(),
                                               ui->checkSpaces->isChecked(),ui->checkUni->isChecked(),
                                               ui->checkNum->isChecked(), ui->checkLowers->isChecked(),
                                               ui->checkUppers->isChecked()));
}

void winGenerate::on_pushButton_clicked()
{
    destroy();
}
