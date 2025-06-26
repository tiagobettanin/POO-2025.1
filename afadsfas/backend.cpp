#include "backend.h"

// ================== IMPLEMENTAÇÃO: DESCONTOS ==================

float SemDesconto::aplicar(float valor) {
    return valor;
}
string SemDesconto::descricao() {
    return "Sem desconto";
}

float DescontoVIP::aplicar(float valor) {
    return valor * 0.9f;
}
string DescontoVIP::descricao() {
    return "Cliente VIP (10%)";
}

float DescontoBaixa::aplicar(float valor) {
    return valor * 0.8f;
}
string DescontoBaixa::descricao() {
    return "Baixa temporada (20%)";
}

// ================== IMPLEMENTAÇÃO: ATENDENTE ==================

Atendente::Atendente(string nome, string senha) {
    this->nome = nome;
    this->senha = senha;
}

bool Atendente::autenticar(string senhaInformada) {
    return senhaInformada == senha;
}

string Atendente::getNome() {
    return nome;
}

void Atendente::alterarSenha(string novaSenha) {
    senha = novaSenha;
}

// ================== IMPLEMENTAÇÃO: CALENDARIO ==================

Calendario::Calendario() {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 31; j++) {
            calendario[i][j] = 0; // 0 = livre
        }
    }
    // Lógica para dias inválidos (simplificada para o exemplo)
    // -1 = dia não existe
    calendario[1][28] = -1; calendario[1][29] = -1; calendario[1][30] = -1; // Fev
    calendario[3][30] = -1; // Abr
    calendario[5][30] = -1; // Jun
    calendario[8][30] = -1; // Set
    calendario[10][30] = -1; // Nov
}

bool Calendario::disponibilidadeCalendario(string data, int dias) {
    int dia = stoi(data.substr(0, 2)) - 1;
    int mes = stoi(data.substr(3, 2)) - 1;

    for (int i = 0; i < dias; i++) {
        if (mes >= 12 || dia >= 31 || calendario[mes][dia] == 1 || calendario[mes][dia] == -1) {
            return false; // Ocupado ou dia inválido
        }
        dia++;
        if (dia >= 31 || calendario[mes][dia] == -1) {
            mes++;
            dia = 0;
            if (mes >= 12) return false; // Passou do fim do ano
        }
    }
    return true;
}

void Calendario::marcarComoOcupado(string data, int dias) {
    int dia = stoi(data.substr(0, 2)) - 1;
    int mes = stoi(data.substr(3, 2)) - 1;

    for (int i = 0; i < dias; i++) {
        if (mes < 12 && dia < 31 && calendario[mes][dia] == 0) {
            calendario[mes][dia] = 1; // 1 = ocupado
        }
        dia++;
        if (dia >= 31 || (mes < 12 && calendario[mes][dia] == -1) ) {
            mes++;
            dia = 0;
        }
    }
}


// ================== IMPLEMENTAÇÃO: BANCO DE RESERVAS (SINGLETON) ==================

// Inicialização do ponteiro estático
BancoDeReservas* BancoDeReservas::instancia = nullptr;

// Construtor privado
BancoDeReservas::BancoDeReservas() {
    // Inicializa o calendário
    this->calendario = new Calendario();

    // Inicializa a lista de atendentes que antes estava no main
    atendentesRegistrados.push_back(Atendente("atendente1", "senha1"));
    atendentesRegistrados.push_back(Atendente("atendente2", "senha2"));
    atendentesRegistrados.push_back(Atendente("atendente3", "senha3"));
    atendentesRegistrados.push_back(Atendente("atendente4", "senha4"));
}

// Método estático para obter a instância
BancoDeReservas* BancoDeReservas::getInstancia() {
    if (!instancia) {
        instancia = new BancoDeReservas();
    }
    return instancia;
}

// NOVO MÉTODO: Autenticação para a GUI
Atendente* BancoDeReservas::autenticar(const string& login, const string& senha) {
    for (Atendente& atendente : atendentesRegistrados) {
        if (atendente.getNome() == login) {
            if (atendente.autenticar(senha)) {
                return &atendente; // Retorna ponteiro para o atendente autenticado
            } else {
                return nullptr; // Senha incorreta
            }
        }
    }
    return nullptr; // Login não encontrado
}

// MÉTODO ADAPTADO: Verifica disponibilidade, avisando sobre reservas pendentes
string BancoDeReservas::verificarECancelarReservaPendente(const string& chave, const string& data, int diarias) {
    for (int i = 0; i < reservas.size(); ++i) {
        string chaveReserva = reservas[i].local + "_" + reservas[i].tipoQuarto + "_" + reservas[i].data;
        if (chaveReserva == chave) {
            if (reservas[i].confirmada) {
                return "Indisponível: Já existe uma reserva CONFIRMADA para esta data e quarto.";
            } else {
                string msg = "Aviso: A reserva pendente do cliente " + reservas[i].cliente + " (CPF: " + reservas[i].cpf + ") foi cancelada por falta de pagamento.";
                reservas.erase(reservas.begin() + i); // Remove a reserva pendente antiga
                // A verificação de calendário continua para a nova reserva
                if(!calendario->disponibilidadeCalendario(data, diarias)){
                    return "Indisponível: O período solicitado não está mais disponível no calendário.";
                }
                return msg; // Retorna a mensagem de aviso sobre o cancelamento
            }
        }
    }
    // Se não encontrou nenhuma reserva para essa chave, checa o calendário
    if(!calendario->disponibilidadeCalendario(data, diarias)){
        return "Indisponível: O período solicitado conflita com outra reserva no calendário.";
    }

    return "Disponível"; // Nenhuma reserva conflitante encontrada
}


// NOVO MÉTODO: Adiciona a reserva e marca no calendário
void BancoDeReservas::criarNovaReserva(const Reserva& r) {
    this->reservas.push_back(r);
    // Se a reserva foi confirmada, ocupa o calendário
    if(r.confirmada){
        calendario->marcarComoOcupado(r.data, r.diarias);
    }
}


// NOVO MÉTODO: Retorna a lista de reservas
const vector<Reserva>& BancoDeReservas::getReservas() const {
    return reservas;
}

// MÉTODO MANTIDO: Gerar o arquivo de texto
void BancoDeReservas::gerarArquivo() {
    FILE *arquivo = fopen("dados_reservas.txt", "w");
    if (arquivo == NULL) {
        // No mundo real, trataríamos esse erro de forma mais robusta
        return;
    }

    fprintf(arquivo, "\n=== Relatório de Reservas ===\n\n");

    for (const Reserva& r : reservas) {
        fprintf(arquivo, "Atendente: %s\nCliente: %s - CPF: %s\nLocal: %s, Quarto: %s\nCheck-in: %s, Diárias: %d\nDesconto: %s\nValor total: R$%.2f, Entrada: R$%.2f\nStatus: %s\n\n",
                r.atendente.c_str(), r.cliente.c_str(), r.cpf.c_str(), r.local.c_str(), r.tipoQuarto.c_str(), r.data.c_str(), r.diarias, r.desc.c_str(),
                r.valorTotal, r.entrada, r.confirmada ? "Confirmada" : "Pendente");
    }

    fclose(arquivo);
}
