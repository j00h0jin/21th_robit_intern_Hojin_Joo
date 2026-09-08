#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QUdpSocket *text_socket = new QUdpSocket(this);

    void udp_read();
    QString current_sender;

private slots:
    void on_sendBtn_clicked();

    void on_abcBtn_clicked();
    void on_defBtn_clicked();
    void on_ghiBtn_clicked();
    void on_jklBtn_clicked();
    void on_mnoBtn_clicked();
    void on_pqrsBtn_clicked();
    void on_tuvBtn_clicked();
    void on_wxyzBtn_clicked();
    void on_spaceBtn_clicked();
    void on_shiftBtn_clicked();
    void on_enterBtn_clicked();
    void on_backBtn_clicked();
    void on_commaBtn_clicked();
    void on_pointBtn_clicked();
    void finalizeChar();

private:
    Ui::MainWindow *ui;

    // 헬퍼 함수
    void handleKey(const QString &keyId);

    QTimer *Timer;
    QMap<QString, QStringList> keyMap;
    QString lastPressedKey;
    int currentIndex;
    bool isUppercase;

    QList<QPushButton*> alphaButtons;
};
#endif // MAINWINDOW_H
