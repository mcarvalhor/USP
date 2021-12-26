import sys
import csv



#
 #	~  Contador de Membros Ganesh  ~
 #	Este programa vai fazer a contagem dos membros do Ganesh e calcular
 #	 a percentagem por curso e ano.
 #	Ele recebe como entrada o arquivo *.CSV da tabela dos membros separado
 #	 por COMMA.
 #	Abaixo estão algumas definições de pré-compilação que podem ajudar.
 #
 #	MATHEUS CARVALHO RAIMUNDO (017)
 #	Secretário (2018.2)
 #	mcarvalhor@usp.br
 #
 #	 _______ _______ _______
 #	|   |   |               \
 #	|   |   |      \    |___|
 #	|   |    \      |   |
 #	|_______ _______|___|
 #
#

COMMA = ',' # Delimitador de coluna do *.CSV



if len(sys.argv) < 2 or len(sys.argv) > 3:
	print("Este programa faz a contagem dos membros do Ganesh e calcula a percentagem por curso e ano.")
	print("É necessário baixar a tabela dos membros do Ganesh, disponível no Google Drive, e utilizar a COMMA como separador.")
	print("Uso do programa:")
	print("\tpython3 " + sys.argv[0] + " CAMINHO_CSV [HEADER]")
	print("Onde:")
	print("\tCAMINHO_CSV -> caminho para o arquivo *.CSV da tabela com colunas separadas por COMMA")
	print("\t[HEADER] -> opcional: se for igual a 1, a primeira linha da tabela será ignorada")
	sys.exit(0);

with open(sys.argv[1], mode='r') as fStream:
	lines = csv.reader(fStream, delimiter=COMMA)
	lines = list(lines)

courses = { }
nMembers = 0

if len(sys.argv) == 3:
	if sys.argv[2] == '1':
		del lines[0]

for member in lines:
	if member[8] == 'Inativo':
		continue
	nMembers += 1
	if not member[3] in courses:
		courses[member[3]] = [0, {}]
	courses[member[3]][0] += 1
	if member[4] in courses[member[3]][1]:
		courses[member[3]][1][member[4]] += 1
	else:
		courses[member[3]][1][member[4]] = 1

print("== Membros do Ganesh: " + str(nMembers) + " ==")
print()
for course in sorted(courses):
	print("~ " + course + ": " + str(courses[course][0]) + " (" + str(round(courses[course][0]*100/nMembers, 2)) + "%) ~")
	for year in sorted(courses[course][1]):
		print("\t" + year + " -> " + str(courses[course][1][year]) + " (" + str(round(courses[course][1][year]*100/nMembers, 2)) + "%)")
	print()

