#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class connectdialog;
}

class connectdialog : public QDialog
{
    Q_OBJECT

public:
    explicit connectdialog(QWidget *parent = nullptr);
    ~connectdialog();

    QString getUrl() const;

private slots:
    void on_ConnectButton_clicked();

private:
    QString url;
    Ui::connectdialog *ui;

};

#endif // CONNECTDIALOG_H
