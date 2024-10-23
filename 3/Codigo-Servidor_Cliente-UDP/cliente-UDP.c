#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFFER_SIZE 128          // Tamanho do buffer para mensagens
#define Mensagem_para_sair "#sair" // Mensagem para sair do cliente

int remote_socket = 0;           // Descritor do socket remoto
int message_length = 0;          // Comprimento da mensagem recebida

unsigned short remote_port = 0;  // Número da porta do servidor

char remote_ip[32];              // Endereço IP do servidor
char message[BUFFER_SIZE];       // Buffer para mensagens

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

    printf("IP do servidor: ");   // Solicita o IP do servidor
    scanf("%s", remote_ip);       // Lê o IP a partir da entrada do usuário
    fflush(stdin);                // Limpa o buffer de entrada

    printf("Porta do servidor: "); // Solicita a porta do servidor
    scanf("%d", &remote_port);     // Lê o número da porta a partir da entrada do usuário
    fflush(stdin);                 // Limpa o buffer de entrada

    // Cria um socket para conexão com o servidor (ALTERADO)
    remote_socket = socket(AF_INET, SOCK_DGRAM, 0); // Mudar SOCK_STREAM para SOCK_DGRAM
    if (remote_socket == INVALID_SOCKET)
    {
        WSACleanup();              // Limpa o Winsock
        msg_err_exit("socket() falhou\n");
    }

    // Preenchendo a estrutura remote_address para o servidor
    memset(&remote_address, 0, sizeof(remote_address)); // Limpa a estrutura
    remote_address.sin_family = AF_INET; // Define a família de endereços como Internet
    remote_address.sin_addr.s_addr = inet_addr(remote_ip); // Define o endereço IP do servidor
    remote_address.sin_port = htons(remote_port); // Define o número da porta do servidor

    printf("digite as mensagens\n"); // Indica que o usuário pode começar a enviar mensagens
    do
    {
        // Limpa o buffer de mensagens
        memset(&message, 0, BUFFER_SIZE);

        printf("Mensagem para o servidor: "); // Solicita a mensagem para o servidor
        gets(message);                       // Lê a mensagem do usuário (cuidado com buffer overflow)
        fflush(stdin);                       // Limpa o buffer de entrada
        message_length = strlen(message);    // Obtém o comprimento da mensagem

        // Envia a mensagem para o servidor (ALTERADO)
        if (sendto(remote_socket, message, message_length, 0, (struct sockaddr *) &remote_address, sizeof(remote_address)) == SOCKET_ERROR) // Mudar send para sendto
        {
            WSACleanup();                  // Limpa o Winsock
            closesocket(remote_socket);    // Fecha o socket remoto
            msg_err_exit("sendto() falhou\n");
        }
    }
    // Continua até enviar a mensagem de saída "#sair"
    while(strcmp(message, Mensagem_para_sair)); // Verifica se a mensagem é "#sair"

    printf("saindo\n"); // Indica que o cliente está saindo
    WSACleanup();       // Limpa o Winsock
    closesocket(remote_socket); // Fecha o socket remoto

    system("PAUSE");   // Pausa antes de sair
    return 0;          // Retorna sucesso
}
