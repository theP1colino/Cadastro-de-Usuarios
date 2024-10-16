#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#define maxUsers 10

struct User{
    char nome[90];
    char nickname[50];
    char idade[20];
    char email[90];
    char senha[90];
    char key[20];
};

char chars[98] = "-f:ICj5yQF*@N[#W|Xrz7(Y{.txMlacbRJ,/$ug=s_O1ohPBZV~TKi9Ek^}mL!q`&60;?+DU%H]8S3n2 veA4wGdp)";

// ###################################################################
// Funcao de criptografia para idade e chave aleatoria
void encryptionFunction1(char texto[], char textoChave[]){
   
    char caractere;
    int tamanhoInicial;
    long int chave;
    tamanhoInicial = strlen(texto);
   
    for(int i = 0; i < strlen(texto); i++){
        char index;
        int numIndex;
        index = texto[i];
        for(int j = 0; j < strlen(chars); j++){
            if(index == chars[j]){
                numIndex = j;
                break;
            }
        }
        chave = (numIndex + ((strlen(textoChave) + i)%strlen(chars))) % strlen(chars);
        texto[i] = chars[chave];
    }
}

void encryptionFunction2(char texto[], long int chave1, long int chave2){
    long int chave;

    for(int i = 0; i < strlen(texto); i++){
        char caractere;
        int index;
        caractere = texto[i];
        for (int j = 0; j < strlen(chars); j++){
            if(caractere == chars[j]){
                index = j;
                break;
            }
        }
        chave = (index + (((chave1 + chave2) + i)%strlen(chars))) % strlen(chars);
        // printf("\n\n%ld\n\n", chave);
        texto[i] = chars[chave];
    }
}

char *decryptionFunction2(char encryptedText[], long int chave1, long int chave2){
    long int chave;
    // char decryptedText[50] = "";
    char *decryptedText = (char*) malloc((strlen(encryptedText) + 1));


    // printf("%ld, %ld\n", chave1, chave2);

    for(int i = 0; i < strlen(encryptedText); i++){
        char caractere;
        int index;
        caractere = encryptedText[i];
        for(int j = 0; j < strlen(chars); j++){
            if(caractere == chars[j]){
                index = j;
                break;
            }
        }
        chave = (index - (((chave1 + chave2) + i)%strlen(chars)));
        if(chave < 0){
            chave += strlen(chars);
        }
        decryptedText[i] = chars[chave];
    }

    decryptedText[strlen(encryptedText)] = '\0';
    // printf("%s\n", decryptedText);
    return decryptedText;
}

long int decryptionFunction1(char encryptedText[], char textoChave[]){
    char caractere;
    long int chave, resultado;
    char decryptedText[20] = "";

    for(int i = 0; i < strlen(encryptedText); i++){
        char index;
        int numIndex;
        index = encryptedText[i];
        for(int j = 0; j < strlen(chars); j++){
            if(index == chars[j]){
                numIndex = j;
                break;
            }
        }
        chave = (numIndex - ((strlen(textoChave) + i)%strlen(chars)));
        if(chave < 0){
            chave += strlen(chars);
        }
        decryptedText[i] = chars[chave];
    }

    resultado = strtol(decryptedText, NULL, 10);
    
    return resultado;
}

// ###################################################################
// Função para criptografar as informações do usuário
void encryption(struct User *usuario){
    
    char keyStr[20];
    long int primaryKey, secondaryKey;
   
    primaryKey = strtol(usuario->idade, NULL, 10);
    secondaryKey = (rand() % 100) + 1;
    sprintf(keyStr, "%ld", secondaryKey);
    strcpy(usuario->key, keyStr);
    // printf("%ld\n", primaryKey);
    // printf("%ld\n", secondaryKey);
    // printf("%s\n", usuario->key);
    // tercearyKey = strlen(usuario.nome);
   
    encryptionFunction1(usuario->idade, usuario->nome);
    encryptionFunction1(usuario->key, usuario->nome);
    encryptionFunction2(usuario->nome, primaryKey, secondaryKey);
    encryptionFunction2(usuario->nickname, primaryKey, secondaryKey);
    encryptionFunction2(usuario->email, primaryKey, secondaryKey);
    encryptionFunction2(usuario->senha, primaryKey, secondaryKey);
   
    // encryptionFunction2
   
}

void testeCriptografia() {
    char nome[] = "Qualquer coisa";
    // char chave[] = "12345";
    char *decrypted;
    long int chave1 = 22;
    long int chave2 = 22;

    printf("%s\n", chars);
    printf(": %ld\n", strlen(chars));
    printf("Texto original: %s\n", nome);

    encryptionFunction2(nome, chave1, chave2);
    printf("Texto criptografado: %s\n", nome);

    decrypted = decryptionFunction2(nome, chave1, chave2);
    printf("Texto descriptografado: %s\n", decrypted);
    free(decrypted);
}

void listarUsuarios(char arquivo[]){
    struct User usuario;
    long int chave1, chave2;
    char *decrypted;
    FILE *ponteiroArquivo = fopen(arquivo, "r");
    if(ponteiroArquivo == NULL){
        printf("Erro na abertura do arquivo.\n");
        return;
    }

    while(fgets(usuario.nome, sizeof(usuario.nome), ponteiroArquivo) != NULL){
        usuario.nome[strcspn(usuario.nome, "\n")] = '\0';

        fgets(usuario.nickname, sizeof(usuario.nickname), ponteiroArquivo);
        usuario.nickname[strcspn(usuario.nickname, "\n")] = '\0';

        fgets(usuario.idade, sizeof(usuario.idade), ponteiroArquivo);
        usuario.idade[strcspn(usuario.idade, "\n")] = '\0';

        fgets(usuario.email, sizeof(usuario.email), ponteiroArquivo);
        usuario.email[strcspn(usuario.email, "\n")] = '\0';

        fgets(usuario.senha, sizeof(usuario.senha), ponteiroArquivo);
        usuario.senha[strcspn(usuario.senha, "\n")] = '\0';

        fgets(usuario.key, sizeof(usuario.key), ponteiroArquivo);
        usuario.key[strcspn(usuario.key, "\n")] = '\0';

        chave1 = decryptionFunction1(usuario.idade, usuario.nome);
        chave2 = decryptionFunction1(usuario.key, usuario.nome);

        printf("Nome: %s\n", usuario.nome);
        decrypted = decryptionFunction2(usuario.nome, chave1, chave2);
        printf("Nome descriptografado: %s\n", decrypted);
        free(decrypted);

        printf("Nickname: %s\n", usuario.nickname);
        decrypted = decryptionFunction2(usuario.nickname, chave1, chave2);
        printf("Nickname descriptografado: %s\n", decrypted);
        free(decrypted);

        printf("Idade criptografada: %s\n", usuario.idade);
        printf("Idade descriptografada: %ld\n", chave1);

        printf("E-mail: %s\n", usuario.email);
        decrypted = decryptionFunction2(usuario.email, chave1, chave2);
        printf("E-mail descriptografado: %s\n", decrypted);
        free(decrypted);

        printf("Senha: %s\n", usuario.senha);
        decrypted = decryptionFunction2(usuario.senha, chave1, chave2);
        printf("Senha descriptografada: %s\n", decrypted);
        free(decrypted);

        printf("Key: %s\n", usuario.key);
        printf("Key descriptografada: %ld\n", chave2);

        printf("\n");
    }
    fclose(ponteiroArquivo);
}

// ###################################################################
// Login de usuario
/*
void buscarLogin(struct User usuarios[], int totalUsuarios){
    char userInput[90] = "", senha[90] = "";
    int authentication = 0, i = 0, t = 1;
    do{
        printf("Digite seu nickname ou seu e-mail: ");
        fgets(userInput, 89, stdin);
        fflush(stdin);
        printf("Agora digite sua senha: ");
        fgets(senha, 89, stdin);
        fflush(stdin);
       
        for(i; i < totalUsuarios; i++){
            long int chave1 = 0, chave2 = 0;
            if((strlen(userInput) == strlen(usuarios[i].nickname) || strlen(userInput) == strlen(usuarios[i].email)) && strlen(senha) == strlen(usuarios[i].senha)){
                chave1 = decryptionFunction1(usuarios[i].idade, usuarios[i].nome);
                chave2 = decryptionFunction1(usuarios[i].key, usuarios[i].nome);
                if(strcmp(userInput, usuarios[i].nickname) == 0 || strcmp(userInput, usuarios[i].email) == 0){
                if(strcmp(senha, usuarios[i].senha) == 0){
                    authentication = 1;
                    i = i;
                    break;
                }
            }
            }
        }
       
        if(authentication == 1){
            printf("\n\nBem vindo %s!\n\n", usuarios[i].nome);
            printf("Informações:\n");
            printf("Nickname: %s\n", usuarios[i].nickname);
            printf("Idade: %s\n", usuarios[i].idade);
            printf("E-mail: %s\n", usuarios[i].email);
        }
        else{
            printf("Usuário ou senha inválidos.\n");
            if(t <= 5){
                printf("Tentativas restantes: %d. \n\n", 5 - t);
            }
            else{
                printf("Acesso negado.");
            }
        }
        t = t++;
    }while(t <= 5);
}
*/

void buscarLogin(char arquivo[]){
    char userInput[90] = "", senha[90] = "";
    char *nicknameDecrypted, *emailDecrypted, *senhaDecrypted, *nameDecrypted;
    int authentication = 0, tentativas = 1;
    long int chave1, chave2;
    struct User usuario;
    FILE *ponteiroArquivo = fopen(arquivo, "r");
    if(ponteiroArquivo == NULL){
        printf("Erro na leitura do arquivo!");
        return;
    }

    do{
        printf("Digite seu nickname ou seu e-mail: ");
        fgets(userInput, 89, stdin);
        userInput[strcspn(userInput, "\n")] = '\0';
        
        printf("Agora digite sua senha: ");
        fgets(senha, 89, stdin);
        senha[strcspn(senha, "\n")] = '\0';

        rewind(ponteiroArquivo);

        while(fgets(usuario.nome, sizeof(usuario.nome), ponteiroArquivo) != NULL){
            usuario.nome[strcspn(usuario.nome, "\n")] = '\0';

            fgets(usuario.nickname, sizeof(usuario.nickname), ponteiroArquivo);
            usuario.nickname[strcspn(usuario.nickname, "\n")] = '\0';

            fgets(usuario.idade, sizeof(usuario.idade), ponteiroArquivo);
            usuario.idade[strcspn(usuario.idade, "\n")] = '\0';

            fgets(usuario.email, sizeof(usuario.email), ponteiroArquivo);
            usuario.email[strcspn(usuario.email, "\n")] = '\0';

            fgets(usuario.senha, sizeof(usuario.senha), ponteiroArquivo);
            usuario.senha[strcspn(usuario.senha, "\n")] = '\0';

            fgets(usuario.key, sizeof(usuario.key), ponteiroArquivo);
            usuario.key[strcspn(usuario.key, "\n")] = '\0';

            if((strlen(userInput) == strlen(usuario.nickname) || strlen(userInput) == strlen(usuario.email)) && strlen(senha) == strlen(usuario.senha)){

                chave1 = decryptionFunction1(usuario.idade, usuario.nome);
                chave2 = decryptionFunction1(usuario.key, usuario.nome);

                nicknameDecrypted = decryptionFunction2(usuario.nickname, chave1, chave2);
                emailDecrypted = decryptionFunction2(usuario.email, chave1, chave2);

                if(strcmp(userInput, nicknameDecrypted) == 0 || strcmp(userInput, emailDecrypted) == 0){

                    senhaDecrypted = decryptionFunction2(usuario.senha, chave1, chave2);

                    if(strcmp(senha, senhaDecrypted) == 0){
                        nameDecrypted = decryptionFunction2(usuario.nome, chave1, chave2);
                        authentication = 1;
                        free(senhaDecrypted);
                        break;
                    }
                    free(senhaDecrypted);
                }
                free(nicknameDecrypted);
                free(emailDecrypted);
            }
        }

        if(authentication == 1){
            printf("\n\nBem vindo %s!\n\n", nameDecrypted);
            printf("Informações:\n");
            printf("Nickname: %s\n", nicknameDecrypted);
            printf("Idade: %ld\n", chave1);
            printf("E-mail: %s\n", emailDecrypted);
            free(nameDecrypted);
            free(nicknameDecrypted);
            free(emailDecrypted);
            break;
        }
        else{
            printf("Usuário ou senha inválidos.\n");
            if(tentativas <= 5){
                printf("Tentativas restantes: %d. \n\n", 5 - tentativas);
            }
            else{
                printf("Acesso negado.");
            }
        }
        tentativas++;
    }while(tentativas <= 5);
    printf("Pressione Enter para continuar...\n");
    getchar();

}

// ###################################################################
// Funcao de cadastro
void cadastrarUsuarios(struct User usuarios[], int *totalUsuarios, char arquivo[20]){
   
    char input[10];
    char *fimConversao;
    long int temp;
    int loop = 0;
    struct User newUser;
    FILE *ponteiroArquivo = fopen(arquivo, "a");

    if(ponteiroArquivo == NULL){
        printf("Erro na abertura do arquivo!");
        return;
    }
   
    // if(*totalUsuarios < maxUsers){
       
    printf("Digite seu nome: ");
    fgets(newUser.nome, sizeof(newUser.nome), stdin);
    fflush(stdin);
    newUser.nome[strcspn(newUser.nome, "\n")] = '\0';

    printf("Digite seu nick: ");
    fgets(newUser.nickname, sizeof(newUser.nickname), stdin);
    fflush(stdin);
    newUser.nickname[strcspn(newUser.nickname, "\n")] = '\0';

    printf("Digite sua idade: ");
    do{
        fgets(input, sizeof(input), stdin);
        fflush(stdin);
        temp = strtol(input, &fimConversao, 10);
        if(fimConversao == input || *fimConversao != '\n' || temp < 1 || temp > 100){
            printf("\nO valor digitado é inválido. Digite novamente: \n");
        }
        else{
            sprintf(newUser.idade, "%ld", temp);;
            loop = 1;
        }
    }
    while(fimConversao == input || *fimConversao != '\n' || temp < 1 || temp > 100);

    printf("Digite seu email: ");
    fgets(newUser.email, sizeof(newUser.email), stdin);
    fflush(stdin);
    newUser.email[strcspn(newUser.email, "\n")] = '\0';

    printf("Defina uma senha: ");
    fgets(newUser.senha, sizeof(newUser.senha), stdin);
    fflush(stdin);
    newUser.senha[strcspn(newUser.senha, "\n")] = '\0';
    
    encryption(&newUser);
    // usuarios[*totalUsuarios] = newUser;
    fprintf(ponteiroArquivo, "%s\n", newUser.nome);
    fprintf(ponteiroArquivo, "%s\n", newUser.nickname);
    fprintf(ponteiroArquivo, "%s\n", newUser.idade);
    fprintf(ponteiroArquivo, "%s\n", newUser.email);
    fprintf(ponteiroArquivo, "%s\n", newUser.senha);
    fprintf(ponteiroArquivo, "%s\n", newUser.key);
    (*totalUsuarios)++;

    fclose(ponteiroArquivo);
    printf("Pressione Enter para continuar...\n");
    getchar();
    return;
    // }
    // else{
    //     printf("O número máximo de usuários foi atingido.");
    //     return;
    // }
}

// ###################################################################
// Funcao Principal
int main(int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    // Definições:
    int totalUsuarios = 0;
    int opcao;
    struct User usuarios[maxUsers];
    char arquivo[] = "banco_de_usuarios.txt";

    do{
        system("clear");
        printf("----------APP----------\n\n");
        printf("Opções:\n");
        printf("1. Cadastrar\n");
        printf("2. Login\n");
        printf("3. Sair\n");
        // printf("3. Listar usuários\n\n");
        scanf("%d", &opcao);
        getchar();
        switch(opcao){
            case 1:
                printf("Cadastrar usuário\n\n");
                cadastrarUsuarios(usuarios, &totalUsuarios, arquivo);
                break;
             
            case 2:
                buscarLogin(arquivo);
                break;
               
            // case 3:
            //     //system("clear");
            //     // listarUsuarios(usuarios, totalUsuarios);
            //     listarUsuarios(arquivo);
            //     break;
               
            case 3:
                printf("Saindo...");
            
            case 10:
                testeCriptografia();
                break;
           
            default:
                printf("Opção inválida.\n\n");
        }
    }
    while(opcao != 3);
    return 0;
}
