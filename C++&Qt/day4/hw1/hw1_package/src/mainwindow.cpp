#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cstdint>
#include <QNetworkDatagram>

uint16_t TEXT_PORT = 10004;
QHostAddress ROBOT_IP = QHostAddress("172.100.0.183");
QHostAddress OPERATOR_IP = QHostAddress("172.100.6.53");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lastPressedKey("")
    , currentIndex(-1)
    , isUppercase(false)
{
    ui->setupUi(this);

    if(text_socket->bind(OPERATOR_IP, TEXT_PORT))
    {
        connect(text_socket, &QUdpSocket::readyRead, this, &MainWindow::udp_read, Qt::UniqueConnection);
    }else {
        qDebug() << "Bind 실패:" << text_socket->errorString();
    }

    keyMap[".,?!"] = QStringList{".", ",", "?", "!"};
    keyMap["abc"] = QStringList{"A", "B", "C"};
    keyMap["def"] = QStringList{"D", "E", "F"};
    keyMap["ghi"] = QStringList{"G", "H", "I"};
    keyMap["jkl"] = QStringList{"J", "K", "L"};
    keyMap["mno"] = QStringList{"M", "N", "O"};
    keyMap["pqrs"] = QStringList{"P", "Q", "R", "S"};
    keyMap["tuv"] = QStringList{"T", "U", "V"};
    keyMap["wxyz"] = QStringList{"W", "X", "Y", "Z"};

    Timer = new QTimer(this);
    Timer->setInterval(800);
    connect(Timer, &QTimer::timeout, this, &MainWindow::finalizeChar);

    alphaButtons = {
        ui->abcBtn, ui->defBtn, ui->ghiBtn,
        ui->jklBtn, ui->mnoBtn, ui->pqrsBtn,
        ui->tuvBtn, ui->wxyzBtn
    };
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::udp_read() {
    while (text_socket->hasPendingDatagrams()) {
        qDebug() << "수신됨";
        QNetworkDatagram datagram =
            text_socket->receiveDatagram();

        QByteArray data = datagram.data();

        QString message = QString::fromUtf8(data);

        // 상대방 메시지 출력
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);

        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignLeft);
        cursor.setBlockFormat(blockFormat);

        if(current_sender != QString("상대"))
            cursor.insertText(QString("상대\n"));

        current_sender = QString("상대");
        cursor.insertText(QString("%1\n")
                                 .arg(message));
    }
}

void MainWindow::on_sendBtn_clicked()
{
    QString message = ui->plainTextEdit->toPlainText();
    QByteArray data = message.toUtf8();

    qint64 sentBytes = text_socket->writeDatagram(data, ROBOT_IP, TEXT_PORT);
    if (sentBytes == -1) {
        qDebug() << "전송 실패:" << text_socket->errorString();
    } else {
        qDebug() << "전송 성공";
    }

    // 내 메세지
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignRight);
    cursor.setBlockFormat(blockFormat);

    if(current_sender != QString("나"))
        cursor.insertText(QString("나\n"));

    current_sender =QString("나");
    cursor.insertText(QString("%1\n")
                            .arg(message));

    ui->plainTextEdit->clear(); // 입력창 비우기
}

void MainWindow::handleKey(const QString &keyId)
{
    Timer->stop(); // 새 입력이 왔으므로 기존 타이머 중단

    if (lastPressedKey == keyId) {
        // 같은 버튼을 눌렀을 때
        currentIndex = (currentIndex + 1) % keyMap[keyId].size();

        QString targetChar = keyMap[keyId][currentIndex];
        if (!isUppercase) targetChar = targetChar.toLower();

        ui->plainTextEdit->textCursor().deletePreviousChar();
        ui->plainTextEdit->insertPlainText(targetChar);

    } else {
        finalizeChar();

        lastPressedKey = keyId;
        currentIndex = 0;

        QString targetChar = keyMap[keyId][currentIndex];
        if (!isUppercase) targetChar = targetChar.toLower();

        ui->plainTextEdit->insertPlainText(targetChar);
    }

    Timer->start();
}

void MainWindow::finalizeChar()
{
    Timer->stop();
    lastPressedKey = "";
    currentIndex = -1;
}

void MainWindow::on_pointBtn_clicked() {handleKey(".,?!");}
void MainWindow::on_abcBtn_clicked() {handleKey("abc");}
void MainWindow::on_defBtn_clicked() {handleKey("def");}
void MainWindow::on_ghiBtn_clicked() {handleKey("ghi");}
void MainWindow::on_jklBtn_clicked() {handleKey("jkl");}
void MainWindow::on_mnoBtn_clicked() {handleKey("mno");}
void MainWindow::on_pqrsBtn_clicked() {handleKey("pqrs");}
void MainWindow::on_tuvBtn_clicked() {handleKey("tuv");}
void MainWindow::on_wxyzBtn_clicked() {handleKey("wxyz");}

void MainWindow::on_spaceBtn_clicked()
{
    QString data = " ";
    ui->plainTextEdit->insertPlainText(data);
}


void MainWindow::on_shiftBtn_clicked()
{
    isUppercase = !isUppercase;

    for (QPushButton *btn : alphaButtons) {
        if (!btn) continue;

        QString text = btn->text();
        if (isUppercase) {
            btn->setText(text.toUpper()); // 대문자로
        } else {
            btn->setText(text.toLower()); // 소문자로
        }
    }

    if (isUppercase) ui->shiftBtn->setText("▲");
    else ui->shiftBtn->setText("shift");
}


void MainWindow::on_enterBtn_clicked()
{
    QString data = "\n";
    ui->plainTextEdit->insertPlainText(data);
}


void MainWindow::on_backBtn_clicked()
{
    ui->plainTextEdit->textCursor().deletePreviousChar();
}

void MainWindow::on_commaBtn_clicked()
{
    QString data = ",";
    ui->plainTextEdit->insertPlainText(data);
}

