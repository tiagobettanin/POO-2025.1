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
public:
    string nome;

    Atendente(string nome, string senha) {
        this->nome = nome;
        setarSenha(senha);
    }

    

    bool autenticarLogin(string login) {
        if (this->nome == login) {
            cout << "Informe a senha: ";
            string senhaLogin;
            cin >> senhaLogin;
            if (autenticarSenha(senhaLogin)) {
                return true;
            } else {
                cout << "Senha incorreta!" << endl;
                return false;
            }
        } else {
            cout << "Login inválido!" << endl;
            return false;
        }
    }

private:
    string senha;

    void setarSenha(string senha) {
        if (this->senha.empty()) {
            this->senha = senha;
        } else {
            cout << "Senha já definida." << endl;
        }
    }

    bool autenticarSenha(string senha) {
        return this->senha == senha;
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

    BancoDeReservas() {} // construtor privado

public:
    static BancoDeReservas* getInstancia() {
        if (!instancia)
            instancia = new BancoDeReservas();
        return instancia;
    }

    bool disponivel(string chave) {
        for (string k : ocupados) {
            if (k == chave) return false;
        }
        return true;
    }

    void reservar(Reserva r) {
        string chave = r.local + "_" + r.tipoQuarto + "_" + r.data;
        ocupados.push_back(chave);
        reservas.push_back(r);
    }

    void mostrarReservas() {
        cout << "\n=== Reservas realizadas ===\n";
        for (Reserva& r : reservas) {
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
        cout << "\nLocal (sem espaços, ex: Jericoacoara): ";
        cin >> local;

        cout << "Data de check-in (DD/MM/AAAA): ";
        cin >> data;

        cout << "Tipo de quarto (Solteiro, Duplo, Casal): ";
        cin >> tipo;

        cout << "Número de diárias: ";
        cin >> diarias;
        if (diarias <= 0) throw invalid_argument("Diárias deve ser maior que 0.");

        cout << "Nome do cliente (sem espaços): ";
        cin >> nome;

        cout << "CPF do cliente: ";
        cin >> cpf;

        cout << "Tipo de desconto:\n0 - Nenhum\n1 - VIP\n2 - Baixa Temporada\nOpção: ";
        cin >> opc;
        if (opc < 0 || opc > 2) throw invalid_argument("Opção inválida.");

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
        else throw invalid_argument("Tipo de quarto inválido.");

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
        r.atendente = at->nome;
        r.cliente = nome;
        r.cpf = cpf;
        r.local = local;
        r.tipoQuarto = tipo;
        r.data = data;
        r.diarias = diarias;
        r.valorTotal = total;
        r.entrada = entrada;
        r.desc = desconto->descricao();
        r.confirmada = (confirmacao == 's');

        banco->reservar(r);
        banco->mostrarReservas();

        delete desconto;
    } catch (exception& e) {
        cout << "Erro: " << e.what() << "\nReserva cancelada.\n";
    }
}

// ================== ENCERRAR PROGRAMA ==================
void ctrlZ(int sinal) {
    //gerar sairda
    exit(0);
}

// ================== MENU PRINCIPAL ==================

void menuPrincipal(string nome){
    //Atendente at1 = new Atendente(nome, senha);
    //if(at1.autenticarLogin())

    while(true){
        int opcao;
        cout << "0- Trocar de Conta.\n1- Realizar reserva.\n2- Consultar reserva.\n";
        cin >> opcao;

        switch (opcao){
        case 0:
            return;
        break;
        
        default:
            break;
        }

    }

}

// ================== FUNÇÃO MAIN ==================

int main() {
    signal(SIGTSTP, ctrlZ);
    cout << "Precione Crtl+Z para sair\n";

    while (true) {
        string nome, senha;
        cout<<"Informe o login: "<<endl;
        cin>> nome;
        menuPrincipal(nome);
        
              
    }


    return 0;
}
