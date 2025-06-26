#ifndef BACKEND_H
#define BACKEND_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdio>

// Usei using namespace std aqui para ser consistente com seu código original.
// Em projetos maiores, é melhor evitar.
using namespace std;

// ================== STRATEGY: DESCONTOS ==================

class Desconto {
public:
    virtual float aplicar(float valor) = 0;
    virtual string descricao() = 0;
    virtual ~Desconto() = default;
};

class SemDesconto : public Desconto {
public:
    float aplicar(float valor) override;
    string descricao() override;
};

class DescontoVIP : public Desconto {
public:
    float aplicar(float valor) override;
    string descricao() override;
};

class DescontoBaixa : public Desconto {
public:
    float aplicar(float valor) override;
    string descricao() override;
};

// ================== ATENDENTE ==================

class Atendente {
private:
    string nome;
    string senha;

public:
    Atendente(string nome, string senha);
    bool autenticar(string senhaInformada);
    string getNome();
    void alterarSenha(string novaSenha);
};

// ================== CALENDARIO ==================

class Calendario {
public:
    int calendario[12][31]; // Removida a inicialização aqui, é feita no .cpp
    Calendario();
    bool disponibilidadeCalendario(string data, int dias);
    void marcarComoOcupado(string data, int dias);
};

// ================== ESTRUTURA RESERVA ==================

class Reserva {
public:
    string atendente;
    string cliente;
    string cpf;
    string local;
    string tipoQuarto;
    string data;
    int diarias;
    string desc;
    float valorTotal;
    float entrada;
    bool confirmada;
};

// ================== SINGLETON: BANCO DE RESERVAS ==================

class BancoDeReservas {
private:
    static BancoDeReservas* instancia;
    Calendario* calendario;
    vector<Reserva> reservas;
    vector<Atendente> atendentesRegistrados;

    BancoDeReservas();

public:
    static BancoDeReservas* getInstancia();

    BancoDeReservas(const BancoDeReservas&) = delete;
    void operator=(const BancoDeReservas&) = delete;

    Atendente* autenticar(const string& login, const string& senha);
    const vector<Reserva>& getReservas() const;
    void criarNovaReserva(const Reserva& r);
    string verificarECancelarReservaPendente(const string& chave, const string& data, int diarias);
    void gerarArquivo();
};

#endif // BACKEND_H
