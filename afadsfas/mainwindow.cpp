#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Includes que antes estavam no .h agora ficam aqui
#include "backend.h"
#include "janela2.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelErro->setText("");
    ui->lineEditSenha->setEchoMode(QLineEdit::Password);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonAtenticar_clicked()
{
    QString usuario = ui->lineEditUsuario->text();
    QString senha = ui->lineEditSenha->text();

    if(usuario.isEmpty() || senha.isEmpty()){
        ui->labelErro->setText("Por favor, preencha todos os campos.");
        return;
    }

    Atendente* atendenteLogado = BancoDeReservas::getInstancia()->autenticar(usuario.toStdString(), senha.toStdString());

    if (atendenteLogado != nullptr) {
        telaPrincipal = new Janela2(atendenteLogado, this);
        telaPrincipal->show();
        this->hide();
    } else {
        ui->labelErro->setText("Usuário ou senha inválidos.");
        ui->lineEditSenha->clear();
    }
}
