#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

using namespace std;



/*
 *	~  Contador de Membros Ganesh  ~
 *	Este programa vai fazer a contagem dos membros do Ganesh
 *	 e calcular a percentagem por curso e ano.
 *	Ele recebe como entrada o arquivo *.CSV da tabela dos membros
 *	 separado por COMMA.
 *	Abaixo estão algumas definições de pré-compilação que podem ajudar.
 *
 * MATHEUS CARVALHO RAIMUNDO
 * mcarvalhor@usp.br
 *
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/


/* Definições de pré-compilação */
#define COMMA ',' /* Separador de colunas do *.CSV */
#define COURSE_COLUMN 3 /* Coluna (começa em 0) em que o nome do curso está localizado */
#define YEAR_COLUMN 4 /* Coluna (começa em 0) em que o ano do curso está localizado (use o mesmo do anterior caso seja a mesma coluna) */
#define STATUS_COLUMN 8 /* Coluna (começa em 0) em que o estado do membro está localizado (ativo ou inativo) */
#define INACTIVE_STR "Inativo" /* String que representa se um membro está inativo */



typedef struct {
	vector<string> columns;
	string course, year;
	bool active;
} MEMBER;

void readFile(vector<MEMBER>&, bool, FILE *);



/* == MAIN == */

int main(int argc, char **argv) {
	map< string, pair< int, map<string, int> > >::iterator mit;
	map< string, pair< int, map<string, int> > > data;
	map<string, int>::iterator mmit;
	vector<MEMBER>::iterator vit;
	vector<MEMBER> members;
	FILE *stream;
	bool header;
	int N;

	if(argc < 2 || argc > 3) {
		cerr << "Uso do programa:" << endl << '\t' << argv[0] << " ARQUIVO_CSV [HEADER]" << endl << "Onde:" << endl
			<< "\tARQUIVO_CSV -> caminho para o arquivo *.CSV da tabela com colunas separadas por COMMA" << endl
			<< "\t[HEADER] -> opcional: se for igual a 1, a primeira linha da tabela será ignorada" << endl;
		return EXIT_FAILURE;
	}

	header = false;
	if(argc == 3 && atoi(argv[2]) == 1)
		header = true;

	stream = fopen(argv[1], "r");
	if(stream == NULL) {
		cerr << "Não foi possível abrir o arquivo para leitura: " << argv[1] << endl;
		return EXIT_FAILURE;
	}

	members = vector<MEMBER>();
	readFile(members, header, stream);

	fclose(stream);

	for(vit = members.begin(), N = 0; vit != members.end(); vit++) {
		if(vit->active == false)
			continue;
		N++;
		data[vit->course].first++;
		data[vit->course].second[vit->year]++;
	}

	cout << fixed << setprecision(2) << "== Membros do Ganesh: " << N << " ==" << endl;
	for(mit = data.begin(); mit != data.end(); mit++) {
		#if COURSE_COLUMN == YEAR_COLUMN
			cout << "\t" << mit->first << ": " << mit->second.first << " (" << (mit->second.first * 100.0)/N << "%)" << endl;
		#else
			cout << endl << "~ " << mit->first << ": " << mit->second.first << " (" << (mit->second.first * 100.0)/N << "%) ~" << endl;
			for(mmit = mit->second.second.begin(); mmit != mit->second.second.end(); mmit++) {
				cout << "\t" << mmit->first << " -> " << mmit->second << " (" << (mmit->second * 100.0)/N << "%)" << endl;
			}
		#endif
	}

	return EXIT_SUCCESS;
}



/* Funções para leitura do *.CSV */

int readColumn(string& dst, FILE *stream) {
	/*
	* Lê uma coluna do *.CSV em 'stream' para a String em 'dst'.
	* Retorna 0 caso EOF ou erro, 1 caso fim de linha, outros valores caso contrário.
	*/
	bool ig, qt;
	char aux;
	qt = ig = false;
	while((aux = fgetc(stream)) != EOF) {
		if(ig == false) {
			if(aux == '"') {
				qt = !qt;
				continue;
			}
			if(qt == false && (aux == '\n' || aux == '\r')) {
				if((aux = fgetc(stream)) != '\n' && aux != '\r')
					fseek(stream, -1, SEEK_CUR);
				return 1;
			}
			if(qt == false && aux == COMMA)
				return 2;
			if(aux == '\\') {
				ig = true;
				continue;
			}
		}
		ig = false;
		dst += aux;
	}
	return 0;
}

int readLine(MEMBER *dst, FILE *stream) {
	/*
	* Lê uma linha do *.CSV em 'stream' inteira para uma estrutura do tipo MEMBER em 'dst'.
	* Retorna 0 caso EOF ou erro, outros valores caso contrário.
	*/
	string column;
	int end;
	column = string();
	end = readColumn(column, stream);
	while(end != 0 && end != 1) {
		dst->columns.push_back(column);
		column = string();
		end = readColumn(column, stream);
	}
	dst->columns.push_back(column);
	return end;
}

void readFile(vector<MEMBER>& dst, bool header, FILE *stream) {
	/*
	* Lê todas as linhas e coluna presentes no arquivo.
	* Ignora a primeira linha (possível header) se 'header' for verdade.
	*/
	MEMBER aux;
	int end;
	end = 1;
	while(end != 0) {
		aux.columns = vector<string>();
		end = readLine(&aux, stream);
		aux.course = aux.columns[COURSE_COLUMN];
		if(YEAR_COLUMN != COURSE_COLUMN)
			aux.year = aux.columns[YEAR_COLUMN];
		if(aux.columns[STATUS_COLUMN] == INACTIVE_STR)
			aux.active = false;
		else
			aux.active = true;
		if(header == true)
			header = false;
		else
			dst.push_back(aux);
	}
}


