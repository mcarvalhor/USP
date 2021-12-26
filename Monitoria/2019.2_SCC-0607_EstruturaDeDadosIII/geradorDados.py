from threading import Thread, Semaphore
from bs4 import BeautifulSoup as Soup
import urllib
import requests
import time
import queue
import csv
import sys
import random
import unicodedata
import re

# URL:
# http://www.distanciasentrecidades.com/pesquisa?from={from}&to={to}

# Todas as cidades que serão combinadas.
cities = ["São Carlos - SP", "São Paulo - SP", "Limeira - SP", "Araraquara - SP", "Barretos - SP", "Ribeirão Preto - SP", "Uberaba - MG", "Rio de Janeiro - RJ",
	"Cuiabá - MT", "Belo Horizonte - MG", "Florianópolis - SC", "Salvador - BA", "Manaus - AM", "Rio Branco - AC", "Maceió - AL", "Belém - PA",
	"Macapá - AP", "Brasília - DF", "Fortaleza - CE", "Vitória - ES", "Goiânia - GO", "Caldas Novas - GO", "São Luís - MA", "Campo Grande - MS",
	"Uberlândia - MG", "Mariana - MG", "Ouro Preto - MG", "João Pessoa - PB", "Curitiba - PR", "Recife - PE", "Teresina - PI", "Natal - RN",
	"Porto Alegre - RS", "Porto Velho - RO", "Boa Vista - RR", "Aracaju - SE", "Palmas - TO", "Gramado - RS", "Petrópolis - RJ", "Campos do Jordão - SP",
	"Foz do Iguaçu - PR", "Paraty - RJ", "Blumenau - SC", "Bento Gonçalves - RS", "Porto Seguro - BA", "Olinda - PE", "Bonito - MS", "Holambra - SP",
	"Niterói - RJ", "Rio Grande - RS", "Santos - SP"]

csvDel = ',' # Delimitador do CSV de saída
maxCount = 5000 #maximo de itens
Null6 = 200 # Qtd de campos TEMPO VIAGEM nulos
tamVariaveis = 2000 # Tamanho maximo total dos três campos variaveis
maxThreads = 5 # Máximo de threads filhas executando simultaneamente
sleepThread = 2 # Tempo em segundos que a thread dorme após requisição (evitar bloqueio de IP)
grafoDirecionado = False # Direcionado ou não-direcionado

timeRegExp = r'^\s*(?:([0-9]+)\s*d[a-zA-Z]*)?\s*(?:([0-9]+)\s*h[a-zA-Z]*)?\s*(?:([0-9]+)\s*m[a-zA-Z]*)?\s*(?:([0-9]+)\s*s[a-zA-Z]*)?\s*$'

def processCities(city1, city2, vector, mutexSem, syncSem):
	cidadeOrigem = city1[1].split('-', 2)[0].strip()
	estadoOrigem = city1[1].split('-', 2)[1].strip()
	cidadeDestino = city2[1].split('-', 2)[0].strip()
	estadoDestino = city2[1].split('-', 2)[1].strip()
	try:
		fetch = requests.get(sys.argv[1].replace('{from}', urllib.parse.quote(city1[0])).replace('{to}', urllib.parse.quote(city2[0])), timeout=10)
		if not fetch or fetch.status_code != 200:
			print("\rCombinação '%s' -> '%s' deu erro ao carregar. Ignorando tal..." % (city1[0], city2[0]))
			syncSem.release()
			return
		fetch = fetch.text
		soupObj = Soup(fetch, features="lxml")
		distancia = soupObj.select_one("#kmsruta") # Km
		tempoViagem = soupObj.select_one("#web #results+p strong") # horas, minutos, ...
		if distancia is None or tempoViagem is None:
			print("\rCombinação '%s' -> '%s' deu erro ao processar. Ignorando tal..." % (city1[0], city2[0]))
			syncSem.release()
			return
		distancia = distancia.text
		tempoViagemMatches = re.search(timeRegExp, tempoViagem.text)
		tempoViagem = 0
		if tempoViagemMatches is None:
			print("\rCombinação '%s' -> '%s' deu erro ao processar o tempo de viagem. Ignorando tal..." % (city1[0], city2[0]))
			syncSem.release()
			return
		if tempoViagemMatches.group(1) is not None:
			tempoViagem += 24 * 60 * 60 * int(tempoViagemMatches.group(1))
		if tempoViagemMatches.group(2) is not None:
			tempoViagem += 60 * 60 * int(tempoViagemMatches.group(2))
		if tempoViagemMatches.group(3) is not None:
			tempoViagem += 60 * int(tempoViagemMatches.group(3))
		if tempoViagemMatches.group(4) is not None:
			tempoViagem += int(tempoViagemMatches.group(4))
		if tempoViagem < 1:
			print("\rCombinação '%s' -> '%s' deu erro ao calcular o tempo de viagem. Ignorando tal..." % (city1[0], city2[0]))
			syncSem.release()
			return
	except Exception as ex:
		print("\rCombinação '%s' -> '%s' deu erro (%s). Ignorando tal..." % (city1[0], city2[0], str(ex)))
		syncSem.release()
		return
	tempoViagem /= 60 # minutos
	tempoViagemStr = str(int(tempoViagem % 60)) + "min"
	tempoViagemStr = str(int(tempoViagem/60)) + "h " + tempoViagemStr
	item = [estadoOrigem, estadoDestino, int(float(distancia.replace(".", ""))), cidadeOrigem, cidadeDestino, tempoViagemStr]
	mutexSem.acquire()
	vector.append(item)
	mutexSem.release()
	time.sleep(sleepThread)
	syncSem.release()

if len(sys.argv) != 2 and len(sys.argv) != 3:
	print("Passe a URL do site que vai fornecer os dados para gerar o CSV e o nome do arquivo final:")
	print("> python3 codigo.py SITE [ARQUIVO]")
	print("\tonde:")
	print("\tSITE -> é a URL http ou https, com os modificadores {from} e {to} que serão substituidos pelas cidades.")
	print("\t[ARQUIVO] -> é o nome do arquivo CSV que será salvo. Se não for passado, será impresso na tela.")
	sys.exit()

def removerAcentos(campos):
	# remove acentuação de campos
	for i in range(len(campos)):
		campos[i] = ''.join(c for c in unicodedata.normalize('NFD', campos[i])
				  if unicodedata.category(c) != 'Mn')
		campos[i] = campos[i].replace('ç', 'c').replace('Ç', 'C').replace(csvDel, '').strip()

def paraMaiusculo(campos):
	# torna campos maiusculos
	for i in range(len(campos)):
		campos[i] = campos[i].upper()

# Função nunca usada
'''
def truncarTamanho(campos):
	# trunca tamanho máximo de campos
	catcampos = ''.join(campos)
	lcatcampos = len(catcampos) + len(campos)
	olcatcampos = -1
	while lcatcampos > tamVariaveis:
		if(lcatcampos == olcatcampos):
			print("parece que um campo (i = %d) não foi devidamente trucado" % lcatcampos)
			print("encerrando script")
			sys.exit()
		splitingField = random.randint(0, len(campos))
		tmpFieldValues = re.split(r'\s+', campos[splitingField])
		tmpRnd = random.randint(1, len(tmpFieldValues) - 1)
		tmpFieldValues[tmpRnd] = tmpFieldValues[tmpRnd][0]
		campos[splitingField] = ' '.join(tmpFieldValues)
		catcampos = ''.join(campos)
		olcatcampos = lcatcampos
		lcatcampos = len(catcampos) + len(campos)
	return campos
'''

# Remover cidades repetidas
citiesLen = len(cities)
citiesUnique = list(dict.fromkeys(cities))
if len(citiesUnique) != citiesLen:
	citiesDict = { }
	citiesRepeated = [ ]
	for city in cities:
		if city in citiesDict:
			citiesRepeated.append(city)
		citiesDict[city] = 1
	print("Atenção, os seguintes items estão repetidos:")
	print(citiesRepeated)
	print()
cities = citiesUnique.copy()
citiesUpper = cities.copy()
removerAcentos(citiesUpper)
paraMaiusculo(citiesUpper)
semMutex = Semaphore()
semSync = Semaphore(maxThreads)
threadsQueue = queue.Queue()
items = [ ]

if grafoDirecionado:
	print("Com %d cidades na lista, aproximadamente %d itens serão gerados..." % (len(cities), len(cities) * len(cities) - len(cities)))
else:
	print("Com %d cidades na lista, aproximadamente %d itens serão gerados..." % (len(cities), (len(cities) * len(cities) - len(cities))/2))
print("Processando... 0%", end = '')
# Combinar todas as cidades
for i in range(0, len(cities)):
	for j in range(0, len(cities)):
		if i == j:
			continue
		if not grafoDirecionado and citiesUpper[i] > citiesUpper[j]: # Direcionado ou não direcionado?
			continue
		if len(citiesUpper[i] + citiesUpper[j]) + 6 > tamVariaveis:
			print("\rCombinação '%s' -> '%s' ignorada." % (citiesUpper[i], citiesUpper[j]))
			continue
		semMutex.acquire()
		print("\rProcessando... %d%%" % (100 * len(items) / (len(cities) * len(cities) - len(cities))), end = '')
		#print("\rCombinação '%s' -> '%s'..." % (citiesUpper[i], citiesUpper[j]))
		semMutex.release()
		semSync.acquire()
		currentThread = Thread(target=processCities, args=([cities[i], citiesUpper[i]], [cities[j], citiesUpper[j]], items, semMutex, semSync))
		threadsQueue.put(currentThread)
		currentThread.start()
while not threadsQueue.empty():
	threadsQueue.get().join()
print("\rProcessando... 100%")
print()

print("Foram gerados %d itens. Processando..." % (len(items)))

random.shuffle(items)
items = items[:maxCount]

i = 0

for item in items:
	if len(item[3] + item[4] + item[5]) + 6 > tamVariaveis:
		item[5] = ""
		i += 1

for item in items:
	if i >= Null6:
		break
	i += 1
	item[5] = ""

random.shuffle(items)
items.insert(0, ['estadoOrigem', 'estadoDestino', 'distancia', 'cidadeOrigem', 'cidadeDestino', 'tempoViagem'])


if len(sys.argv) != 3:
	print(items)
	sys.exit()

with open(sys.argv[2], 'w') as csvfile: # salvar
	spamwriter = csv.writer(csvfile, delimiter=csvDel, quotechar='', escapechar='\\', quoting=csv.QUOTE_NONE)
	spamwriter.writerows(items)

