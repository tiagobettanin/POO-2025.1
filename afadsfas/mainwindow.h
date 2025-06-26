#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Declaração antecipada é suficiente, pois só usamos um ponteiro (Janela2*) aqui.
// Não precisamos da definição completa da Janela2 neste arquivo.
class Janela2;

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

private slots:
    void on_pushButtonAtenticar_clicked();

private:
    Ui::MainWindow *ui;
    Janela2* telaPrincipal;
};
#endif // MAINWINDOW_H
