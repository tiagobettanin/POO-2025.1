#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <csignal>
#include <cstdlib>
#include <cstdio>

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
    float aplicar(float valor) override {
        return valor;
    }
    string descricao() override {
        return "Sem desconto";
    }
};

class DescontoVIP : public Desconto {
public:
    float aplicar(float valor) override {
        return valor * 0.9f;
    }
    string descricao() override {
        return "Cliente VIP (10%)";
    }
};

class DescontoBaixa : public Desconto {
public:
    float aplicar(float valor) override {
        return valor * 0.8f;
    }
    string descricao() override {
        return "Baixa temporada (20%)";
    }
};

// ================== CLASSE ATENDENTE ==================

class Atendente {
private:
    string nome;
    string senha;

public:
    Atendente(string nome, string senha) {
        this->nome = nome;
        this->senha = senha;
    }

    bool autenticar(string senhaInformada) {
        return senhaInformada == senha;
    }

    string getNome() {
        return nome;
    }

    void alterarSenha(string novaSenha) {
        senha = novaSenha;
    }
};

// ================== GERENCIAMENTO DE ATENDENTES ==================

class AtendenteRegistro {
public:
    string nome;
    string senha;
};

Atendente* autenticarAtendente(vector<AtendenteRegistro> registros) {
    string login, senhaEntrada;
    cout << "Informe o login: ";
    cin >> login;

    for (auto reg : registros) {
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

// ================== CALENDARIO ==================

class Calendario{
    public:
        int calendario [12][31] = {0};

        Calendario(){
            for(int i=0; i<12; i++){
                if(i==1){
                    calendario[i][29] = calendario[i][30] = calendario[i][31] =  -1;
                }else if (i == 3 || i == 5 || i == 8 || i == 10) {
                    calendario[i][30] = -1;
                }
                else {
                    calendario[i][30] = calendario[i][31] =  -1;
                }
            }
        }

        bool disponibilidadeCalendario(string data, int dias){
            int dia = (data[0] - '0') * 10 + (data[1] - '0');
            dia--;
            int mes = (data[3] - '0') * 10 + (data[4] - '0');
            mes--;

            for(int i=0; i<dias; i++){
                if(calendario[mes][dia] == 1){
                    return false;
                }else if(calendario[mes][dia] == 0){
                    calendario[mes][dia] = 1;
                    dia++;
                }else if(calendario[mes][dia] == -1){
                    i--;
                    mes++;
                    dia=0;
                }
            }
            return true;

        }
};

// ================== ESTRUTURA E SINGLETON DE RESERVAS ==================

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

class BancoDeReservas {
private:
    static BancoDeReservas* instancia;
    Calendario *caldendario = new Calendario();
    vector<Reserva> reservas;
    vector<string> ocupados;

    BancoDeReservas() {}

public:
    static BancoDeReservas* getInstancia() {
        if (!instancia)
            instancia = new BancoDeReservas();
        return instancia;
    }

    bool disponivel(string chave) {
        for (int i = 0; i < reservas.size(); ++i) {
            string chaveReserva = reservas[i].local + "_" + reservas[i].tipoQuarto + "_" + reservas[i].data;

            if (chaveReserva == chave && reservas[i].confirmada) {
                if(caldendario->disponibilidadeCalendario( reservas[i].data, reservas[i].diarias)){
                    return false;
                }
                cout<< "Não foi possivel pois já temos uma reserva nos dias"<<endl;
                return true;
            }

            if (chaveReserva == chave && !reservas[i].confirmada) {
                cout << "Avisar o cliente " << reservas[i].cliente << "com o cpf " << reservas[i].cpf << " que a reserva dele foi cancelada" <<endl;
                reservas[i].cliente = "Reserva Cancelada por falta de pagamento";
                return true;
            }
        }
        return true;
    }

    void reservar(Reserva r) {
        string chave = r.local + "_" + r.tipoQuarto + "_" + r.data;
        this->ocupados.push_back(chave);
        this->reservas.push_back(r);
    }

    void mostrarReservas() {
        cout << "\n=== Reservas realizadas ===\n";
        for (Reserva r : reservas) {
            cout << "Atendente: " << r.atendente << "\n"
                 << "Cliente: " << r.cliente << " - CPF: " << r.cpf << "\n"
                 << "Local: " << r.local << ", Quarto: " << r.tipoQuarto << "\n"
                 << "Check-in: " << r.data << ", Diárias: " << r.diarias << "\n"
                 << "Desconto: " << r.desc << "\n"
                 << "Valor total: R$" << r.valorTotal << ", Entrada: R$" << r.entrada << "\n"
                 << "Status: " << (r.confirmada ? "Confirmada" : "Pendente") << "\n\n";
        }
    }

    void gerarArquivo() {
        FILE *arquivo = fopen("dados.txt", "w");
        if (arquivo == NULL) {
            printf("Erro ao criar o arquivo!\n");
            return;
        }

        fprintf(arquivo, "\n=== Reservas realizadas ===\n");

        for (Reserva& r : reservas) {
            fprintf(arquivo, "Atendente: %s\nCliente: %s - CPF: %s\nLocal: %s, Quarto: %s\nCheck-in: %s, Diárias: %s\nDesconto: %s\nValor total: R$%s, Entrada: R$%s\nStatus: %s\n",
                r.atendente.c_str(), r.cliente.c_str(), r.cpf.c_str(), r.local.c_str(), r.tipoQuarto.c_str(), r.data.c_str(), to_string(r.diarias).c_str(), r.desc.c_str(),
                to_string(r.valorTotal).c_str(), to_string(r.entrada).c_str(), r.confirmada ? "Confirmada" : "Pendente");
        }

        fclose(arquivo);
    }

    void fazerReserva(Atendente* at) {
        string local, tipo, data, nome, cpf;
        int diarias, opc;

        try {
            cout << "\nLocal (sem espaços. Jericoacoara, Canoa Quebrada ou Cumbuco): ";
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
            else if (tipo == "Triplo") diaria = 450;
            else if (tipo == "Quadruplo" || tipo == "Quádruplo") diaria = 550;

            string chave = local + "_" + tipo + "_" + data;

            if (!disponivel(chave)) {
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

            reservar(r);
            mostrarReservas();

            delete desconto;
        } catch (exception& e) {
            cout << "Erro: " << e.what() << "\nReserva cancelada.\n";
        }
    }
};

BancoDeReservas* BancoDeReservas::instancia = nullptr;

// ================== MENU PRINCIPAL ==================

void menuPrincipal(Atendente* at) {
    if (at == nullptr) return;

    BancoDeReservas* banco = BancoDeReservas::getInstancia();

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
                banco->fazerReserva(at);
                break;
            case 2:
                banco->mostrarReservas();
                break;
            case 0:
                delete at;
                return;
            default:
                cout << "Opção inválida!\n";
        }
    }
}

// ================== CONTROLE PARA SAIR ==================

void ctrlZ(int sinal) {
    BancoDeReservas::getInstancia()->gerarArquivo();
    exit(0);
}

// ================== FUNÇÃO MAIN ==================

int main() {
    signal(SIGTSTP, ctrlZ);
    cout << "Pressione Ctrl+Z para sair\n";

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
