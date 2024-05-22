#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

void report(int cliente, int operacao, int conta, int valor) {
    cout << "ERRO CONSULTA:" << endl;
    cout << cliente << ' ' << operacao << ' ' << conta << ' ' << valor << endl;
    exit(-1);
}

int main() {
    int numClientes, numOP;
    cin >> numClientes >> numOP;
    int lines = ((numOP+1)*numClientes);

    unordered_map<int, int> Cliente_to_Conta;
    unordered_map<int, int> saldoContas;

    int cliente, operacao, conta, valor;
    while (lines--) {
        cin >> cliente >> operacao >> conta >> valor;

        switch (operacao) {
        case 0: // consulta
            if (saldoContas[Cliente_to_Conta[cliente]] != valor) 
                //report(cliente, operacao, conta, valor);
            break;

        case 1: // deposito
            saldoContas[Cliente_to_Conta[cliente]] += valor;
            break;

        case 2: // saque
            saldoContas[Cliente_to_Conta[cliente]] -= valor;
            break;

        case 9: // atribuicao conta
            Cliente_to_Conta[cliente] = conta;
            // Inicializa a conta no saldoContas se ainda não existir
            if (saldoContas.find(conta) == saldoContas.end()) {
                saldoContas[conta] = 0;
            }
            break;

        default:
            cout << "default??????" << endl;
            break;
        }
    }

    for (const auto& pair : saldoContas) {
        cout << "<conta " << pair.first << ": " << pair.second << ">  ";
    }
    cout << endl;

    return 0;
}


/*
#include <iostream>
using namespace std;

void report(int cliente,int operacao, int conta, int valor) {
    cout << "ERRO CONSULTA:" << endl;
    cout << cliente << ' ' << operacao <<  ' ' << conta <<  ' ' << valor << endl;
    exit(-1);
}

int main() {
    int numClientes, numOP; cin >> numClientes, numOP;
    int lines = ((numOP+1)*numClientes);
    int Cliente_to_Conta[numClientes] = {};
    int saldoContas[numClientes]      = {};

    int cliente, operacao, conta, valor;
    while (lines--) {
        cin >> cliente >> operacao >> conta >> valor;
        
        switch (operacao) {
        case 0: // consulta
            //if(saldoContas[Cliente_to_Conta[cliente]] != valor) report(cliente, operacao, conta, valor);
            break;
        
        case 1:  // deposito
            saldoContas[Cliente_to_Conta[cliente]] += valor;
            break;

        case 2: // saque
            saldoContas[Cliente_to_Conta[cliente]] -= valor;
            break;

        case 9: // atribuicao cont
            Cliente_to_Conta[cliente] = conta;
            break;

        default:
            cout << "default??????" << endl;
            break;
        }
    }
    
    for (int i = 0; i < numClientes; i++) printf("<conta %d: %d>  ", i, saldoContas[i]); // informa o saldo final de cada conta para conferir o funcionamento, caso desejado
    printf("\n");
    
    return 0;
}
*/