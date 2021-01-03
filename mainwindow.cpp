#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void ShannonFano(CharInfo* chars, int end, int start = 0, const std::string& branch = "", const std::string& fullBranch = "") {
    std::string code = fullBranch + branch; //К уже накопленному коду прибавляем branch (0, или1, в зависимости от разветвления)

    // когда упёрлись в один символ - присваиваем ему текущий код и заканчиваем углубление
    if (start == end) {
        chars[start].code = code;
        return;
    }

    int sum = 0; // переменная отвечающая за частоту
    // считаем сумму частоты от начала и до конца
    for (int i = start; i <= end; i++)
        sum += chars[i].freq; //Увеличиваем её из текущих чарактеров

    sum /= 2; // делим пополам, в соответствии с алгоритмом Шеннона-Фано, чтобы получить идеал равных частот
    int i, sum2 = chars[start].freq; //Объявляем переменную под итератор, так как она нам потом будет нужна и так же нам нужна вторая сумма..
    //..стартового чарактера

    // ищем границу раздела массива частот для наиболее равномерного разделения
    for (i = start + 1; abs(sum - (sum2 + chars[i].freq)) < abs(sum - sum2) && i < end; i++)//итератор стартует от начала+1,
        sum2 += chars[i].freq;
    //Условия для возможности следующей итерации: модуль частоты минус значение текущей суммы плюс частота текущего чарактера должно быть..
    //..меньше, чем модуль разности текущей суммы минус сумма частот. Также итератор не должен перевалить за конец.

    ShannonFano(chars, i - 1, start, "0", code); //Вызываем нашу функцию ещё раз, в качестве конца укажем значение итератора - 1,
    //start как и был остаётся, пускаем ветку по нулю
    ShannonFano(chars, end, i, "1", code); //Здесь мы в качестве конца укажем ранее переданный конец, и в качестве начала будет выступать итератор..
    //..ветку пускаем по единице.
}

void MainWindow::on_btn_ApplyEncode_clicked()
{
    ui->text_ViewEncodeString->clear(); //Очищаем текст для алфавита и текущей закодированной строки, чтобы в случае новой кодировки..
    ui->text_alphabet->clear(); //..данные не сконкатенировались
    if (mainString == "") { //Если пользователь ничего не ввёл, то значит, что мы не должны ничего кодировать. Покажем ошибку и сделаем возврат из функции
        QMessageBox::critical(this, "Ошибка!", "Введите сообщения для кодировки.");
        return;
    }
    CharInfo* characters = new CharInfo[mainString.length()]; // массив символов и частот строки
    int size = 0; // размер массива
            // обрабатываем всю строку
    for (int i = 0; i < (int)mainString.length(); i++) {
        int j = 0;
    //ищем в массиве текущий символ
        while (j < size && characters[j].c[0] != mainString[i])
            j++;
             // если не нашли (j = size), то добавляем этот символ и учитываем, что частота его равна 1
        if (j == size) {
            characters[size].c = mainString[i];
            characters[size++].freq = 1;
        }
        else
            characters[j].freq++; // если нашли, увеличиваем значение частоты
    }
    bool isSorted = false;
    while (!isSorted) { //Сортировочка, чтобы по порядку вывести получившийся алфавит
        isSorted = true;
        for (int i = 0; i < size - 1; i++) {
            if (characters[i].freq < characters[i + 1].freq) {
                CharInfo tmp = characters[i];
                characters[i] = characters[i + 1];
                characters[i + 1] = tmp;
                isSorted = false;
            }
        }
    }
    ShannonFano(characters, size - 1);
    QString encodedStr; //в эту строку поместим закодированный алфавит
    for (int i = 0; i < mainString.length(); i++) { //По всем символам исходной строки
        for (int j = 0; j < size; j++) {//Ищем символ из алфавита, схожий с текущим символом из строки
            if (characters[j].c == mainString[i]) { //Символы равны?
                encodedStr += characters[j].code.c_str(); //В кодировочную строку прибавляем код символа и пробел
                encodedStr += " ";
        }
        }
    }
    ui->text_ViewEncodeString->append(encodedStr); //вставляем закодированный текст в браузер текста
    for (int i = 0; i < size; i++) { //Выводим алфавит по порядку, через строку, вместе с символом, частотой и кодом.
        QString temp =characters[i].c + QString(" | ") + QString::number(characters[i].freq) + QString(" | ") + QString::fromUtf8(characters[i].code.c_str());
        ui->text_alphabet->append(temp);
    }
}

QString MainWindow::caesarCalc(int offset) {
    QString ruAlpha = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    QString enAlpha = "abcdefghijklmnopqrstuvwxyz";
    QString encoded = "";
    bool ok = false;
    bool isEncoded = false;
    for (int i = 0; i < mainString.size(); i++) {
        for (int r = 0; r < 33; r++)
            if (mainString[i].toLower() == ruAlpha[r]) {
                            encoded += ruAlpha[(r + offset) % 33];
                            ok = true;
                            isEncoded = true;
                            break;
                        }
        if (!ok)
            for (int en = 0; en < 28; en++)
                if (mainString[i].toLower() == enAlpha[en]) {
                                encoded += enAlpha[(en + offset) % 28];
                                ok = false;
                                isEncoded = true;
                                break;
                            }
        ok = false;
        if (!isEncoded)
            encoded += mainString[i];
        isEncoded = false;
    }

    return encoded;
}

QString MainWindow::cryptString(QString toCrypt)
    {
        QString resultString = "";
        QString B, C, D = "";
        QString Ae = "abcdefghijklmnopqrstuvwxyz";
        QString Aru = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
        bool isRu = false, isEn = false;
        for (int i = 0; i < 33; i++) {
            if(mainString[0].toLower() == Aru[i])
                isRu = true;
        }
        for (int i = 0; i < 26; i++) {
            if(mainString[0].toLower() == Ae[i])
                isEn = true;
        }
        C = ui->line_Key->text();
        C = C.toLower();
        int size; QString A;
        bool flag = false;
        if (isRu) {
            qDebug() << "Ru";
            size = 33;
            A = Aru;
            for (int i = 0; i < 33; i++)
                if (C[0] == Aru[i])
                    flag = true;
            if (!flag) {
                QMessageBox::critical(this, "Ошибка!", "Неверный ключ для кодировки.");
                return nullptr;
            }

        } else {
            qDebug() << "En";
            size = 26;
            A = Ae;
            for (int i = 0; i < 26; i++)
                if (C[0] == Ae[i])
                    flag = true;
            if (!flag) {
                QMessageBox::critical(this, "Ошибка!", "Неверный ключ для кодировки.");
                return nullptr;
            }
        }
        qDebug() << toCrypt;
        B = toCrypt;
        B = B.toLower();
        C = ui->line_Key->text();
        C = C.toLower();
        int F[B.size()];
        int G[B.size()];
        int c = C.size(), b = B.size();
        if (b >= c ) {
            for (int i = 0; i < (b/c); i++)
                D = D + C;
            for (int j = 0; j < (b%c); j++)
                D = D + C[j];
        } else {
            for (int s = 0; s < b; s++) {
                D = D + B[s];
            }
        }
        for (int k = 0; k < b; k++) {
            for(int n = 0; n < size; n++) {
                if (B[k] == A[n]) {
                    F[k] = n;
                }
                if (D[k] == A[n]) {
                    G[k] = n;
                }
            }
        }
        int e = 0;
        for (int u = 0; u < b; u++) {
            e = ((F[u] + G[u])%size);
            B[u] = A[e];
        }
        qDebug() << B;
        return B;
    }
int MainWindow::keyIndex(int index)
{
    int len = kEncryptorString.length();
    int multiple = index / len;
    if ( index >=  len ) {
        return index - (len * multiple);
    }
    return index;
}

void MainWindow::encodeDecode()
{
        const char* input = mainString.toStdString().c_str();
        int inputLength = mainString.size();
        const char* key = ui->line_XOR->text().toLatin1().data();
        int keyLength = ui->line_XOR->text().size();

        char output[inputLength];

        for (int i = 0; i < inputLength + 1; ++i)
        {
            output[i] = input[i] ^ key[i % keyLength + 1];
        }

        ui->text_XOR->setText(QString::fromLatin1(output, inputLength));
}

void MainWindow::on_action_triggered()
{
    QMessageBox::information(this, "О программе", "Программа кодирует строки различными способами.");
}

void MainWindow::on_getString_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                      "/Текстовый файл",
                                                      tr("Текст (*.txt)"));
    std::ifstream f(fileName.toStdString());
    std::string str;
    std::getline(f,str);
    mainString = QString::fromUtf8(str.c_str());
}

void MainWindow::on_btn_Caesar_clicked()
{
    if (mainString == "") { //Если пользователь ничего не ввёл, то значит, что мы не должны ничего кодировать. Покажем ошибку и сделаем возврат из функции
        QMessageBox::critical(this, "Ошибка!", "Введите сообщения для кодировки.");
        return;
    }
    ui->caesarBrowser->setText(caesarCalc(ui->caesarOffset->value()));
}

void MainWindow::on_btn_Vijener_clicked()
{
    if (mainString == "" || ui->line_Key->text() == "") {
        QMessageBox::critical(this, "Ошибка!", "Введите сообщение и ключ для кодировки.");
        return;
    }
    QString str = "", newStr = "", showStr = "";
    QStringList s = mainString.split(' ');
    for (int i = 0; i < s.size(); i++) {
        str += s.at(i);
    }
    newStr = cryptString(str);
    int counter = 0;
    for (int i = 0; i < mainString.size(); i++) {
        if (mainString[i] == ' ')
            showStr+=" ";
        else {
            showStr += (newStr[counter]);
            counter++;
        }
    }
    ui->text_Vijener->setText(showStr);
}

void MainWindow::on_btn_XOR_clicked()
{
    if (mainString == "")  {
        QMessageBox::critical(this, "Ошибка!", "Введите сообщения для кодировки.");
        return;
    }
    if (ui->line_XOR->text() == "") {
        QMessageBox::critical(this, "Ошибка!", "Введите ключ для кодировки.");
        return;
    }
    encodeDecode();
}

void MainWindow::on_caesarOffset_sliderMoved(int position)
{
    ui->line_Caesar->setText(QString::number(position));
}

void MainWindow::on_line_Caesar_editingFinished()
{
    ui->caesarOffset->setValue(ui->line_Caesar->text().toInt());
}

void MainWindow::on_btn_Exit_clicked()
{
    exit(0);
}

void MainWindow::on_btn_Save_clicked()
{
    QString XOR;
    QString Caesar;
    QString SF;
    QString Vijener;
    XOR = ui->text_XOR->toPlainText();
    Caesar = ui->caesarBrowser->toPlainText();
    SF = ui->text_ViewEncodeString->toPlainText();
    Vijener = ui->text_Vijener->toPlainText();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"),
                                                      "/Текстовый файл",
                                                      tr("Текст (*.txt)"));
    std::ofstream f(fileName.toStdString(), std::ios::app);
    f << "Origin: " << mainString.toStdString() <<"\nShannon-Fano: " << SF.toStdString() << "\nCaesar: " << Caesar.toStdString() << "\nVijener: " << Vijener.toStdString() << "\nXOR: " << XOR.toStdString() << "\n";
    f.close();
}


void MainWindow::on_mainString_textChanged(const QString &arg1)
{
    mainString = arg1;
}
