#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale.h>
#define MAX 30
#define limpar_tela "cls"
 
typedef struct carro
{
	int id;
	int cliente_id;
	char marca[MAX];
	float preco;
} carro_car;
 
typedef struct cliente
{
	int id;
	char nome[MAX];
} cliente_car;

char menu();
void cadastrar_cliente();
void cadastrar_carro();
void listar_carros();
void listar_clientes();
carro_car *obter_carro(FILE *arq_carros, int id_carro);
cliente_car *obter_cliente(FILE *arq_clientes, int cliente_id);
void pesquisar_carro();
void pesquisar_cliente();
int existe_carro(FILE *arq_carros, int id_carro);
int existe_cliente(FILE *arq_clientes, int cliente_id);
void atualizar_carros(FILE *arq_carros, carro_car *carro_alugado);
void alugar_carro();
void entregar_carro();
void excluir_carro();
int str_somente_numeros(char str[]);
//int str_somente_letras(char str[]);

int main(int argc, char *argv[])
{
	char resposta;
	while(1)
	{
		resposta = menu();
 
		if(resposta == '1')
			cadastrar_cliente();
		else if(resposta == '2')
			cadastrar_carro();
		else if(resposta == '3')
			listar_clientes();
		else if(resposta == '4')
			listar_carros();
		else if(resposta == '5')
			pesquisar_cliente();
		else if(resposta == '6')
			pesquisar_carro();
		else if(resposta == '7')
			alugar_carro();
		else if(resposta == '8')
			entregar_carro();
		else if(resposta == '9')
			excluir_carro();
		else if(resposta == '0')
			break;
		else
		{
			printf("\nOpçãoo inválida!");
			//limpar o buffer de entrada
			fseek(stdin, 0, SEEK_END);
		}
		system(limpar_tela);
	}
	return 0;
}
 
char menu()
{
	setlocale(LC_ALL, "");
	char resposta[2];
 
	printf("########################## Locadora De Automóveis ##########################");
	printf("\n\n1 - Cadastrar um cliente\n");
	printf("2 - Cadastrar um carro\n");
	printf("3 - Listar todos os clientes\n");
	printf("4 - Listar todos os carros\n");
	printf("5 - Pesquisar por cliente\n");
	printf("6 - Pesquisar por carro\n");
	printf("7 - Alugar um carro\n");
	printf("8 - Entregar um carro\n");
	printf("9 - Excluir um carro\n");
	printf("0 - Sair\n");
	printf("Informe o número da opção: ");
	scanf("%1s%*c", resposta);
 
	fseek(stdin, 0, SEEK_END);
	return resposta[0];
}

int str_somente_numeros(char str[])
{
	int i = 0;
	int len_str = strlen(str);
 
	for(i = 0; i < len_str; i++)
	{
		if(str[i] < '0' || str[i] > '9')
			return 0;
	}
	return 1;
}
 
void cadastrar_cliente()
{
	// abre o arquivo para escrita
	FILE *arq_clientes = fopen("clientes.dat", "a+b");

	if(arq_clientes == NULL)
	{
		printf("\nNão foi possível abrir arquivo!\n");
		exit(1);
	}
	//pega o último ID para criar o próximo
	int cont_bytes = 0;
	fseek(arq_clientes, 0, SEEK_END);
	// pegamos a quantidade de bytes com a função ftell
	cont_bytes = ftell(arq_clientes);
	cliente_car cliente;
	if(cont_bytes == 0)
	{
		cliente.id = 1;
	}
	else
	{
		cliente_car ultimo_cliente;
 
		// utilizo a função fseek para posicionar o arquivo
		// cont_bytes - sizeof(cliente_car) serve para posicionar para pegar o último cliente cadastrado
		fseek(arq_clientes, cont_bytes - sizeof(cliente_car), SEEK_SET);
		fread(&ultimo_cliente, sizeof(cliente_car), 1, arq_clientes);
		cliente.id = ultimo_cliente.id + 1;
	}

	printf("\nInforme o nome do cliente: ");
	scanf("%99[^\n]%*c", cliente.nome);
	fseek(stdin, 0, SEEK_END);
	fseek(arq_clientes, 0, SEEK_END);
	fwrite(&cliente, sizeof(cliente_car), 1, arq_clientes);
	fclose(arq_clientes);
 
	printf("\nCadastrado com sucesso!\n");
	system("pause");
}

void cadastrar_carro()
{
	// abre o arquivo para escrita
	FILE *arq_carros = fopen("carros.dat", "ab+"); //ab+ anexa dados em um arquivo já existente, permitindo também a
                                                  //leitura (cria arquivo se ele não existir)
	if(arq_carros == NULL)
	{
		printf("\nNão foi possível abrir arquivo!\n");
		exit(1); 
	}
	carro_car carro;
	int cont_bytes = 0;
	fseek(arq_carros, 0, SEEK_END);
	cont_bytes = ftell(arq_carros);
 
	if(cont_bytes == 0)
	{
		carro.id = 1;
	}
	else
	{
		carro_car ultimo_carro;
		fseek(arq_carros, cont_bytes - sizeof(carro_car), SEEK_SET);
		fread(&ultimo_carro, sizeof(carro_car), 1, arq_carros);
		carro.id = ultimo_carro.id + 1;
	}

	printf("\nDigite a marca do carro: ");
	scanf("%99[^\n]%*c", carro.marca);

	fseek(stdin, 0, SEEK_END);
 
	do
	{
		char str_preco[5];
		float float_preco;
		int somente_numeros = 1;

		printf("Informe o valor do aluguel do carro: ");
		scanf("%4s%*c", str_preco);
 
		fseek(stdin, 0, SEEK_END);
		somente_numeros = str_somente_numeros(str_preco);
		if(somente_numeros == 1)
		{
			int int_preco;
			sscanf(str_preco, "%d", &int_preco);
			float_preco = int_preco;
			carro.preco = float_preco;
			break;
		}
	}
	while(1);
	carro.cliente_id = -1;
	fseek(arq_carros, 0, SEEK_END);
	fwrite(&carro, sizeof(carro_car), 1, arq_carros);
	fclose(arq_carros);
 
	printf("\nCarro cadastrado com sucesso!\n");
	system("pause");
}

void listar_clientes()
{
	// rb => abre arquivo para leitura apenas
	FILE *arq_clientes = fopen("clientes.dat", "rb");
	if(arq_clientes == NULL)
	{
		printf("\Não foi possível abrir arquivo!ou ");
		printf("Nenhum cliente cadastrado.\n");
		system("pause");
		return;
	}
 
	// variável que indica se encontrou pelo menos 1 cliente
	int encontrou_clientes = 0;
	cliente_car cliente;
 
	printf("\nLista de todos os clientes...\n");
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&cliente, sizeof(cliente_car), 1, arq_clientes);
		if(result == 0)
			break;
		encontrou_clientes = 1;
 
		printf("\nID do cliente: %d\n", cliente.id);
		printf("Nome do cliente: %s\n", cliente.nome);
	}
 
	if(encontrou_clientes == 0)
		printf("\nNenhum cliente cadastrado.\n");
 
	fclose(arq_clientes);
	system("pause");

}

cliente_car *obter_cliente(FILE *arq_clientes, int cliente_id)
{
	rewind(arq_clientes);
	cliente_car *cliente;
	cliente = (cliente_car *)malloc(sizeof(cliente_car));
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(cliente, sizeof(cliente_car), 1, arq_clientes);
		if(result == 0)
		{
			free(cliente); 
			return NULL;
		}
		if(cliente->id == cliente_id)
			break;
	}
	return cliente;
}

void listar_carros()
{
 	// rb => abre para leitura somente
	FILE *arq_carros = fopen("carros.dat", "rb");
	FILE *arq_clientes = fopen("clientes.dat", "rb");
 
	if(arq_carros == NULL)
	{
		printf("\Não foi possível abrir arquivo! ou ");
		printf("Nenhum carro cadastrado.\n");
		system("pause");
		fseek(stdin, 0, SEEK_END); 
		return;
	}
	int encontrou_carros = 0;
	printf("\nLista de todos os carros...\n");
	carro_car carro;
	while(1)
	{
		size_t result = fread(&carro, sizeof(carro_car), 1, arq_carros);
		if(result == 0)
			break;
 
		encontrou_carros = 1;
 
		printf("\nID do carro: %d\n", carro.id);
		printf("Marca do carro: %s\n", carro.marca);
		printf("Preço: %.2f\n", carro.preco);
 
		if(carro.cliente_id != -1)
		{
			if(arq_clientes == NULL)
			{
				printf("\Não foi possível abrir arquivo!!\n");
				fclose(arq_carros); 
				exit(1); 
			}
			cliente_car *cliente = obter_cliente(arq_clientes, carro.cliente_id);
			printf("Foi alugado? Sim. Pelo cliente: %s\n", cliente->nome);
			free(cliente);
		}
		else
			printf("Foi alugado? Não\n");
	}
	if(encontrou_carros == 0)
		printf("\nNenhum carro cadastrado.\n");
	if(arq_clientes != NULL)
		fclose(arq_clientes); 
	fclose(arq_carros);
	system("pause");
}
 
 void pesquisar_carro()
{
	char marca[MAX];
	int encontrou_carro = 0;
 
	// rb => abre para leitura somente
	FILE *arq_carros = fopen("carros.dat", "rb");
	FILE *arq_clientes = fopen("clientes.dat", "rb");
 
	if(arq_carros == NULL)
	{
		printf("\Não foi possível abrir arquivo!!\n");
		system("pause");
	}
 
	printf("\nInforme a marca do carro: ");
	scanf("%99[^\n]%*c", marca);
 
	printf("\nCarros com a marca \"%s\":\n\n", marca);
	carro_car carro;
	while(1)
	{
		size_t result = fread(&carro, sizeof(carro_car), 1, arq_carros);
		if(result == 0)
			break;
 
		char marca_aux[MAX];
		strcpy(marca_aux, carro.marca);

		if(strcmp(strupr(marca_aux), strupr(marca)) == 0)
		{
			printf("ID do carro: %d\n", carro.id);
			printf("Preço: %.2lf\n", carro.preco);
			if(carro.cliente_id != -1)
			{
				if(arq_clientes == NULL)
				{
					printf("\Não foi possível abrir arquivo!!\n");
					fclose(arq_carros);
					system("pause");
				}
				cliente_car *cliente = obter_cliente(arq_clientes, carro.cliente_id);
				printf("Foi alugado? Sim. Pelo cliente: %s\n", cliente->nome);
				free(cliente);
			}
			else
				printf("Foilugado? Nao\n");
			encontrou_carro = 1;
			printf("\n");
		}
	}
 
	if(encontrou_carro == 0)
		printf("Nenhum carro foi encontrado.\n\n");
 
	fclose(arq_carros);
	system("pause");
}

void pesquisar_cliente()
{
	char nome[MAX];
	int encontrou_cliente = 0;
 
	FILE *arq_clientes = fopen("clientes.dat", "rb");
	if(arq_clientes == NULL)
	{
		printf("\Não foi possível abrir arquivo!)!\n");
		exit(1); 
	}
 
	printf("\nInforme o nome do cliente: ");
	scanf("%99[^\n]%*c", nome);
 
	printf("\nClientes com o nome \"%s\":\n\n", nome);
	cliente_car cliente;
	while(1)
	{
		size_t result = fread(&cliente, sizeof(cliente_car), 1, arq_clientes);
		if(result == 0)
			break;
 
		char nome_aux[MAX];
		strcpy(nome_aux, cliente.nome);
		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			printf("ID do cliente: %d\n\n", cliente.id);
			encontrou_cliente = 1;
		}
	}
 
	if(encontrou_cliente == 0)
		printf("Nenhum cliente encontrado.\n\n");
	fclose(arq_clientes);
	system("pause");
	fseek(stdin, 0, SEEK_END);
}
 
int existe_carro(FILE *arq_carros, int id_carro)
{
	rewind(arq_carros);
	carro_car carro;
	while(1)
	{
		size_t result = fread(&carro, sizeof(carro_car), 1, arq_carros);
		if(result == 0)
			break;
		if(carro.id == id_carro)
			return 1;
	}
	return 0;
}
 
int existe_cliente(FILE *arq_clientes, int cliente_id)
{
	rewind(arq_clientes);
	cliente_car cliente;
	while(1)
	{
		size_t result = fread(&cliente, sizeof(cliente_car), 1, arq_clientes);
		if(result == 0)
			break;
		if(cliente.id == cliente_id)
			return 1;
	}
	return 0;
}
 
carro_car *obter_carro(FILE *arq_carros, int id_carro)
{
	rewind(arq_carros);
	carro_car *carro;
	carro = (carro_car *)malloc(sizeof(carro_car));
	while(1)
	{
		size_t result = fread(carro, sizeof(carro_car), 1, arq_carros);
		if(result == 0)
			break;
		if(carro->id == id_carro)
			return carro;
	}
	free(carro); 
	return NULL;
}

void atualizar_carros(FILE *arq_carros, carro_car *carro_alugado)
{
	rewind(arq_carros);
	carro_car carro;
	while(1)
	{
		size_t result = fread(&carro, sizeof(carro_car), 1, arq_carros);
		if(result == 0)
			break;
		if(carro.id == carro_alugado->id)
		{
			fseek(arq_carros, - sizeof(carro_car), SEEK_CUR);
			fwrite(carro_alugado, sizeof(carro_car), 1, arq_carros);
			break; 
		}
	}
}
 
void alugar_carro()
{
	char str_cliente_id[10];
	int cliente_id;
 
	// rb+ abre para leitura/atualização
	FILE *arq_carros = fopen("carros.dat", "rb+");
	FILE *arq_clientes = fopen("clientes.dat", "rb+");

	if(arq_carros == NULL)
	{
		arq_carros = fopen("carros.dat", "wb+");
		if(arq_carros == NULL)
		{
			printf("\nNão foi possível criar o arquivo!\n");
			exit(1);
		}
	}
 
	if(arq_clientes == NULL)
	{
		arq_clientes = fopen("clientes.dat", "wb+");
		if(arq_clientes == NULL)
		{
			printf("\nNão foi possível criar o arquivo!\n");
			exit(1); 
		}
	}
	printf("\nInforme o ID do cliente: ");
	scanf("%10s%*c", str_cliente_id);
 
	fseek(stdin, 0, SEEK_END);
 
	if(str_somente_numeros(str_cliente_id) == 1)
	{
		sscanf(str_cliente_id, "%d", &cliente_id);
		if(existe_cliente(arq_clientes, cliente_id))
		{
			char str_id_carro[10];
			int id_carro;
			printf("\nInforme o ID do carro: ");
			scanf("%10s%*c", str_id_carro);
			fseek(stdin, 0, SEEK_END); 
 
			if(str_somente_numeros(str_id_carro) == 1)
			{
				sscanf(str_id_carro, "%d", &id_carro);
				carro_car *carro = obter_carro(arq_carros, id_carro);
 
				if(carro != NULL)
				{
					if(carro->cliente_id != -1)
						printf("\nO carro \"%s\" já foi alugado!\n", carro->marca);
					else
					{
						carro->cliente_id = cliente_id;
						atualizar_carros(arq_carros, carro);
						printf("\nO carro \"%s\" foi alugado com sucesso!\n", carro->marca);
					}
					free(carro);
				}
				else
					printf("\nNão existe carro com o ID \"%d\".\n", id_carro);
			}
			else
				printf("\nO ID so pode conter números!\n");
		}
		else
			printf("\nNão existe cliente com o ID \"%d\".\n", cliente_id);
	}
	else
		printf("\nO ID só pode conter números!\n");
	fclose(arq_clientes);
	fclose(arq_carros);
	system("pause");
}

void entregar_carro()
{
	char str_id_carro[10];
	int id_carro;
 
	FILE *arq_carros = fopen("carros.dat", "rb+");
 
	if(arq_carros == NULL)
	{
		arq_carros = fopen("carros.dat", "wb+");
		if(arq_carros == NULL)
		{
			printf("\nFNão foi possível criar o arquivo!\n");
			exit(1);
		}
	}
 
	printf("\nInforme o ID do carro: ");
	scanf("%10s%*c", str_id_carro);
 
	fseek(stdin, 0, SEEK_END);
	if(str_somente_numeros(str_id_carro) == 1)
	{
		sscanf(str_id_carro, "%d", &id_carro);
		carro_car *carro = obter_carro(arq_carros, id_carro);
 
		if(carro != NULL)
		{
			if(carro->cliente_id == -1)
				printf("\nO carro \"%s\" já está disponível!\n", carro->marca);
			else
			{
				carro->cliente_id = -1;
				atualizar_carros(arq_carros, carro); 
				printf("\nO carro \"%s\" foi entregue com sucesso!\n", carro->marca);
			}
			free(carro);
		}
		else
			printf("\nNão existe carro com o ID \"%d\".\n", id_carro);
	}
	else
		printf("\nO ID só pode conter números!\n");

	fclose(arq_carros);
	system("pause");
}

void excluir_carro()
{
	char str_id_carro[10];
	int id_carro;
 
	printf("\nInforme o ID do carro: ");
	scanf("%10s%*c", str_id_carro);
 
	fseek(stdin, 0, SEEK_END);

	if(str_somente_numeros(str_id_carro) == 1)
	{
		sscanf(str_id_carro, "%d", &id_carro);
 
		// rb abre para leitura (o arquivo deve existir)
		FILE *arq_carros = fopen("carros.dat", "rb");
 
		if(arq_carros == NULL)
		{
			printf("\nNão foi possível abrir o arquivo!\n");
			exit(1);
		}
		if(existe_carro(arq_carros, id_carro) == 1)
		{
			char marca_carro[MAX];
			FILE *arq_temp = fopen("temp_carros.dat", "ab+");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_carros);
				exit(1);
			}
			rewind(arq_carros);
 
			carro_car carro;
			while(1)
			{
				size_t result = fread(&carro, sizeof(carro_car), 1, arq_carros);
				if(result == 0)
					break;
				if(carro.id != id_carro)
				{
					fwrite(&carro, sizeof(carro_car), 1, arq_temp);
				}
				else
					strcpy(marca_carro, carro.marca);
			}
			fclose(arq_carros);
			fclose(arq_temp);

			if(remove("carros.dat") != 0)
				printf("\nErro ao deletar o arquivo \n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_carros.dat", "carros.dat");
				if(r != 0)
				{
					printf("\nPermissão negada para renomear o arquivo!\n");
				}
				else
					printf("\nCarro removido com sucesso!\n");
			}
		}
		else
		{
			fclose(arq_carros);
			printf("\nNão existe carro com o ID \"%d\".\n", id_carro);
		}
	}
	else
		printf("\nO ID só pode conter números!\n");
	system("pause");
	fseek(stdin, 0, SEEK_END);
}
 
