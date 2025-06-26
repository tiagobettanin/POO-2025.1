#include "janela2.h"
#include "ui_janela2.h"
#include <QString> // Adicionado para garantir que QString funcione

// O construtor recebe o atendente que veio da MainWindow
Janela2::Janela2(Atendente* atendente, QWidget *parent) :
    QDialog(parent), // CORREÇÃO: Chamando o construtor do pai correto (QDialog)
    ui(new Ui::Janela2)
{
    ui->setupUi(this);
    this->atendenteLogado = atendente; // Guarda o atendente

    // Define um título para a janela e uma mensagem de boas-vindas
    this->setWindowTitle("Tela Principal");

    // Constrói uma mensagem de boas-vindas para confirmar que o login funcionou
    QString mensagem = "Login bem-sucedido! Bem-vindo(a), " + QString::fromStdString(atendenteLogado->getNome());

    // A linha abaixo só funcionará se você tiver um QLabel com o objectName "labelBoasVindas" no seu .ui
    if (ui->labelBoasVindas) { // Verificação para evitar crash se o label não existir
        ui->labelBoasVindas->setText(mensagem);
    }
}

Janela2::~Janela2()
{
    delete ui;
}
