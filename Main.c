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
}

typedef struct _estagio {
	int id;
	char nome[100];
	int horas;
} Estagio;

typedef struct _tcc {
	int id;
	char nome[100];
	char orientador[100];
} TCC;

typedef struct _materia {
	char id[10];
	char nome[100];
	float notas[2];
	char status[10];
} Materia;

typedef struct _aluno {
	char id[14];
	char nome[100];
	
} Aluno;

typedef struct _matricula {
	int id;
	int semestre;
	int id_aluno;
	int materias[10];
} Matricula;

typedef struct _curso {
	int id;
	char nome[100];
	int grade_curricular[70];
} Curso;

int main() {
	FILE* curso, *matricula, *aluno, *materia, *tcc, *estagio, *atividade_complementar;
	curso = fopen("curso.dat", "a+");
	matricula = fopen("matricula.dat", "a+");
	aluno = fopen("aluno.dat", "a+");
	materia = fopen("materia.dat", "a+");
	tcc = fopen("tcc.dat", "a+");
	estagio = fopen("estagio.dat", "a+");
	atividade_complementar = fopen("atividade_complementar.dat", "a+");

	write_atividade_complementar(atividade_complementar, 6, "Test 6", 18);
	AtividadeComplementar test = read_atividade_complementar(atividade_complementar, 6);
	update_atividade_complementar(atividade_complementar, 6, "Test 66", 20);
	delete_atividade_complementar(atividade_complementar, 6);

	
	fclose(curso);
	fclose(matricula);
	fclose(aluno);
	fclose(materia);
	fclose(tcc);
	fclose(estagio);
	fclose(atividade_complementar);
	

	return 0;
}