from datetime import datetime
import random
import string
import names
import time
import sys
import csv


csvDel = ',' # Delimitador do CSV de saída.
maxCount = 150 # Número de itens do arquivo Pessoa.
Null1 = .15 # % de campos NOME nulos.
Null2 = .4 # % de campos IDADE nulos.
Null3 = .95 # % de lista_segue eliminados.
timeRandomVariation = 60*60*24*730 # padrão: 2 anos.



now = int(time.time())
lista = { }
count = 0

def randomPastDate():
	start_point = now - timeRandomVariation * 4
	start_point += random.uniform(-timeRandomVariation, timeRandomVariation)
	return datetime.fromtimestamp(start_point).strftime("%Y-%m-%d")

def randomFutureDate():
	start_point = now + timeRandomVariation * 4
	start_point += random.uniform(-timeRandomVariation, timeRandomVariation)
	return datetime.fromtimestamp(start_point).strftime("%Y-%m-%d")

if len(sys.argv) < 2 or len(sys.argv) > 3:
	print("Passe como argumento o nome do arquivo CSV a ser salvo e, opcionalmente, o nome do arquivo de seguidores.")
	sys.exit(0)

print("%.2f%% concluído..." % (0.0), end="")
while len(lista) < maxCount:
	print("\r%.2f%% concluído..." % (100 * len(lista)/maxCount), end="")
	idPessoa = str(count + 1)
	nomePessoa = names.get_full_name()
	for _ in range(random.randint(0, 4)):
		nomePessoa += " " + names.get_last_name()
	idadePessoa = str(random.randint(14, 40))
	twitterPessoa = nomePessoa.replace(" ", "").strip().lower()
	if len(twitterPessoa) > 14:
		twitterPessoa = twitterPessoa[:14]
	lista[twitterPessoa] = [idPessoa, nomePessoa, idadePessoa, twitterPessoa]
	count += 1
print("\r%.2f%% concluído... Pronto." % (100.0))

lista = [lista[k] for k in lista]

# Gerar lista de seguidores.
lista_segue = [ ]
print("%.2f%% concluído..." % (0.0), end="")
for i in range(len(lista)):
	print("\r%.2f%% concluído..." % (100 * i/len(lista)), end="")
	for pessoa2 in lista:
		if lista[i][0] == pessoa2[0]:
			continue
		lista_segue.append( [lista[i][0], pessoa2[0], "%d" % (random.randint(0, 2)), randomPastDate(), randomFutureDate()] )
print("\r%.2f%% concluído... Pronto." % (100.0))

Null1 = int(Null1 * len(lista))
Null2 = int(Null2 * len(lista))
Null3 = int(Null3 * len(lista_segue))

random.shuffle(lista)
for i in range(Null1):
	lista[i][1] = ""

random.shuffle(lista)
for i in range(Null2):
	lista[i][2] = "-1"

random.shuffle(lista_segue)
lista_segue = lista_segue[Null3:]

random.shuffle(lista)
random.shuffle(lista_segue)
lista.insert(0, ["idPessoa", "nomePessoa", "idadePessoa", "twitterPessoa"])
lista_segue.insert(0, ["idPessoaQueSegue", "idPessoaQueESeguida", "grauAmizade", "dataInicioQueSegue", "dataFimQueSegue"])

with open(sys.argv[1], 'w') as csvfile: # Salvar.
	spamwriter = csv.writer(csvfile, delimiter=csvDel, quotechar='', escapechar='\\', quoting=csv.QUOTE_NONE)
	spamwriter.writerows(lista)

if len(sys.argv) == 3:
	with open(sys.argv[2], 'w') as csvfile: # Salvar.
		spamwriter = csv.writer(csvfile, delimiter=csvDel, quotechar='', escapechar='\\', quoting=csv.QUOTE_NONE)
		spamwriter.writerows(lista_segue)
