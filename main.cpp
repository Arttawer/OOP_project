    #include "mainwindow.h"
    #include <QTextCodec>
    #include <QApplication>
    
    int main(int argc, char *argv[])
    {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
        setlocale(LC_ALL, "russian");
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
