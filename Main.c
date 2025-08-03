#include <stdio.h>
#include <string.h>
/*
 * Cada struct é seguido de seus formatadores e suas funções de CRUD.
 * Apenas as funções do primeiro struct serão comentadas, porque o
 * resto é igual.
 */
typedef struct _atividade_complementar {
	int id;
	char nome[100];
	int horas;
} AtividadeComplementar;

const char* AT_COMP_FORMAT_IN = "(%d;%[^;];%d)";
const char* AT_COMP_FORMAT_OUT = "(%d;%s;%d)\n";

/*
 * Traz o cursor para o final do arquivo, depois escreve nele de acordo
 * com o formato especificado acima.
 */
void write_atividade_complementar(FILE* file, int id, char* nome, int horas) {
	fseek(file, 0, SEEK_END);
	fprintf(file, AT_COMP_FORMAT_OUT, id, nome, horas);
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
		fscanf(file, AT_COMP_FORMAT_IN, &id, nome, &horas);
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
		fscanf(file, AT_COMP_FORMAT_IN, &id, nome, &horas);
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

	printf("ERRO: ID de estágio inválido.\n");
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
		printf("Estágio deletado com sucesso!\n");
	}
	else {
		printf("Erro ao deletar estágio\n");
	}
	fopen("estagio.dat", "a+");
}

void update_estagio(FILE* file, int id_seek, char* nome, int horas) {
	delete_estagio(file, id_seek);
	write_estagio(file, id_seek, nome, horas);
	printf("Estágio atualizado com sucesso!\n");
}

void print_estagio(Estagio es) {
	printf("ID: %d, Nome: %s, %d horas\n", es.id, es.nome, es.horas);
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

	printf("ERRO: ID de TCC inválido.\n");
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

//END OF TCC

typedef struct _materia {
	int id;
	char nome[100];
	float nota1;
	float nota2;
	char status[12];
} Materia;

const char* MATERIA_FORMAT_IN = "(DPAAIN.%03d;%[^;];%f;%f;%[^)])";
const char* MATERIA_FORMAT_OUT = "(DPAAIN.%03d;%s;%.2f;%.2f;%s)\n";

void write_materia(FILE* file, int id, char* nome, float nota1, float nota2, char* status) {
	fseek(file, 0, SEEK_END);
	fprintf(file, MATERIA_FORMAT_OUT, id, nome, nota1, nota2, status);
}

Materia read_materia(FILE* file, int id_seek) {
	fseek(file, 0, SEEK_SET);
	Materia mt;
	int id = 0;
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
		fscanf(file, MATERIA_FORMAT_IN, &id, nome, &nota1, &nota2, status);
		mt.id = id;
		strcpy(mt.nome, nome);
		mt.nota1 = nota1;
		mt.nota2 = nota2;
		strcpy(mt.status, status);

		if (mt.id == id_seek) {
			return mt;
		}
		else {
			fseek(file, 2, SEEK_CUR);
		}
	}

	printf("ERRO: ID de matéria inválido.\n");
	Materia erro = { 0, "\0", 0, 0, "\0" };
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
	int id = 0;
	char nome[100] = { "\0" }, status[12] = { "\0" };
	float nota1 = 0, nota2 = 0;
	for (int i = 0; i < count; i++) {
		fscanf(file, MATERIA_FORMAT_IN, &id, nome, &nota1, &nota2, status);
		if (id != id_seek) {
			write_materia(new_file, id, nome, nota1, nota2, status);
		}
		fseek(file, 2, SEEK_CUR);
	}

	fclose(file);
	fclose(new_file);
	if (remove("materia.dat") == 0 && rename("materia_new.dat", "materia.dat") == 0) {
		printf("Matéria deletada com sucesso!\n");
	}
	else {
		printf("Erro ao deletar matéria\n");
	}
	fopen("materia.dat", "a+");
}

void update_materia(FILE* file, int id_seek, char* nome, float nota1, float nota2, char* status) {
	delete_materia(file, id_seek);
	write_materia(file, id_seek, nome, nota1, nota2, status);
	printf("Matéria atualizada com sucesso!\n");
}

void print_materia(Materia mt) {
	printf("ID: DPAAIN.%03d, Nome: %s, Nota 1: %.2f, Nota 2: %.2f, %s\n", mt.id, mt.nome, mt.nota1, mt.nota2, mt.status);
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
		printf("ERRO: Não existe aluno cadastrado.");
		return al;
	}
	else {
		return al;
	}
}

void delete_aluno(FILE* file, double matricula_seek) {
	FILE* new_file = fopen("aluno_new.dat", "w+");
	fseek(file, 0, SEEK_SET);
	int count = 0, c = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}

	fseek(file, 0, SEEK_SET);
	double matricula = 0.0;
	char nome[100] = { '\0' };
	int semestre = 0;
	for (int i = 0; i < count; i++) {
		fscanf(file, ALUNO_FORMAT_IN, &matricula, nome, &semestre);
		if (matricula != matricula_seek) {
			write_aluno(new_file, matricula, nome, semestre);
		}
		fseek(file, 2, SEEK_CUR);
	}

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

void update_aluno(FILE* file, double matricula_seek, char* nome, int semestre) {
	delete_aluno(file, matricula_seek);
	write_aluno(file, matricula_seek, nome, semestre);
	printf("Aluno atualizado com sucesso!\n");
}

void print_aluno(Aluno al) {
	printf("Matrícula: %.0lf, Nome: %s, %do Semestre\n", al.matricula, al.nome, al.semestre);
}

//END OF ALUNO

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

	printf("ERRO: ID de matéria inválido.\n");
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
		printf("Matéria deletada com sucesso!\n");
	}
	else {
		printf("Erro ao deletar matéria\n");
	}
	fopen("grade_curricular.dat", "a+");
}

void update_grade_curricular(FILE* file, int id_seek, char* nome_materia) {
	delete_grade_curricular(file, id_seek);
	write_grade_curricular(file, id_seek, nome_materia);
	printf("Matéria atualizada com sucesso!\n");
}

void print_grade_curricular(GradeCurricular gr) {
	printf("ID: DPAAIN.%03d, Nome: %s\n", gr.id, gr.nome_materia);
}

//END OF GRD_CUR

int main() {
	FILE* grade_curricular, *aluno, *materia, *tcc, *estagio, *atividade_complementar;
	grade_curricular = fopen("grade_curricular.dat", "a+");
	aluno = fopen("aluno.dat", "a+");
	materia = fopen("materia.dat", "a+");
	tcc = fopen("tcc.dat", "a+");
	estagio = fopen("estagio.dat", "a+");
	atividade_complementar = fopen("atividade_complementar.dat", "a+");

	AtividadeComplementar at_com_test;
	Estagio estagio_test;
	TCC tcc_test;
	Materia materia_test;
	Aluno aluno_test;
	GradeCurricular gc_test;

	at_com_test = read_atividade_complementar(atividade_complementar, 1);
	estagio_test = read_estagio(estagio, 1);
	tcc_test = read_tcc(tcc, 1);
	materia_test = read_materia(materia, 1);
	aluno_test = read_aluno(aluno);
	gc_test = read_grade_curricular(grade_curricular, 3);

	print_atividade_complementar(at_com_test);
	print_estagio(estagio_test);
	print_tcc(tcc_test);
	print_materia(materia_test);
	print_aluno(aluno_test);
	print_grade_curricular(gc_test);

  	fclose(grade_curricular);
	fclose(aluno);
	fclose(materia);
	fclose(tcc);
	fclose(estagio);
	fclose(atividade_complementar);

	return 0;
}