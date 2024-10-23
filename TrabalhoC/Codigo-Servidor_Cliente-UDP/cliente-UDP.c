#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock.h>

#define BUFFER_SIZE 128          // Tamanho do buffer para mensagens
#define Mensagem_para_sair "#sair" // Mensagem para sair do cliente

int remote_socket = 0;           // Descritor do socket remoto
int message_length = 0;          // Comprimento da mensagem recebida

unsigned short remote_port = 0;  // N�mero da porta do servidor

char remote_ip[32];              // Endere�o IP do servidor
char message[BUFFER_SIZE];       // Buffer para mensagens

struct sockaddr_in remote_address; // Estrutura para o endere�o remoto

WSADATA wsa_data;               // Estrutura para inicializa��o do Winsock

/* 
 * Esta fun��o exibe uma mensagem de erro e sai do programa.
 * Recebe uma string de mensagem como argumento, imprime no stderr,
 * pausa para entrada do usu�rio e, em seguida, termina o programa.
 */
void msg_err_exit(char *msg)
{
    fprintf(stderr, msg);        // Imprime a mensagem de erro no stderr
    system("PAUSE");             // Pausa para que o usu�rio veja a mensagem
    exit(EXIT_FAILURE);          // Sai do programa com um status de falha
}

int main(int argc, char **argv)
{
    // Inicializa o Winsock 2.0
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
        msg_err_exit("WSAStartup() falhou\n");

    printf("IP do servidor: ");   // Solicita o IP do servidor
    scanf("%s", remote_ip);       // L� o IP a partir da entrada do usu�rio
    fflush(stdin);                // Limpa o buffer de entrada

    printf("Porta do servidor: "); // Solicita a porta do servidor
    scanf("%d", &remote_port);     // L� o n�mero da porta a partir da entrada do usu�rio
    fflush(stdin);                 // Limpa o buffer de entrada

    // Cria um socket para conex�o com o servidor (ALTERADO)
    remote_socket = socket(AF_INET, SOCK_DGRAM, 0); // Mudar SOCK_STREAM para SOCK_DGRAM
    if (remote_socket == INVALID_SOCKET)
    {
        WSACleanup();              // Limpa o Winsock
        msg_err_exit("socket() falhou\n");
    }

    // Preenchendo a estrutura remote_address para o servidor
    memset(&remote_address, 0, sizeof(remote_address)); // Limpa a estrutura
    remote_address.sin_family = AF_INET; // Define a fam�lia de endere�os como Internet
    remote_address.sin_addr.s_addr = inet_addr(remote_ip); // Define o endere�o IP do servidor
    remote_address.sin_port = htons(remote_port); // Define o n�mero da porta do servidor

    printf("digite as mensagens\n"); // Indica que o usu�rio pode come�ar a enviar mensagens
    do
    {
        // Limpa o buffer de mensagens
        memset(&message, 0, BUFFER_SIZE);

        printf("Mensagem para o servidor: "); // Solicita a mensagem para o servidor
        gets(message);                       // L� a mensagem do usu�rio (cuidado com buffer overflow)
        fflush(stdin);                       // Limpa o buffer de entrada
        message_length = strlen(message);    // Obt�m o comprimento da mensagem

        // Envia a mensagem para o servidor (ALTERADO)
        if (sendto(remote_socket, message, message_length, 0, (struct sockaddr *) &remote_address, sizeof(remote_address)) == SOCKET_ERROR) // Mudar send para sendto
        {
            WSACleanup();                  // Limpa o Winsock
            closesocket(remote_socket);    // Fecha o socket remoto
            msg_err_exit("sendto() falhou\n");
        }
    }
    // Continua at� enviar a mensagem de sa�da "#sair"
    while(strcmp(message, Mensagem_para_sair)); // Verifica se a mensagem � "#sair"

    printf("saindo\n"); // Indica que o cliente est� saindo
    WSACleanup();       // Limpa o Winsock
    closesocket(remote_socket); // Fecha o socket remoto

    system("PAUSE");   // Pausa antes de sair
    return 0;          // Retorna sucesso
}
