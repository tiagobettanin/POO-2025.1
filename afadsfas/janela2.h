#ifndef JANELA2_H
#define JANELA2_H

#include <QDialog> // CORREÇÃO: Alterado de <QWidget> para <QDialog>
#include "backend.h"

namespace Ui {
class Janela2;
}

// CORREÇÃO: A classe agora herda de QDialog, que é o que o seu arquivo .ui espera.
class Janela2 : public QDialog
{
    Q_OBJECT

public:
    explicit Janela2(Atendente* atendente, QWidget *parent = nullptr);
    ~Janela2();

private:
    Ui::Janela2 *ui;
    Atendente* atendenteLogado;
};

#endif // JANELA2_H
