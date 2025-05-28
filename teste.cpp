#include<iostream>
#include<string>

using namespace std;

class Atendente{
    public:
        string nome;

        Atendente(string nome, string senha){
            this->nome = nome;
            setarSenha(senha);
        }

        bool autenticarLogin( string login){
            bool x = true;
            do{
                
                if(this->nome == login){
                    cout<<"Informe a senha: ";
                    string senhaLogin; 
                    cin>> senhaLogin;
                    autenticarSenha(senhaLogin);
                    return true;
                }else{
                    cout<<"Não a nenhum login igual a este"<<endl;
                    x = false;
                }
            } while (!x);
        }

    private:
        string senha;

        void setarSenha(string senha){
            if(this->senha.empty()){
                this->senha = senha;
            }else{
                cout<<"Impossivel setar senha"<<endl;
            }
        };

        bool autenticarSenha(string senha){
            if(this->senha == senha){
                return true;
            }else{
                return false;
            }

        };


};



int main(void){
    Atendente *at1 = new Atendente("Atendente1", "senha1");

    at1->autenticarLogin("Atendente1");

    return 0;
}