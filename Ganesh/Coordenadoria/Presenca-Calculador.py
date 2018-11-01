import sys
import csv



#
 #	~  Contador de Presenças Ganesh  ~
 #	Este programa vai listar os membros presentes em alguma reunião específica.
 #	Ele recebe como entrada o arquivo *.CSV da tabela dos feedbacks separado
 #	 por COMMA.
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



if len(sys.argv) < 3 or len(sys.argv) > 4:
	print("Este programa lista os membros presentes em alguma reunião específica do Ganesh com base na tabela disponível no Google Drive.")
	print("É necessário baixar essa tabela e utilizar a COMMA como separador.")
	print("Uso do programa:")
	print("\tpython3 " + sys.argv[0] + " CAMINHO_CSV FILTRO [JUSTIFICADO]")
	print("Onde:")
	print("\tCAMINHO_CSV -> caminho para o arquivo *.CSV da tabela com colunas separadas por COMMA")
	print("\tFILTRO -> filtro que será usado para retornar os nomes")
	print("\t[JUSTIFICADO] -> opcional: se for igual a 1, considera as justificativas como presença")
	sys.exit(0);

with open(sys.argv[1], mode='r') as fStream:
	lines = csv.reader(fStream, delimiter=COMMA)
	lines = list(lines)

justified = False
frequency = { }

if len(sys.argv) == 4:
	if sys.argv[3] == '1':
		justified = True

for key in range(8, len(lines[0])):
	for member in lines[2:]:
		if member[key] != 'P' and (not justified or member[key] != 'J'):
			continue
		if not lines[0][key] in frequency:
			frequency[lines[0][key]] = []
		frequency[lines[0][key]].append(member)

for date in frequency:
	if sys.argv[2] not in date:
		continue
	print("== Reunião geral " + date + ": " + str(len(frequency[date])) + " membros presentes ==")
	for member in frequency[date]:
		print(member[1])
	print()


