import sys
import csv



#
 #	~  Contador de Feedback Ganesh  ~
 #	Este programa vai fazer a contagem dos feedbacks recebidos do Ganesh e calcular
 #	 a percentagem.
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



if len(sys.argv) < 2 or len(sys.argv) > 3:
	print("Este programa calcula o Feedback para as atividades do Ganesh com base na tabela disponível no Google Drive gerada pelo formulário.")
	print("É necessário baixar essa tabela e utilizar a COMMA como separador.")
	print("Uso do programa:")
	print("\tpython3 " + sys.argv[0] + " CAMINHO_CSV [HEADER]")
	print("Onde:")
	print("\tCAMINHO_CSV -> caminho para o arquivo *.CSV da tabela com colunas separadas por COMMA")
	print("\t[HEADER] -> opcional: se for igual a 1, a primeira linha da tabela será ignorada")
	sys.exit(0);

with open(sys.argv[1], mode='r') as fStream:
	lines = csv.reader(fStream, delimiter=COMMA)
	lines = list(lines)

data = { }

if len(sys.argv) == 3:
	if sys.argv[2] == '1':
		del lines[0]

for response in lines:
	if not response[1] in data:
		data[response[1]] = [0, {}, {}, {}, [], []]
	data[response[1]][0] += 1
	if response[2] in data[response[1]][1]:
		data[response[1]][1][response[2]] += 1
	else:
		data[response[1]][1][response[2]] = 1
	if response[3] in data[response[1]][2]:
		data[response[1]][2][response[3]] += 1
	else:
		data[response[1]][2][response[3]] = 1
	if response[4] in data[response[1]][3]:
		data[response[1]][3][response[4]] += 1
	else:
		data[response[1]][3][response[4]] = 1
	if response[5]:
		data[response[1]][4].append(response[5])
	if response[6]:
		data[response[1]][5].append(response[6])

for dataKey in data:
	print("== " + dataKey + ": " + str(data[dataKey][0]) + " feedbacks recebidos ==")
	print("Gostou do tema da atividade?")
	for liked in sorted(data[dataKey][1]):
		print("\t- " + liked + ": " + str(data[dataKey][1][liked]) + " (" + str(round(data[dataKey][1][liked]*100/data[dataKey][0], 2)) + "%)")
	print("O quanto você entendeu do assunto?")
	for score in sorted(data[dataKey][2]):
		print("\t- " + score + "/5: " + str(data[dataKey][2][score]) + " (" + str(round(data[dataKey][2][score]*100/data[dataKey][0], 2)) + "%)")
	print("Vale a pena revisar o assunto em reuniões futuras?")
	for review in sorted(data[dataKey][3]):
		print("\t- " + review + ": " + str(data[dataKey][3][review]) + " (" + str(round(data[dataKey][3][review]*100/data[dataKey][0], 2)) + "%)")
	print("Algum assunto que gostaria para próximas atividades ou reuniões?")
	if len(data[dataKey][4]) < 1:
		print("\t(nada)")
	for subject in data[dataKey][4]:
		print("\t```\"" + subject + "\"```")
	print("Alguma observação ou sugestão de mudança?")
	if len(data[dataKey][5]) < 1:
		print("\t(nada)")
	for suggestion in data[dataKey][5]:
		print("\t```\"" + suggestion + "\"```")
	print()


