#include<iostream>
#include<string>
#include<vector>

using namespace std;

class ContaBanco{
    private:
        int numConta;
        string tipo;
        double saldo;
        bool status;
        string dono; 
    
    public:
        ContaBanco(int numero, const string& tipo, double saldo, bool status, string dono){
            this->numConta = numero;
            this->tipo = tipo;
            this->saldo = 0.0;
            this->status = false;
            this->dono = dono;

        };
        //~ContaBanco(){};    

        void abrirConta(){
            if(this->status == true) return;
            this->status = true;
            if(this->tipo == "CP"){
                this->saldo = 50.0;
            }
            else if(this->tipo == "CC"){
                this->saldo = 150.0;
            }else{
                cout<<"Conta aberta com sucesso" <<endl;
            }
            cout<<"COnta aberta com sucesso"<<endl;
            HistoricoTransacao::instace()->regsitrarTransacao("Conta "+this->tipo+" saldo"+to_string(this->saldo)+" Dono:" + this->dono);
        };

        void fecharConta(){
            if(this->status == false){
                cout<<"Conta inativa"<<endl;
                return;
            }
            if(this->saldo == 0){
                this->status = false;
                HistoricoTransacao::instace()->regsitrarTransacao("Conta de "+this->dono+" fechada com sucesso");
            }else{
                cout<<"Eror ao fechar a conta"<<endl;
                HistoricoTransacao::instace()->regsitrarTransacao("Erro ao fechar a conta de "+this->dono+" saldo diferenete de 0");
            }
        };

        void depositar(double valor){
            if(this->status == false){
                cout<<"Conta inativa deposito não realizado"<<endl;
                return;
            }
            this->saldo += valor;
            HistoricoTransacao::instace()->regsitrarTransacao("Conta de "+this->dono+" obteve um deposidto de " + to_string(valor));



        };

        void sacar (double valor){
            if(this->status == false){
                cout<<"Conta inativa! Saque nao realizado";
                return;
            }

            if(valor <= this->saldo){
                this->saldo = valor;
                HistoricoTransacao::instace()->regsitrarTransacao("Conta de "+this->dono+" obteve um saque de " + to_string(valor));
            }
            else{
                cout<<"Saldo insuficiente para o saque. Saldo de: "<<this->saldo;
                HistoricoTransacao::instace()->regsitrarTransacao("Conta de "+this->dono+" tendou um saque de " + to_string(valor) + "mas o saldo eh insuficiente");
            }
        }

        void pagarMensal(){
            this->saldo  -= this->tipo == "CP" ? 20.0 : 12.0;
            HistoricoTransacao::instace()->regsitrarTransacao("Pagou mensalidade, dono" + this->dono);
        }
 
};


class HistoricoTransacao{//utilza padrao singleton
    public: 
        static HistoricoTransacao *instace(void){
            if(!p){
                p=new HistoricoTransacao();
            }
            return p;
        }

        void regsitrarTransacao( string& msg){
            this->transacoes.push_back(msg);
        };

        void exibirHistorico(){
            cout<<"\n\n===============================\n\n" ;
            cout<<"\n\n     Historico de Transaçoes     \n\n" ;
            cout<<"\n\n===============================\n\n" ;
            
            for(const auto& msg: transacoes){
                cout<<"\n- " <<msg;
            }

        };

    private:    
        HistoricoTransacao(){ cout<<"\nSingleton carregado!\n"; };
        static HistoricoTransacao *p;
        vector<string> transacoes;
};

HistoricoTransacao* HistoricoTransacao::p=nullptr;

int main(void){
    ContaBanco c1( 1, "CC", 0.0, 1,  "ana");
    ContaBanco c2( 11, "CP", 0.0, 1,  "anjoão");

    c1.abrirConta();
    c2.abrirConta();

    c1.depositar(100);
    c2.sacar(30);

    c1.pagarMensal();
    c2.pagarMensal();

    c2.sacar(20);

    HistoricoTransacao::instace()->exibirHistorico();

    return 0;
}