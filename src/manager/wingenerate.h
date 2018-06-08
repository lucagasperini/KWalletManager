#ifndef WINGENERATE_H
#define WINGENERATE_H

#include <QDialog>

namespace Ui {
class winGenerate;
}

class  winGenerate : public QDialog
{
    Q_OBJECT

public:
    explicit winGenerate(QWidget *parent = 0);
    ~winGenerate();
    static QString generatePassword(int length, bool symbols, bool spaces, bool unicode, bool numbers, bool lowers, bool uppers);
private slots:
    void on_pushGenerate_clicked();

    void on_pushButton_clicked();

private:
    Ui::winGenerate *ui;
};

#endif // WINGENERATE_H
