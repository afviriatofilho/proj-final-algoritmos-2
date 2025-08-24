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

//MENSAGENS DE ERRO
//Para ter uma mensagem consistente através do programa, quando o mesmo
//erro ocorre.
const char* ERR_INVALID_CHARACTER = "Voce digitou um numero ou caracter invalido. Por favor, tente novamente.";

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
	fseek(file, 0, SEEK_SET);
	AtividadeComplementar at;
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	fseek(file, 0, SEEK_SET);
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

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
 * Essa função retorna o ID da última atividade complementar
 * armazenada no arquivo, para que a função fill_atividade_complementar
 * não repita IDs por utilizar variáveis alocadas dinâmicamente.
 */
int atv_com_get_last_id(FILE* file) {
	fseek(file, 0, SEEK_SET);
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	fseek(file, 0, SEEK_SET);
	Estagio es;
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	fseek(file, 0, SEEK_SET);
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

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

int est_get_last_id(FILE* file) {
	fseek(file, 0, SEEK_SET);
	int id = 0, horas = 0;
	char nome[100] = { "\0" };

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	fseek(file, 0, SEEK_SET);
	TCC tc;
	int id = 0;
	char nome[100] = { "\0" }, orientador[100] = { '\0' };

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	fseek(file, 0, SEEK_SET);
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

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

int tcc_get_last_id(FILE* file) {
	fseek(file, 0, SEEK_SET);
	int id = 0;
	char nome[100] = { "\0" }, orientador[100] = {'\0'};

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	fseek(file, 0, SEEK_SET);
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

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
	fseek(file, 0, SEEK_SET);
	Materia mt;
	int id = 0, faltas = 0;
	char nome[100] = { "\0" }, status[10] = { "\0" };
	float nota1 = 0, nota2 = 0;

	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
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
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

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
	printf("ID: DPAAIN.%03d, Nome: %s, Nota 1: %.2f, Nota 2: %.2f, Faltas: %d, %s\n", mt.id, mt.nome, mt.nota1, mt.nota2, mt.status);
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
		printf("\nSe voce esta vendo essa mensagem, houve um erro da parte do programador. Por favor, comunique isso a ele.\n");
	}
}

void menu_aluno_update_data(FILE* file, Aluno al) {
	char aluno_upd_dat_next_action = '0';
	
	do {
		menu_print_aluno_options(2);
		scanf("\n%c", &aluno_upd_dat_next_action);
		switch (aluno_upd_dat_next_action) {
		case ('1'):
			printf("\nSe nao quiser alterar um ou ambos os valores, apenas digite o mesmo valor\n");
			
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

/*void menu_print_materia_options() {
	printf("\n1. Ver materias")
}*/

void menu_materia() {
	char materia_next_action = '0';
	do {
		printf("PLACEHOLDER IMPLEMENTATION. TYPE e (LOWERCASE) TO EXIT.\n");
		scanf("\n%c", &materia_next_action);
	} while (materia_next_action != 'e');
}

void menu_atv_com() {
	char atv_com_next_action = '0';
	do {
		printf("PLACEHOLDER IMPLEMENTATION. TYPE e (LOWERCASE) TO EXIT.\n");
		scanf("\n%c", &atv_com_next_action);
	} while (atv_com_next_action != 'e');
}

void menu_estagio() {
	char estagio_next_action = '0';
	do {
		printf("PLACEHOLDER IMPLEMENTATION. TYPE e (LOWERCASE) TO EXIT.\n");
		scanf("\n%c", &estagio_next_action);
	} while (estagio_next_action != 'e');
}

void menu_tcc() {
	char tcc_next_action = '0';
	do {
		printf("PLACEHOLDER IMPLEMENTATION. TYPE e (LOWERCASE) TO EXIT.\n");
		scanf("\n%c", &tcc_next_action);
	} while (tcc_next_action != 'e');
}

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
				menu_materia();
				break;
			case ('4'):
				menu_atv_com();
				break;
			case ('5'):
				menu_estagio();
				break;
			case ('6'):
				menu_tcc();
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