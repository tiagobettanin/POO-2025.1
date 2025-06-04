#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <csignal>
#include <cstdlib>

using namespace std;

// ================== PADRÃO STRATEGY: DESCONTOS ==================

class Desconto {
public:
    virtual float aplicar(float valor) = 0;
    virtual string descricao() = 0;
    virtual ~Desconto() = default;
};

class SemDesconto : public Desconto {
public:
    float aplicar(float valor) override { 
        return valor; 
    }
    string descricao() override { return "Sem desconto"; }
};

class DescontoVIP : public Desconto {
public:
    float aplicar(float valor) override { return valor * 0.9f; }
    string descricao() override { return "Cliente VIP (10%)"; }
};

class DescontoBaixa : public Desconto {
public:
    float aplicar(float valor) override { return valor * 0.8f; }
    string descricao() override { return "Baixa temporada (20%)"; }
};

// ================== CLASSE ATENDENTE ==================

class Atendente {
private:
    string nome;
    string senha;

public:
    Atendente(const string& nome, const string& senha) : nome(nome), senha(senha) {}

    bool autenticar(const string& senhaInformada) const {
        return senhaInformada == senha;
    }

    string getNome() const {
        return nome;
    }

    void alterarSenha(const string& novaSenha) {
        senha = novaSenha;
    }
};

// ================== ESTRUTURA E SINGLETON DE RESERVAS ==================

struct Reserva {
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

class BancoDeReservas {
private:
    static BancoDeReservas* instancia;
    vector<Reserva> reservas;
    vector<string> ocupados;

    BancoDeReservas() {}

public:
    static BancoDeReservas* getInstancia() {
        if (!instancia)
            instancia = new BancoDeReservas();
        return instancia;
    }

    bool disponivel(const string& chave) {  
        for (int i = 0; i < reservas.size(); ++i) {
            string chaveReserva = reservas[i].local + "_" + reservas[i].tipoQuarto + "_" + reservas[i].data;

            if (chaveReserva == chave && reservas[i].confirmada) {
                return false;
            }

            if (chaveReserva == chave && !reservas[i].confirmada) {
                reservas[i].cliente = "Reserva Cancelada por falta de pagamento";
                return true;
            }
        }
         return true;
    }


    void reservar(const Reserva& r) {
        string chave = r.local + "_" + r.tipoQuarto + "_" + r.data;
        ocupados.push_back(chave);
        reservas.push_back(r);
    }

    void mostrarReservas() {
        cout << "\n=== Reservas realizadas ===\n";
        for (const Reserva& r : reservas) {
            cout << "Atendente: " << r.atendente << "\n"
                 << "Cliente: " << r.cliente << " - CPF: " << r.cpf << "\n"
                 << "Local: " << r.local << ", Quarto: " << r.tipoQuarto << "\n"
                 << "Check-in: " << r.data << ", Diárias: " << r.diarias << "\n"
                 << "Desconto: " << r.desc << "\n"
                 << "Valor total: R$" << r.valorTotal << ", Entrada: R$" << r.entrada << "\n"
                 << "Status: " << (r.confirmada ? "Confirmada" : "Pendente") << "\n\n";
        }
    }
};

BancoDeReservas* BancoDeReservas::instancia = nullptr;

// ================== FUNÇÃO DE RESERVA ==================

void fazerReserva(Atendente* at) {
    string local, tipo, data, nome, cpf;
    int diarias, opc;

    try {
        cout << "\nLocal (sem espaços. Jericoacoara, Canoa Quebrada ou Cumbuco: ";
        cin >> local;

        cout << "Data de check-in (DD/MM/AAAA): ";
        cin >> data;

        cout << "Tipo de quarto (Solteiro, Duplo, Casal): ";
        cin >> tipo;

        cout << "Número de diárias: ";
        cin >> diarias;

        cout << "Nome do cliente (sem espaços): ";
        cin >> nome;

        cout << "CPF do cliente: ";
        cin >> cpf;

        cout << "Tipo de desconto:\n0 - Nenhum\n1 - VIP\n2 - Baixa Temporada\nOpção: ";
        cin >> opc;

        Desconto* desconto;
        if (opc == 1)
            desconto = new DescontoVIP();
        else if (opc == 2)
            desconto = new DescontoBaixa();
        else
            desconto = new SemDesconto();

        float diaria = 0;
        if (tipo == "Solteiro") diaria = 200;
        else if (tipo == "Duplo") diaria = 300;
        else if (tipo == "Casal") diaria = 350;

        string chave = local + "_" + tipo + "_" + data;

        BancoDeReservas* banco = BancoDeReservas::getInstancia();
        if (!banco->disponivel(chave)) {
            cout << ">> Quarto já reservado nessa data!\n";
            delete desconto;
            return;
        }

        float total = desconto->aplicar(diaria * diarias);
        float entrada = total / 3.0f;

        cout << "\nResumo da reserva:\n";
        cout << "Cliente: " << nome << ", CPF: " << cpf << "\n";
        cout << "Hotel: " << local << ", Quarto: " << tipo << "\n";
        cout << "Data: " << data << ", Diárias: " << diarias << "\n";
        cout << "Desconto: " << desconto->descricao() << "\n";
        cout << "Valor total: R$" << total << ", Entrada: R$" << entrada << "\n";

        char confirmacao;
        cout << "Confirmar reserva com pagamento da entrada? (s/n): ";
        cin >> confirmacao;

        Reserva r;
        r.atendente = at->getNome();
        r.cliente = nome;
        r.cpf = cpf;
        r.local = local;
        r.tipoQuarto = tipo;
        r.data = data;
        r.diarias = diarias;
        r.valorTotal = total;
        r.entrada = entrada;
        r.desc = desconto->descricao();
        r.confirmada = (confirmacao == 's' || confirmacao == 'S');

        banco->reservar(r);
        banco->mostrarReservas();

        delete desconto;
    } catch (exception& e) {
        cout << "Erro: " << e.what() << "\nReserva cancelada.\n";
    }
}

// ================== CONTROLE PARA SAIR ==================

void ctrlZ(int sinal) {
    exit(0);
}

// ================== GERENCIAMENTO DE ATENDENTES ==================

struct AtendenteRegistro {
    string nome;
    string senha;
};

Atendente* autenticarAtendente(const vector<AtendenteRegistro>& registros) {
    string login, senhaEntrada;
    cout << "Informe o login: ";
    cin >> login;

    for (const auto& reg : registros) {
        if (reg.nome == login) {
            cout << "Informe a senha: ";
            cin >> senhaEntrada;
            if (senhaEntrada == reg.senha) {
                return new Atendente(reg.nome, reg.senha);
            } else {
                cout << "Senha incorreta!" << endl;
                return nullptr;
            }
        }
    }
    cout << "Login inválido!" << endl;
    return nullptr;
}

// ================== MENU PRINCIPAL ==================

void menuPrincipal(Atendente* at) {
    if (at == nullptr) return;

    while (true) {
        cout << "\n=== Menu Principal ===\n";
        cout << "1 - Realizar reserva\n";
        cout << "2 - Consultar reservas\n";
        cout << "0 - Trocar de conta / Sair\n";
        cout << "Opção: ";

        int opcao;
        cin >> opcao;

        switch (opcao) {
            case 1:
                fazerReserva(at);
                break;
            case 2:
                BancoDeReservas::getInstancia()->mostrarReservas();
                break;
            case 0:
                delete at;
                return;
            default:
                cout << "Opção inválida!\n";
        }
    }
}

// ================== FUNÇÃO MAIN ==================

int main() {
    signal(SIGTSTP, ctrlZ);
    cout << "Pressione Ctrl+Z para sair\n";

    // Cadastro fixo de atendentes
    vector<AtendenteRegistro> atendentesRegistrados = {
        {"atendente1", "senha1"},
        {"atendente2", "senha2"},
        {"atendente3", "senha3"},
        {"atendente4", "senha4"}
    };

    while (true) {
        Atendente* atendenteAtual = autenticarAtendente(atendentesRegistrados);
        if (atendenteAtual != nullptr) {
            menuPrincipal(atendenteAtual);
        }
        cout << "\n--- Novo login ---\n";
    }

    return 0;
}
