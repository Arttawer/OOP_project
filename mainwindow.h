#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <set>
#include <vector>
#include <string>
#include <cmath>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <fstream>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct CharInfo {
    QString c;
    int freq;
    std::string code;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_ApplyEncode_clicked();

    void on_action_triggered();

    void on_getString_clicked();

    void on_btn_Caesar_clicked();

    void on_btn_Vijener_clicked();

    void on_btn_XOR_clicked();

    void on_caesarOffset_sliderMoved(int position);

    void on_line_Caesar_editingFinished();

private:
    double entropyCalc(QString& str, CharInfo* chars, int n);
    QString caesarCalc(int offset);
    QString cryptString(QString toCrypt);
    void encodeDecode();
    int keyIndex(int index);
    const QString kEncryptorString = QString("tank");
    QString mainString;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

