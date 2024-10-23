#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BACKLOG_MAX 5            // Número máximo de conexões na fila
#define BUFFER_SIZE 128          // Tamanho do buffer para mensagens
#define Mensagem_para_sair "#sair" // Mensagem para sair do servidor

int local_socket = 0;           // Descritor do socket local
// int remote_socket = 0;       // Descritor do socket remoto (remover essa variável)

int remote_length = 0;          // Comprimento da estrutura do endereço remoto
int message_length = 0;         // Comprimento da mensagem recebida

unsigned short local_port = 0;  // Número da porta local
unsigned short remote_port = 0; // Número da porta remota

char message[BUFFER_SIZE];      // Buffer para mensagens recebidas

struct sockaddr_in local_address; // Estrutura para o endereço local
struct sockaddr_in remote_address; // Estrutura para o endereço remoto

WSADATA wsa_data;               // Estrutura para inicialização do Winsock

/* 
 * Esta função exibe uma mensagem de erro e sai do programa.
 * Recebe uma string de mensagem como argumento, imprime no stderr,
 * pausa para entrada do usuário e, em seguida, termina o programa.
 */
void msg_err_exit(char *msg)
{
    fprintf(stderr, msg);        // Imprime a mensagem de erro no stderr
    system("PAUSE");             // Pausa para que o usuário veja a mensagem
    exit(EXIT_FAILURE);          // Sai do programa com um status de falha
}

int main(int argc, char **argv)
{
    // Inicializa o Winsock 2.0
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() falhou\n");

    // Cria um socket local para o servidor (ALTERADO)
    local_socket = socket(AF_INET, SOCK_DGRAM, 0); // Mudar SOCK_STREAM para SOCK_DGRAM
    if (local_socket == INVALID_SOCKET)
    {
        WSACleanup();              // Limpa o Winsock
        msg_err_exit("socket() falhou\n");
    }

    printf("Porta local: ");       // Solicita o número da porta local
    scanf("%d", &local_port);      // Lê o número da porta a partir da entrada do usuário
    fflush(stdin);                 // Limpa o buffer de entrada

    // Limpa a estrutura local_address
    memset(&local_address, 0, sizeof(local_address));

    // Define a família de endereços como Internet
    local_address.sin_family = AF_INET;

    // Define o número da porta local
    local_address.sin_port = htons(local_port);

    // Usa qualquer endereço IP local (INADDR_ANY)
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Liga o socket ao endereço local
    if (bind(local_socket, (struct sockaddr *) &local_address, sizeof(local_address)) == SOCKET_ERROR)
    {
        WSACleanup();              // Limpa o Winsock
        closesocket(local_socket); // Fecha o socket
        msg_err_exit("bind() falhou\n");
    }

    // Não precisa do listen para UDP (remover linhas 51-53)
    // Coloca o socket para escutar conexões
    // if (listen(local_socket, BACKLOG_MAX) == SOCKET_ERROR)
    // {
    //     WSACleanup();              // Limpa o Winsock
    //     closesocket(local_socket); // Fecha o socket
    //     msg_err_exit("listen() falhou\n");
    // }

    remote_length = sizeof(remote_address); // Prepara para aceitar um endereço remoto

    printf("aguardando alguma mensagem...\n"); // Indica que o servidor está esperando por uma mensagem

    // Loop para receber mensagens (remover accept)
    do
    {
        // Limpa o buffer de mensagens
        memset(&message, 0, BUFFER_SIZE);

        // Recebe uma mensagem do cliente (ALTERADO)
        message_length = recvfrom(local_socket, message, BUFFER_SIZE, 0, (struct sockaddr *) &remote_address, &remote_length); // Mudar recv para recvfrom
        if(message_length == SOCKET_ERROR)
            msg_err_exit("recvfrom() falhou\n");

        // Exibe a mensagem recebida
        printf("%s: %s\n", inet_ntoa(remote_address.sin_addr), message);
    }
    // Continua até receber a mensagem de saída "#sair"
    while(strcmp(message, Mensagem_para_sair));

    printf("encerrando\n"); // Indica que o servidor está se desligando
    WSACleanup();           // Limpa o Winsock
    closesocket(local_socket); // Fecha o socket local
    // Não precisa de remote_socket aqui (remover linha 64)

    system("PAUSE");       // Pausa antes de sair
    return 0;              // Retorna sucesso
}
