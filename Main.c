#include <stdio.h>
#include <string.h>

//PREFIXOS DE VARIÁVEIS:
//am_ - "amount", quantidades
//yn_ - "yes/no", sim ou não

//PREFIXOS DE FUNÇÕES:
//menu_ - Funções relacionadas à navegação dentro do programa.

//VARIÁVEL GLOBAL
//Não consigo pensar num jeito melhor de detectar se o usuário inseriu um
//TCC durante o preenchimento das matérias que não seja uma variável de
//escopo global.
int tcc_info = 0;

//MENSAGENS
//Para ter uma mensagem consistente através do programa, quando o mesmo
//erro ou situação ocorrem.
const char* ERR_INVALID_CHARACTER = "\nVoce digitou um numero ou caracter invalido. Por favor, tente novamente.\n";
const char* ERR_INVALID_PRINT_TYPE = "\nSe voce esta vendo essa mensagem, houve um erro por parte do programador. Por favor, comunique isso a ele.\n";
const char* ERR_NO_RESULT = "\nEssa consulta nao retornou nenhum resultado. Certifique-se de que digitou corretamente.\n";

const char* MSG_EXACT_SEARCH = "\nAtencao: esse mecanismo de pesquisa e rudimentar e apenas encontra resultados se a consulta for exata.\nSe voce nao encontrar o que procura e tiver certeza de que existe, procure na listagem geral desse tipo.\n";
const char* MSG_SAME_UPDATE = "\nCaso nao queira atualizar algum dos dados, apenas insira o mesmo.\n";

//FUNÇÕES AUXILIARES
//Funções que nasceram de código reaproveitado.
int count_line_breaks(FILE* file) {
	fseek(file, 0, SEEK_SET);
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
	return count;
}

/*
 * Cada struct é seguido de seus formatadores e suas funções (CRUD, outras).
 * Apenas as funções do primeiro struct serão comentadas, porque o
 * resto é igual.
 */
typedef struct _atividade_complementar {
	int id;
	char nome[100];
	int horas;
} AtividadeComplementar;

const char* ATV_COM_FORMAT_IN = "(%d;%[^;];%d)";
const char* ATV_COM_FORMAT_OUT = "(%d;%s;%d)\n";

/*
 * Traz o cursor para o final do arquivo, depois escreve nele de acordo
 * com o formato especificado acima.
 */
void write_atividade_complementar(FILE* file, int id, char* nome, int horas) {
	fseek(file, 0, SEEK_END);
	fprintf(file, ATV_COM_FORMAT_OUT, id, nome, horas);
}

/*
 * Traz o cursor para o início do arquivo, conta a quantidade de quebras
 * de linha (ou seja, a quantidade de registros) e depois passa pelos
 * registros, até encontrar o registro com o ID especificado, retornando,
 * então, esse registro no formato de um AtividadeComplementar. Se não
 * encontrar o ID especificado, então imprime uma mensagem de erro e
 * retorna um AtividadeComplementar vazio.
 */
AtividadeComplementar read_atividade_complementar(FILE* file, int id_seek) {
	AtividadeComplementar at;
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, ATV_COM_FORMAT_IN, &id, nome, &horas);
		at.id = id;
		strcpy(at.nome, nome);
		at.horas = horas;

		if (at.id == id_seek) {
			return at;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}

	printf("ERRO: ID de atividade complementar inválido.\n");
	AtividadeComplementar erro = { 0, "\0", 0 };
	return erro;
}

/*
 * Abre um novo arquivo, depois conta a quantidade de registros e então
 * passa todos os registros, menos o que tem o ID especificado, para o
 * novo arquivo, deletando então o antigo e renomeando o novo para o
 * substituir. Então, reabre o arquivo para poder continuar sendo usado
 * na main().
 */
void delete_atividade_complementar(FILE* file, int id_seek) {
	FILE* new_file = fopen("atividade_complementar_new.dat", "w+");

	int count = count_line_breaks(file);

	fseek(file, 0, SEEK_SET);
	int id = 0, horas = 0;
	char nome[100] = { '\0' };
	for (int i = 0; i < count; i++) {
		fscanf(file, ATV_COM_FORMAT_IN, &id, nome, &horas);
		if (id != id_seek) {
			write_atividade_complementar(new_file, id, nome, horas);
		}
		fseek(file, 2, SEEK_CUR);
	}

	fclose(file);
	fclose(new_file);
	if (remove("atividade_complementar.dat") == 0 && rename("atividade_complementar_new.dat", "atividade_complementar.dat") == 0) {
		printf("Atividade complementar deletada com sucesso!\n");
	}
	else {
		printf("Erro ao deletar atividade complementar\n");
	}
	fopen("atividade_complementar.dat", "a+");
}

/*
 * Deleta o registro com o ID especificado e então o readiciona com os dados
 * atualizados no final do arquivo.
 */
void update_atividade_complementar(FILE* file, int id_seek, char* nome, int horas) {
	delete_atividade_complementar(file, id_seek);
	write_atividade_complementar(file, id_seek, nome, horas);
	printf("Atividade complementar atualizada com sucesso!\n");
}

/*
 * Ô saudade de um toString(). Essa função imprime o struct com uma formatação
 * mais amigável.
 */
void print_atividade_complementar(AtividadeComplementar at) {
	printf("ID: %d, Nome: %s, %d horas\n", at.id, at.nome, at.horas);
}

/*
 * Essa função imprime todos desse tipo guardados no arquivo.
 */
void print_all_atv_com(FILE* file) {
	int id = 0, horas = 0;
	char nome[100] = { "\0" };
	AtividadeComplementar at;

	int count = count_line_breaks(file);

	printf("\n");

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, ATV_COM_FORMAT_IN, &id, nome, &horas);
		fseek(file, 2, SEEK_CUR);

		at.id = id;
		strcpy(at.nome, nome);
		at.horas = horas;

		print_atividade_complementar(at);
	}
}

/*
 * Essa função retorna o ID da última atividade complementar
 * armazenada no arquivo, para que a função fill_atividade_complementar
 * não repita IDs por utilizar variáveis alocadas dinâmicamente.
 */
int atv_com_get_last_id(FILE* file) {
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	int atv_com_highest_id = 0;

	for (int i = 0; i < count; i++) {
		fscanf(file, ATV_COM_FORMAT_IN, &id, nome, &horas);

		if (id > atv_com_highest_id) {
			atv_com_highest_id = id;
		}

		fseek(file, 2, SEEK_CUR);
	}

	return atv_com_highest_id;
}

/*
 * Essa função pode ser chamada a qualquer momento para que o
 * usuário possa preencher uma ou mais atividades complementares.
 */
void fill_atividade_complementar(FILE* file) {
	printf("Quantas atividades complementares deseja preencher? ");
	int am_fill_atv_com = 0;
	scanf("%d", &am_fill_atv_com);

	for (int i = 0; i < am_fill_atv_com; i++) {
		printf("Insira o nome da %da atividade complementar: ", i + 1);
		char nome[100] = {'\0'};
		scanf("\n%[^\n]", &nome);

		printf("Insira a quantidade de horas da %da atividade complementar: ", i + 1);
		int horas = 0;
		scanf("%d", &horas);

		int last_id = atv_com_get_last_id(file);
		int id = last_id + 1;
		write_atividade_complementar(file, id, nome, horas);

		printf("\n");
	}
}

//END OF ATIVIDADE COMPLEMENTAR

typedef struct _estagio {
	int id;
	char nome[100];
	int horas;
} Estagio;

const char* ESTAGIO_FORMAT_IN = "(%d;%[^;];%d)";
const char* ESTAGIO_FORMAT_OUT = "(%d;%s;%d)\n";

void write_estagio(FILE* file, int id, char* nome, int horas) {
	fseek(file, 0, SEEK_END);
	fprintf(file, ESTAGIO_FORMAT_OUT, id, nome, horas);
}

Estagio read_estagio(FILE* file, int id_seek) {
	Estagio es;
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, ESTAGIO_FORMAT_IN, &id, nome, &horas);
		es.id = id;
		strcpy(es.nome, nome);
		es.horas = horas;

		if (es.id == id_seek) {
			return es;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}

	printf("ERRO: ID de estagio invalido.\n");
	Estagio erro = { 0, "\0", 0 };
	return erro;
}

void delete_estagio(FILE* file, int id_seek) {
	FILE* new_file = fopen("estagio_new.dat", "w+");
	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	int id = 0, horas = 0;
	char nome[100] = { '\0' };
	for (int i = 0; i < count; i++) {
		fscanf(file, ESTAGIO_FORMAT_IN, &id, nome, &horas);
		if (id != id_seek) {
			write_estagio(new_file, id, nome, horas);
		}
		fseek(file, 2, SEEK_CUR);
	}

	fclose(file);
	fclose(new_file);
	if (remove("estagio.dat") == 0 && rename("estagio_new.dat", "estagio.dat") == 0) {
		printf("Estagio deletado com sucesso!\n");
	}
	else {
		printf("Erro ao deletar estagio\n");
	}
	fopen("estagio.dat", "a+");
}

void update_estagio(FILE* file, int id_seek, char* nome, int horas) {
	delete_estagio(file, id_seek);
	write_estagio(file, id_seek, nome, horas);
	printf("Estagio atualizado com sucesso!\n");
}

void print_estagio(Estagio es) {
	printf("ID: %d, Nome: %s, %d horas\n", es.id, es.nome, es.horas);
}

void print_all_estagio(FILE* file) {
	int id = 0, horas = 0;
	char nome[100] = { "\0" };
	Estagio es;

	int count = count_line_breaks(file);

	printf("\n");

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, ATV_COM_FORMAT_IN, &id, nome, &horas);
		fseek(file, 2, SEEK_CUR);

		es.id = id;
		strcpy(es.nome, nome);
		es.horas = horas;

		print_estagio(es);
	}
}

int est_get_last_id(FILE* file) {
	fseek(file, 0, SEEK_SET);
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	int est_highest_id = 0;

	for (int i = 0; i < count; i++) {
		fscanf(file, ESTAGIO_FORMAT_IN, &id, nome, &horas);
		
		if (id > est_highest_id) {
			est_highest_id = id;
		}
		fseek(file, 2, SEEK_CUR);
	}

	return est_highest_id;
}

void fill_estagio(FILE* file) {
	printf("Quantos estagios voce quer preencher? ");
	int am_estagio = 0;
	scanf("%d", &am_estagio);

	for (int i = 0; i < am_estagio; i++) {
		printf("Insira o nome do %do estagio: ", i + 1);
		char nome[100] = { '\0' };
		scanf("\n%[^\n]", &nome);
		printf("Insira a quantidade de horas do %do estagio: ", i + 1);
		int horas = 0;
		scanf("%d", &horas);

		int id = est_get_last_id(file) + 1;

		write_estagio(file, id, nome, horas);

		printf("\n");
	}
}

//END OF ESTAGIO

typedef struct _tcc {
	int id;
	char nome[100];
	char orientador[100];
} TCC;

const char* TCC_FORMAT_IN = "(%d;%[^;];%[^)])";
const char* TCC_FORMAT_OUT = "(%d;%s;%s)\n";

void write_tcc(FILE* file, int id, char* nome, char* orientador) {
	fseek(file, 0, SEEK_END);
	fprintf(file, TCC_FORMAT_OUT, id, nome, orientador);
}

TCC read_tcc(FILE* file, int id_seek) {
	TCC tc;
	int id = 0;
	char nome[100] = { "\0" }, orientador[100] = { '\0' };

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, TCC_FORMAT_IN, &id, nome, orientador);
		tc.id = id;
		strcpy(tc.nome, nome);
		strcpy(tc.orientador, orientador);

		if (tc.id == id_seek) {
			return tc;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}

	printf("ERRO: ID de TCC invalido.\n");
	TCC erro = { 0, "\0", "\0"};
	return erro;
}

void delete_tcc(FILE* file, int id_seek) {
	FILE* new_file = fopen("tcc_new.dat", "w+");
	int count = count_line_breaks(file);

	fseek(file, 0, SEEK_SET);
	int id = 0;
	char nome[100] = { '\0' }, orientador[100] = { '\0' };
	for (int i = 0; i < count; i++) {
		fscanf(file, TCC_FORMAT_IN, &id, nome, orientador);
		if (id != id_seek) {
			write_tcc(new_file, id, nome, orientador);
		}
		fseek(file, 2, SEEK_CUR);
	}

	fclose(file);
	fclose(new_file);
	if (remove("tcc.dat") == 0 && rename("tcc_new.dat", "tcc.dat") == 0) {
		printf("TCC deletado com sucesso!\n");
	}
	else {
		printf("Erro ao deletar TCC\n");
	}
	fopen("tcc.dat", "a+");
}

void update_tcc(FILE* file, int id_seek, char* nome, char* orientador) {
	delete_tcc(file, id_seek);
	write_tcc(file, id_seek, nome, orientador);
	printf("TCC atualizado com sucesso!\n");
}

void print_tcc(TCC tcc) {
	printf("ID: %d, Nome: %s, Orientador: %s\n", tcc.id, tcc.nome, tcc.orientador);
}

void print_all_tcc(FILE* file) {
	int id = 0;
	char nome[100] = { "\0" }, orientador[100] = { '\0' };
	TCC tc;

	int count = count_line_breaks(file);

	printf("\n");

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, TCC_FORMAT_IN, &id, nome, orientador);
		fseek(file, 2, SEEK_CUR);

		tc.id = id;
		strcpy(tc.nome, nome);
		strcpy(tc.orientador, orientador);

		print_tcc(tc);
	}
}

int tcc_get_last_id(FILE* file) {
	int id = 0;
	char nome[100] = { "\0" }, orientador[100] = {'\0'};

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	int tcc_highest_id = 0;

	for (int i = 0; i < count; i++) {
		fscanf(file, TCC_FORMAT_IN, &id, nome, orientador);

		if (id > tcc_highest_id) {
			tcc_highest_id = id;
		}
		fseek(file, 2, SEEK_CUR);
	}

	return tcc_highest_id;
}

void fill_tcc(FILE* file) {
	for (int i = 0; i < tcc_info; i++) {
		printf("Insira o nome do seu TCC %d: ", i + 1);
		char nome[100] = { '\0' };
		scanf("\n%[^\n]", nome);
		
		printf("Insira o nome do seu orientador para o TCC %d: ", i + 1);
		char orientador[100] = { '\0' };
		scanf("\n%[^\n]", orientador);

		int id = tcc_get_last_id(file) + 1;
		
		write_tcc(file, id, nome, orientador);
	}
}

//END OF TCC

typedef struct _grade_curricular {
	int id;
	char nome_materia[100];
} GradeCurricular;

const char* GRD_CUR_FORMAT_IN = "(DPAAIN.%03d;%[^)])";
const char* GRD_CUR_FORMAT_OUT = "(DPAAIN.%03d;%s)\n";

void write_grade_curricular(FILE* file, int id, char* nome) {
	fseek(file, 0, SEEK_END);
	fprintf(file, GRD_CUR_FORMAT_OUT, id, nome);
}

GradeCurricular read_grade_curricular(FILE* file, int id_seek) {
	fseek(file, 0, SEEK_SET);
	GradeCurricular gc;
	int id = 0;
	char nome_materia[100] = { "\0" };

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, GRD_CUR_FORMAT_IN, &id, nome_materia);
		gc.id = id;
		strcpy(gc.nome_materia, nome_materia);

		if (gc.id == id_seek) {
			return gc;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}

	printf("ERRO: ID de materia invalido.\n");
	GradeCurricular erro = { 0, "\0" };
	return erro;
}

void delete_grade_curricular(FILE* file, int id_seek) {
	FILE* new_file = fopen("grade_curricular_new.dat", "w+");
	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	int id = 0;
	char nome_materia[100] = { "\0" };
	for (int i = 0; i < count; i++) {
		fscanf(file, GRD_CUR_FORMAT_IN, &id, nome_materia);
		if (id != id_seek) {
			write_grade_curricular(new_file, id, nome_materia);
		}
		fseek(file, 2, SEEK_CUR);
	}

	fclose(file);
	fclose(new_file);
	if (remove("grade_curricular.dat") == 0 && rename("grade_curricular_new.dat", "grade_curricular.dat") == 0) {
		printf("Materia deletada com sucesso!\n");
	}
	else {
		printf("Erro ao deletar materia\n");
	}
	fopen("grade_curricular.dat", "a+");
}

void update_grade_curricular(FILE* file, int id_seek, char* nome_materia) {
	delete_grade_curricular(file, id_seek);
	write_grade_curricular(file, id_seek, nome_materia);
	printf("Materia atualizada com sucesso!\n");
}

void print_grade_curricular(GradeCurricular gr) {
	printf("ID: DPAAIN.%03d, Nome: %s\n", gr.id, gr.nome_materia);
}

const int NUM_MATERIAS = 60;

void fill_grade_curricular(FILE* file) {
	write_grade_curricular(file, 389, "Introducao a Engenharia de Software");
	write_grade_curricular(file, 390, "Fundamentos de Algebra e Calculo");
	write_grade_curricular(file, 391, "Logica e Pensamento Computacional");
	write_grade_curricular(file, 392, "Escrita Cientifica para Computacao");
	write_grade_curricular(file, 393, "Ingles Instrumental");
	write_grade_curricular(file, 394, "Engenharia de Requisitos");
	write_grade_curricular(file, 395, "Algoritmos e Tecnicas de Programacao I");
	write_grade_curricular(file, 396, "Diversidade e Inclusao");
	write_grade_curricular(file, 397, "Analise e Projeto de Sistemas");
	write_grade_curricular(file, 398, "Etica, Tecnologia e Sociedade");
	write_grade_curricular(file, 399, "Calculo 1");
	write_grade_curricular(file, 400, "Extensao Curricular I");
	write_grade_curricular(file, 401, "Algoritmos e Tecnicas de Programacao II");
	write_grade_curricular(file, 402, "Sistemas Operacionais");
	write_grade_curricular(file, 403, "Algebra Linear");
	write_grade_curricular(file, 404, "Banco de Dados I");
	write_grade_curricular(file, 405, "Extensao Curricular II");
	write_grade_curricular(file, 406, "Estruturas de Dados");
	write_grade_curricular(file, 407, "Programacao Orientada a Objetos I");
	write_grade_curricular(file, 408, "Banco de Dados II");
	write_grade_curricular(file, 409, "Extensao Curricular III");
	write_grade_curricular(file, 410, "Administracao e Gestao Estrategica");
	write_grade_curricular(file, 411, "Inovacao e Empreendedorismo");
	write_grade_curricular(file, 412, "Interface Homem-Maquina");
	write_grade_curricular(file, 413, "Programacao Orientada a Objetos II");
	write_grade_curricular(file, 414, "Redes de Computadores");
	write_grade_curricular(file, 415, "Extensao Curricular IV");
	write_grade_curricular(file, 416, "Sistemas Distribuidos");
	write_grade_curricular(file, 417, "Projeto de Pesquisa e Inovacao I");
	write_grade_curricular(file, 418, "Relacoes Etnico-Raciais e Cultura Afrobrasileira e Indigena");
	write_grade_curricular(file, 419, "Governanca, Seguranca e Auditoria de Sistemas");
	write_grade_curricular(file, 420, "Engenharia da Experiencia do Usuario");
	write_grade_curricular(file, 421, "Pratica de Fabrica de Software I");
	write_grade_curricular(file, 422, "Projeto de Pesquisa e Inovacao II");
	write_grade_curricular(file, 423, "Testes de Software");
	write_grade_curricular(file, 424, "Introducao a Inteligencia Artificial");
	write_grade_curricular(file, 425, "Economia Criativa");
	write_grade_curricular(file, 426, "Qualidade de Software");
	write_grade_curricular(file, 427, "Pratica de Fabrica de Software II");
	write_grade_curricular(file, 428, "Redes Moveis");
	write_grade_curricular(file, 429, "Estatistica Descritiva");
	write_grade_curricular(file, 430, "Marketing Digital");
	write_grade_curricular(file, 431, "Extensao Curricular V");
	write_grade_curricular(file, 432, "Pratica de Fabrica de Software III");
	write_grade_curricular(file, 433, "TCC I");
	write_grade_curricular(file, 434, "Software para Sistemas Ubiquos");
	write_grade_curricular(file, 435, "Arquitetura de Software");
	write_grade_curricular(file, 436, "Extensao Curricular VI");
	write_grade_curricular(file, 437, "TCC II");
	write_grade_curricular(file, 438, "Lingua Brasileira de Sinais");
	write_grade_curricular(file, 439, "Quimica, Meio Ambiente e Sociedade");
	write_grade_curricular(file, 440, "Gestao de Custos");
	write_grade_curricular(file, 441, "Comportamento Organizacional");
	write_grade_curricular(file, 442, "Probabilidade");
	write_grade_curricular(file, 443, "Desenvolvimento de Software para Dispositivos Moveis");
	write_grade_curricular(file, 444, "Ciencia de Dados");
	write_grade_curricular(file, 445, "Leitura e Producao de Generos Academicos");
	write_grade_curricular(file, 446, "Introducao a Teoria da Computacao");
	write_grade_curricular(file, 447, "Redes Neurais e Aprendizado Profundo");
	write_grade_curricular(file, 448, "Tecnicas de Programacao para Middleware");
}

void print_full_grade_curricular(FILE* file) {
	printf("\n");
	for (int i = 0; i < NUM_MATERIAS; i++) {
		GradeCurricular gc_printing = read_grade_curricular(file, i + 389);
		print_grade_curricular(gc_printing);
	}
}

//END OF GRD_CUR

typedef struct _materia {
	int id;
	char nome[100];
	float nota1;
	float nota2;
	int faltas;
	char status[12];
} Materia;

const char* MATERIA_FORMAT_IN = "(DPAAIN.%03d;%[^;];%f;%f;%d;%[^)])";
const char* MATERIA_FORMAT_OUT = "(DPAAIN.%03d;%s;%.2f;%.2f;%d;%s)\n";

void write_materia(FILE* file, int id, char* nome, float nota1, float nota2, int faltas, char* status) {
	fseek(file, 0, SEEK_END);
	fprintf(file, MATERIA_FORMAT_OUT, id, nome, nota1, nota2, faltas, status);
}

Materia read_materia(FILE* file, int id_seek) {
	Materia mt;
	int id = 0, faltas = 0;
	char nome[100] = { "\0" }, status[10] = { "\0" };
	float nota1 = 0, nota2 = 0;

	int count = count_line_breaks(file);
	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, MATERIA_FORMAT_IN, &id, nome, &nota1, &nota2, &faltas, status);
		mt.id = id;
		strcpy(mt.nome, nome);
		mt.nota1 = nota1;
		mt.nota2 = nota2;
		mt.faltas = faltas;
		strcpy(mt.status, status);

		if (mt.id == id_seek) {
			return mt;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}

	printf("ERRO: ID de materia invalido.\n");
	Materia erro = { 0, "\0", 0, 0, 0, "\0" };
	return erro;
}

void delete_materia(FILE* file, int id_seek) {
	FILE* new_file = fopen("materia_new.dat", "w+");
	fseek(file, 0, SEEK_SET);
	int count = count_line_breaks(file);

	fseek(file, 0, SEEK_SET);
	int id = 0, faltas = 0;
	char nome[100] = { "\0" }, status[12] = { "\0" };
	float nota1 = 0, nota2 = 0;
	for (int i = 0; i < count; i++) {
		fscanf(file, MATERIA_FORMAT_IN, &id, nome, &nota1, &nota2, &faltas, status);
		if (id != id_seek) {
			write_materia(new_file, id, nome, nota1, nota2, faltas, status);
		}
		fseek(file, 2, SEEK_CUR);
	}

	fclose(file);
	fclose(new_file);
	if (remove("materia.dat") == 0 && rename("materia_new.dat", "materia.dat") == 0) {
		printf("Materia deletada com sucesso!\n");
	}
	else {
		printf("Erro ao deletar materia\n");
	}
	fopen("materia.dat", "a+");
}

void update_materia(FILE* file, int id_seek, char* nome, float nota1, float nota2, int faltas, char* status) {
	delete_materia(file, id_seek);
	write_materia(file, id_seek, nome, nota1, nota2, faltas, status);
	printf("Materia atualizada com sucesso!\n");
}

void print_materia(Materia mt) {
	printf("ID: DPAAIN.%03d, Nome: %s, Nota 1: %.2f, Nota 2: %.2f, Faltas: %d, %s\n", mt.id, mt.nome, mt.nota1, mt.nota2, mt.faltas, mt.status);
}

void print_all_materia(FILE* file) {
	int id = 0, faltas = 0;
	char nome[100] = { "\0" }, status[10] = { "\0" };
	float nota1 = 0, nota2 = 0;
	Materia mt;

	int count = count_line_breaks(file);

	printf("\n");

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, MATERIA_FORMAT_IN, &id, nome, &nota1, &nota2, &faltas, status);
		fseek(file, 2, SEEK_CUR);

		mt.id = id;
		strcpy(mt.nome, nome);
		mt.nota1 = nota1;
		mt.nota2 = nota2;
		mt.faltas = faltas;
		strcpy(mt.status, status);

		print_materia(mt);
	}
}

void fill_materia(FILE* materia, FILE* grade_curricular) {
	printf("Quantas materias deseja inserir? ");
	int am_materias = 0;
	scanf("%d", &am_materias);

	for (int i = 0; i < am_materias; i++) {
		printf("Insira os digitos do codigo da %da materia: ", i + 1);
		int digits_materia = 0;
		scanf("%d", &digits_materia);
		if (digits_materia == 433 || digits_materia == 437) {
			tcc_info++;
		}
		GradeCurricular gc_to_materia = read_grade_curricular(grade_curricular, digits_materia);
		printf("Insira suas notas na %da materia, separadas por um espaco (Se nao estiverem fechadas ainda, coloque 0) ", i + 1);
		double nota_1 = 0, nota_2 = 0;
		scanf("%lf %lf", &nota_1, &nota_2);

		printf("Quantas faltas voce teve nessa materia? ");
		int faltas = 0;
		scanf("%d", &faltas);

		printf("Ja Passou ou Reprovou nessa materia, ou ainda esta Cursando? <p/r/c> ");
		char status_materia = 0;
try_agn:scanf("\n%c", &status_materia);
		char status_final[15] = { '\0' };
		switch (status_materia) {
		case 'p':
			strcpy(status_final, "CONCLUIDA");
			break;
		case 'r':
			strcpy(status_final, "REPROVADO");
			break;
		case 'c':
			strcpy(status_final, "CURSANDO");
			break;
		default:
			printf("Caracter invalido digitado. Por favor, tente novamente.");
			goto try_agn;
		}
		write_materia(materia, digits_materia, gc_to_materia.nome_materia, nota_1, nota_2, faltas, status_final);

		printf("\n");
	}

}

//END OF MATERIA

typedef struct _aluno {
	double matricula;
	char nome[100];
	int semestre;
} Aluno;

const char* ALUNO_FORMAT_IN = "(%lf;%[^;];%d)";
const char* ALUNO_FORMAT_OUT = "(%.0lf;%s;%d)\n";

void write_aluno(FILE* file, double matricula, char* nome, int semestre) {
	fseek(file, 0, SEEK_END);
	fprintf(file, ALUNO_FORMAT_OUT, matricula, nome, semestre);
}

Aluno read_aluno(FILE* file) {
	fseek(file, 0, SEEK_SET);
	Aluno al;
	double matricula = 0;
	char nome[100] = { "\0" };
	int semestre = 0;

	
	fscanf(file, ALUNO_FORMAT_IN, &matricula, nome, &semestre);
	al.matricula = matricula;
	strcpy(al.nome, nome);
	al.semestre = semestre;

	if (al.matricula == 0) {
		printf("ERRO: Nao existe aluno cadastrado.");
		return al;
	}
	else {
		return al;
	}
}

void delete_aluno(FILE* file) {
	FILE* new_file = fopen("aluno_new.dat", "w+");
	fseek(file, 0, SEEK_SET);

	fclose(file);
	fclose(new_file);
	
	if (remove("aluno.dat") == 0 && rename("aluno_new.dat", "aluno.dat") == 0) {
		printf("Aluno deletado com sucesso!\n");
	}
	else {
		printf("Erro ao deletar aluno\n");
	}
	fopen("aluno.dat", "a+");
}

void update_aluno(FILE* file, char* nome, int semestre) {
	Aluno save_aluno_id = read_aluno(file);

	delete_aluno(file);
	write_aluno(file, save_aluno_id.matricula, nome, semestre);
	printf("Aluno atualizado com sucesso!\n");
}

void print_aluno(Aluno al) {
	printf("Matricula: %.0lf, Nome: %s, %do Semestre\n", al.matricula, al.nome, al.semestre);
}

//END OF ALUNO

void initial_filling(FILE* grade_curricular, FILE* aluno, FILE* materia, FILE* tcc, FILE* estagio, FILE* atividade_complementar) {
	fill_grade_curricular(grade_curricular);
	printf("Bem-vindo ao cadastro inicial. Vamos comecar pelos seus dados:\n");
	printf("Insira seu numero de matricula no IFG: ");
	double matricula = 0;
	scanf("%lf", &matricula);
	printf("Perfeito. Agora me diga seu nome completo ou nome social, como esta no seu cadastro do IFG: ");
	char nome_aluno[100] = { '/0' };
	scanf("\n%[^\n]", &nome_aluno);
	printf("Agora, preciso saber em qual semestre ou periodo de Engenharia de Software voce esta: ");
	int semestre_aluno = 0;
	scanf("%d", &semestre_aluno);
	write_aluno(aluno, matricula, nome_aluno, semestre_aluno);
	
	printf("Agora vamos cadastrar as suas materias. Deseja ver a matriz curricular do curso? <s/n> ");
	char yn_show_grd_cur = '\0';
	scanf("\n%c", &yn_show_grd_cur);
	if (yn_show_grd_cur == 's') {
		print_full_grade_curricular(grade_curricular);
	}

	fill_materia(materia, grade_curricular);

	printf("Deseja preencher atividades complementares? <s/n> ");
	char yn_initial_atv_com = 0;
	scanf("\n%c", &yn_initial_atv_com);
	if (yn_initial_atv_com == 's') {
		fill_atividade_complementar(atividade_complementar);
	}

	if (semestre_aluno >= 4) {
		printf("Deseja preencher estagios? <s/n> ");
		char yn_initial_estagio = 0;
		scanf("\n%c", &yn_initial_estagio);
		if (yn_initial_estagio == 's') {
			fill_estagio(estagio);
		}
	}

	if (semestre_aluno >= 8) {
		printf("Deseja preencher TCC? <s/n> ");
		char yn_initial_tcc = 0;
		scanf("\n%c", &yn_initial_tcc);
		if (yn_initial_tcc == 's') {
			fill_tcc(tcc);
		}
	}
}

//END OF INITIAL FILLING

void menu_print_main() {
	printf("\n-- AGENDA 33 v1.0 --\n");
	printf("Pressione o numero desejado e depois tecle ENTER\n");
	printf("1. Meu Cadastro\n");
	printf("2. Grade Curricular\n");
	printf("3. Minhas Materias\n");
	printf("4. Minhas Atividades Complementares\n");
	printf("5. Meus Estagios\n");
	printf("6. Meu TCC\n");
	printf("7. Sair\n");
	printf("\n");
	printf("Sua entrada: ");
}

//END OF MENU MAIN

void menu_print_aluno_options(int type) {
	switch (type) {
	case (1):
		printf("\n1. Ver dados do aluno\n");
		printf("2. Atualizar dados do aluno\n");
		printf("3. De volta ao menu principal\n");
		printf("\nSua entrada: ");
		break;
	case (2):
		printf("\n1. Atualizar nome/nome social e/ou semestre\n");
		printf("2. Voltar\n");
		printf("Nao e possivel alterar o codigo da matricula.\n");
		printf("\nSua entrada: ");
		break;
	default:
		printf(ERR_INVALID_PRINT_TYPE);
	}
}

void menu_aluno_update_data(FILE* file, Aluno al) {
	char aluno_upd_dat_next_action = '0';
	
	do {
		menu_print_aluno_options(2);
		scanf("\n%c", &aluno_upd_dat_next_action);
		switch (aluno_upd_dat_next_action) {
		case ('1'):
			printf(MSG_SAME_UPDATE);
			
			printf("\nNome atualmente cadastrado: %s\n", al.nome);
			printf("Insira o novo nome: ");
			char nome_upd[100] = { '\0' };
			scanf("\n%[^\n]", &nome_upd);
			
			printf("\nSemestre atualmente cadastrado: %d\n", al.semestre);
			printf("Insira o novo semestre: ");
			int semestre_upd = 0;
			scanf("%d", &semestre_upd);

			update_aluno(file, nome_upd, semestre_upd);

			break;
		case ('2'):
			printf("Voltando.\n");
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}

	} while (aluno_upd_dat_next_action != '2');

}

void menu_aluno(FILE* file) {
	char aluno_next_action = '0';
	do {
		menu_print_aluno_options(1);
		scanf("\n%c", &aluno_next_action);

		Aluno al = read_aluno(file);

		switch (aluno_next_action) {
		case ('1'):
			printf("O aluno cadastrado tem os seguintes dados: ");
			print_aluno(al);
			break;
		case ('2'):
			menu_aluno_update_data(file, al);
			break;
		case ('3'):
			printf("Voltando.\n");
		case ('\n'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
	} while (aluno_next_action != '3');
}

//END OF MENU ALUNO

void menu_print_grd_cur_options() {
	printf("\n1. Ver a grade curricular\n");
	printf("2. Voltar\n");
	printf("Nao e possivel fazer alteracoes na grade curricular.\n");
	printf("\nSua entrada: ");
}

void menu_grd_cur(FILE* file) {
	char grd_cur_next_action = '0';
	do {
		menu_print_grd_cur_options();
		scanf("\n%c", &grd_cur_next_action);
		switch (grd_cur_next_action) {
		case ('1'):
			print_full_grade_curricular(file);
			break;
		case('2'):
			printf("Voltando.\n");
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
	} while (grd_cur_next_action != '2');
}

//END OF MENU GRD_CUR

void menu_print_materia_options(int type) {
	switch (type) {
	case (1):
		printf("\n1. Ver materias\n");
		printf("2. Atualizar materias\n");
		printf("3. Cadastrar novas materias\n");
		printf("4. Deletar materias\n");
		printf("5. Voltar\n");
		printf("\nSua entrada: ");
		break;
	case (2):
		printf("\n1.Pesquisar por nome\n");
		printf("2. Voltar\n");
		printf("\nSua entrada: ");
		break;
	default:
		printf(ERR_INVALID_PRINT_TYPE);
	}
}

menu_materia_search_by_name(FILE* file) {
	int count = count_line_breaks(file);
	int id = 0, faltas = 0;
	char nome[100] = { '\0' }, status[15] = { '\0' }, nome_search[100] = { '\0' };
	float nota_1 = 0, nota_2 = 0;

	printf(MSG_EXACT_SEARCH);
	printf("Sua pesquisa: ");
	scanf("\n%[^\n]", &nome_search);
	
	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, MATERIA_FORMAT_IN, &id, nome, &nota_1, &nota_2, &faltas, status);

		if (strcmp(nome, nome_search) == 0) {
			Materia search_result = read_materia(file, id);
			printf("Materia encontrada!\n");
			printf("Os dados da materia sao: ");
			print_materia(search_result);
			return;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}
	printf(ERR_NO_RESULT);
}

void menu_materia_search(FILE* file) {
	char menu_mat_search_nxt_act = '0';
	do {
		print_all_materia(file);
		menu_print_materia_options(2);
		scanf("\n%c", &menu_mat_search_nxt_act);
		switch (menu_mat_search_nxt_act) {
		case ('1'):
			menu_materia_search_by_name(file);
			break;
		case ('2'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
		
	} while (menu_mat_search_nxt_act != '2');
}

void menu_materia_update(FILE* file) {
	print_all_materia(file);
	printf("\nInsira o ID da materia que deseja atualizar: ");
	int mat_id_for_update = 0;
	scanf("%d", &mat_id_for_update);

	Materia mat = read_materia(file, mat_id_for_update);
	int faltas = 0;
	char nome[100] = { '\0' }, status[15] = { '\0' };
	float nota1 = 0, nota2 = 0;

	if (mat.id != 0) {
		printf(MSG_SAME_UPDATE);
		printf("Nome atual: %s\n", mat.nome);
		printf("Insira o novo nome: ");
		scanf("\n%[^\n]", &nome);

		printf("Notas atuais: %.2f %.2f\n", mat.nota1, mat.nota2);
		printf("Insira as novas notas: ");
		scanf("%f %f", &nota1, &nota2);

		printf("Faltas atuais: %d\n", mat.faltas);
		printf("Insira o novo numero de faltas: ");
		scanf("%d", &faltas);

		printf("Status atual: %s\n", mat.status);
		printf("Insira o novo status: ", &status);
		scanf("\n%[^\n]", &status);

		update_materia(file, mat_id_for_update, nome, nota1, nota2, faltas, status);
	}
	else {
		printf(ERR_NO_RESULT);
	}
}

void menu_materia_delete(FILE* file) {
	print_all_materia(file);

	printf("Digite o ID da materia a ser deletada: ");
	int id_materia_delete = 0;
	scanf("%d", &id_materia_delete);

	delete_materia(file, id_materia_delete);
}

void menu_materia(FILE* materia, FILE* grd_cur) {
	char materia_next_action = '0';
	do {
		menu_print_materia_options(1);
		scanf("\n%c", &materia_next_action);
		switch (materia_next_action) {
		case ('1'):
			menu_materia_search(materia);
			break;
		case ('2'):
			menu_materia_update(materia);
			break;
		case ('3'):
			fill_materia(materia, grd_cur);
			break;
		case ('4'):
			menu_materia_delete(materia);
			break;
		case ('5'):
			printf("\nVoltando.\n");
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
	} while (materia_next_action != '5');
}

//END OF MENU MATERIA

void menu_print_atv_com_options(int type) {
	switch (type) {
	case (1):
		printf("\n1. Ver atividades complementares\n");
		printf("2. Atualizar atividades complementares\n");
		printf("3. Cadastrar novas atividades complementares\n");
		printf("4. Deletar atividades complementares\n");
		printf("5. Voltar\n");
		printf("\nSua entrada: ");
		break;
	case (2):
		printf("\n1.Pesquisar por nome\n");
		printf("2. Voltar\n");
		printf("\nSua entrada: ");
		break;
	default:
		printf(ERR_INVALID_PRINT_TYPE);
	}
}

void menu_atv_com_search_by_name(FILE* file) {
	int count = count_line_breaks(file);
	int id = 0, horas = 0;
	char nome[100] = { '\0' }, nome_search[100] = { '\0' };

	printf(MSG_EXACT_SEARCH);
	printf("Sua pesquisa: ");
	scanf("\n%[^\n]", &nome_search);

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, ATV_COM_FORMAT_IN, &id, nome, &horas);

		if (strcmp(nome, nome_search) == 0) {
			AtividadeComplementar search_result = read_atividade_complementar(file, id);
			printf("Atividade complementar encontrada!\n");
			printf("Os dados da atividade complementar sao: ");
			print_atividade_complementar(search_result);
			return;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}
	printf(ERR_NO_RESULT);
}

void menu_atv_com_search(FILE* file) {
	char menu_atv_com_search_nxt_act = '0';
	do {
		print_all_atv_com(file);
		menu_print_atv_com_options(2);
		scanf("\n%c", &menu_atv_com_search_nxt_act);
		switch (menu_atv_com_search_nxt_act) {
		case ('1'):
			menu_atv_com_search_by_name(file);
			break;
		case ('2'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}

	} while (menu_atv_com_search_nxt_act != '2');
}

void menu_atv_com_update(FILE* file) {
	print_all_atv_com(file);
	printf("\nInsira o ID da atividade complementar que deseja atualizar: ");
	int atv_com_id_for_update = 0;
	scanf("%d", &atv_com_id_for_update);

	AtividadeComplementar at = read_atividade_complementar(file, atv_com_id_for_update);
	int horas = 0;
	char nome[100] = { '\0' };

	if (at.id != 0) {
		printf(MSG_SAME_UPDATE);
		printf("Nome atual: %s\n", at.nome);
		printf("Insira o novo nome: ");
		scanf("\n%[^\n]", &nome);

		printf("Horas atuais: %.2f %.2f\n", at.horas);
		printf("Insira as novas horas: ");
		scanf("%d", &horas);

		update_atividade_complementar(file, atv_com_id_for_update, nome, horas);
	}
	else {
		printf(ERR_NO_RESULT);
	}
}

void menu_atv_com_delete(FILE* file) {
	print_all_atv_com(file);

	printf("Digite o ID da atividade complementar a ser deletada: ");
	int id_atv_com_delete = 0;
	scanf("%d", &id_atv_com_delete);

	delete_atividade_complementar(file, id_atv_com_delete);
}

void menu_atv_com(FILE* file) {
	char atv_com_next_action = '0';
	do {
		menu_print_atv_com_options(1);
		scanf("\n%c", &atv_com_next_action);
		switch (atv_com_next_action) {
		case ('1'):
			menu_atv_com_search(file);
			break;
		case ('2'):
			menu_atv_com_update(file);
			break;
		case ('3'):
			fill_atividade_complementar(file);
			break;
		case ('4'):
			menu_atv_com_delete(file);
			break;
		case ('5'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
	} while (atv_com_next_action != '5');
}

//END OF MENU ATV_COM

void menu_print_estagio_options(int type) {
	switch (type) {
	case (1):
		printf("\n1. Ver estagios\n");
		printf("2. Atualizar estagios\n");
		printf("3. Cadastrar novos estagios\n");
		printf("4. Deletar estagios\n");
		printf("5. Voltar\n");
		printf("\nSua entrada: ");
		break;
	case (2):
		printf("\n1.Pesquisar por nome\n");
		printf("2. Voltar\n");
		printf("\nSua entrada: ");
		break;
	default:
		printf(ERR_INVALID_PRINT_TYPE);
	}
}

void menu_estagio_search_by_name(FILE* file) {
	int count = count_line_breaks(file);
	int id = 0, horas = 0;
	char nome[100] = { '\0' }, nome_search[100] = { '\0' };

	printf(MSG_EXACT_SEARCH);
	printf("Sua pesquisa: ");
	scanf("\n%[^\n]", &nome_search);

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, ESTAGIO_FORMAT_IN, &id, nome, &horas);

		if (strcmp(nome, nome_search) == 0) {
			Estagio search_result = read_estagio(file, id);
			printf("Estagio encontrado!\n");
			printf("Os dados do estagio sao: ");
			print_estagio(search_result);
			return;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}
	printf(ERR_NO_RESULT);
}

void menu_estagio_search(FILE* file) {
	char menu_estagio_search_nxt_act = '0';
	do {
		print_all_estagio(file);
		menu_print_estagio_options(2);
		scanf("\n%c", &menu_estagio_search_nxt_act);
		switch (menu_estagio_search_nxt_act) {
		case ('1'):
			menu_estagio_search_by_name(file);
			break;
		case ('2'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}

	} while (menu_estagio_search_nxt_act != '2');
}

void menu_estagio_update(FILE* file) {
	print_all_estagio(file);
	printf("\nInsira o ID do estagio que deseja atualizar: ");
	int estagio_id_for_update = 0;
	scanf("%d", &estagio_id_for_update);

	Estagio es = read_estagio(file, estagio_id_for_update);
	int horas = 0;
	char nome[100] = { '\0' };

	if (es.id != 0) {
		printf(MSG_SAME_UPDATE);
		printf("Nome atual: %s\n", es.nome);
		printf("Insira o novo nome: ");
		scanf("\n%[^\n]", &nome);

		printf("Horas atuais: %.2f %.2f\n", es.horas);
		printf("Insira as novas horas: ");
		scanf("%d", &horas);

		update_estagio(file, estagio_id_for_update, nome, horas);
	}
	else {
		printf(ERR_NO_RESULT);
	}
}

void menu_estagio_delete(FILE* file) {
	print_all_atv_com(file);

	printf("Digite o ID da atividade complementar a ser deletada: ");
	int id_estagio_delete = 0;
	scanf("%d", &id_estagio_delete);

	delete_estagio(file, id_estagio_delete);
}

void menu_estagio(FILE* file) {
	char estagio_next_action = '0';
	do {
		menu_print_estagio_options(1);
		scanf("\n%c", &estagio_next_action);
		switch (estagio_next_action) {
		case ('1'):
			menu_estagio_search(file);
			break;
		case ('2'):
			menu_estagio_update(file);
			break;
		case ('3'):
			fill_estagio(file);
			break;
		case ('4'):
			menu_estagio_delete(file);
			break;
		case ('5'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
	} while (estagio_next_action != '5');
}

//END OF MENU ESTAGIO

void menu_print_tcc_options(int type) {
	switch (type) {
	case (1):
		printf("\n1. Ver TCCs\n");
		printf("2. Atualizar TCCs\n");
		printf("3. Cadastrar novos TCCs\n");
		printf("4. Deletar TCCs\n");
		printf("5. Voltar\n");
		printf("\nSua entrada: ");
		break;
	case (2):
		printf("\n1.Pesquisar por nome\n");
		printf("2. Voltar\n");
		printf("\nSua entrada: ");
		break;
	default:
		printf(ERR_INVALID_PRINT_TYPE);
	}
}

void menu_tcc_search_by_name(FILE* file) {
	int count = count_line_breaks(file);
	int id = 0;
	char nome[100] = { '\0' }, orientador[100] = { '\0' }, nome_search[100] = { '\0' };

	printf(MSG_EXACT_SEARCH);
	printf("Sua pesquisa: ");
	scanf("\n%[^\n]", &nome_search);

	fseek(file, 0, SEEK_SET);

	for (int i = 0; i < count; i++) {
		fscanf(file, TCC_FORMAT_IN, &id, nome, orientador);

		if (strcmp(nome, nome_search) == 0) {
			TCC search_result = read_tcc(file, id);
			printf("TCC encontrado!\n");
			printf("Os dados do TCC sao: ");
			print_tcc(search_result);
			return;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}
	printf(ERR_NO_RESULT);
}

void menu_tcc_search(FILE* file) {
	char menu_tcc_search_nxt_act = '0';
	do {
		print_all_tcc(file);
		menu_print_tcc_options(2);
		scanf("\n%c", &menu_tcc_search_nxt_act);
		switch (menu_tcc_search_nxt_act) {
		case ('1'):
			menu_tcc_search_by_name(file);
			break;
		case ('2'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}

	} while (menu_tcc_search_nxt_act != '2');
}

void menu_tcc_update(FILE* file) {
	print_all_tcc(file);
	printf("\nInsira o ID do TCC que deseja atualizar: ");
	int tcc_id_for_update = 0;
	scanf("%d", &tcc_id_for_update);

	TCC tc = read_tcc(file, tcc_id_for_update);
	char nome[100] = { '\0' }, orientador[100] = { '\0' };

	if (tc.id != 0) {
		printf(MSG_SAME_UPDATE);
		printf("Nome atual: %s\n", tc.nome);
		printf("Insira o novo nome: ");
		scanf("\n%[^\n]", &nome);

		printf("Orientador atual: %s\n", tc.orientador);
		printf("Insira o novo orientador: ");
		scanf("\n%[^\n]", &orientador);

		update_tcc(file, tcc_id_for_update, nome, orientador);
	}
	else {
		printf(ERR_NO_RESULT);
	}
}

void menu_tcc_delete(FILE* file) {
	print_all_tcc(file);

	printf("Digite o ID do TCC a ser deletado: ");
	int id_tcc_delete = 0;
	scanf("%d", &id_tcc_delete);

	delete_tcc(file, id_tcc_delete);
}

void menu_tcc(FILE* file) {
	char tcc_next_action = '0';
	do {
		menu_print_tcc_options(1);
		scanf("\n%c", &tcc_next_action);
		switch (tcc_next_action) {
		case ('1'):
			menu_tcc_search(file);
			break;
		case ('2'):
			menu_tcc_update(file);
			break;
		case ('3'):
			fill_tcc(file);
			break;
		case ('4'):
			menu_tcc_delete(file);
			break;
		case ('5'):
			break;
		default:
			printf(ERR_INVALID_CHARACTER);
		}
	} while (tcc_next_action != '5');
}

//END OF MENU TCC

int main() {
	FILE* grade_curricular, *aluno, *materia, *tcc, *estagio, *atividade_complementar;
	grade_curricular = fopen("grade_curricular.dat", "a+");
	aluno = fopen("aluno.dat", "a+");
	materia = fopen("materia.dat", "a+");
	tcc = fopen("tcc.dat", "a+");
	estagio = fopen("estagio.dat", "a+");
	atividade_complementar = fopen("atividade_complementar.dat", "a+");

	if (grade_curricular != NULL && aluno != NULL && materia != NULL && tcc != NULL && estagio != NULL && atividade_complementar != NULL) {
		printf("-- AGENDA 33 v1.0 --\n");
		printf("ATENCAO: ESSE PROGRAMA NAO SUPORTA CARACTERES ESPECIAIS COMO LETRAS ACENTUADAS E O CEDILHA. NAO UTILIZE-OS NAS SUAS ENTRADAS.\n");
		printf("Arquivos criados/abertos com sucesso.\n");
		char yn_first_execution = 'n';
		printf("Essa e a primeira vez que esta executando esse programa? <s/N> ");
		scanf("%c", &yn_first_execution);
		if (yn_first_execution == 's') {
			initial_filling(grade_curricular, aluno, materia, tcc, estagio, atividade_complementar);
		}

		char next_action = '0';

		do {
			menu_print_main();
			scanf("\n%c", &next_action);

			switch (next_action) {
			case ('1'):
				menu_aluno(aluno);
				break;
			case ('2'):
				menu_grd_cur(grade_curricular);
				break;
			case ('3'):
				menu_materia(materia, grade_curricular);
				break;
			case ('4'):
				menu_atv_com(atividade_complementar);
				break;
			case ('5'):
				menu_estagio(estagio);
				break;
			case ('6'):
				menu_tcc(tcc);
				break;
			case ('7'):
				printf("Ate mais!\n");
			case ('\n'):
				break;
			default:
				printf(ERR_INVALID_CHARACTER);
			}
		} while (next_action != '7');
	}
	else {
		printf("Erro ao abrir os arquivos necessarios para o funcionamento do programa.\n");
		printf("Certifique-se de que o programa tem as permissoes necessarias para a criacao de arquivos nessa pasta.\n");
	}

  	fclose(grade_curricular);
	fclose(aluno);
	fclose(materia);
	fclose(tcc);
	fclose(estagio);
	fclose(atividade_complementar);

	return 0;
}