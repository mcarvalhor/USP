import unicodedata
import threading
import random
import string
import time
import sys
import csv
import re

import tweepy


CONSUMER_KEY = 'CONSUMER KEY HERE'
CONSUMER_SECRET = 'CONSUMER SECRET HERE'
ACCESS_KEY = 'ACCESS KEY HERE'
ACCESS_SECRET = 'ACCESS SECRET HERE'

RATE_SLEEP_INTERVAL = 15 * 60 # In seconds (recommended: 15 min = 60*15).
MAX_CONCURRENT_REQUESTS = 20

csvDel = ',' # Delimitador do CSV de saída
Null1 = .0 # % de campos NOME nulos
Null2 = .0 # % de campos IDADE nulos


lista = { }
count = 0

def normalize(text):
	text = ''.join(c for c in unicodedata.normalize('NFD', text) if unicodedata.category(c) != 'Mn')
	text = text.replace('Ç', 'C').replace('ç', 'c').replace(csvDel, '').strip()
	text = re.sub(r'[^a-zA-Z0-9\-\_\s\.]+', ' ', text)
	return text

def get_user(account): # Baixar dados e salvar na lista.
	global permit, lista
	permit.acquire()
	tries = 0
	while tries < 3:
		try:
			user = api.get_user(screen_name = account)
			user_data = [0, normalize(user.name) if type(user.name) == str else "", str(random.randint(16, 60)), account]
			lista.append(user_data)
			permit.release()
			return
		except tweepy.RateLimitError:
			time.sleep(RATE_SLEEP_INTERVAL)
		except:
			tries += 1
	permit.release()

if len(sys.argv) != 2:
	print("Passe como argumento o nome do arquivo CSV a ser salvo.")
	sys.exit(0)

# Autenticar
auth = tweepy.OAuthHandler(CONSUMER_KEY,CONSUMER_SECRET)
api = tweepy.API(auth)
auth.set_access_token(ACCESS_KEY, ACCESS_SECRET)

contas = { }

print("Entre com cada conta de Twitter em uma linha diferente, sem o '@' (use EOF ou CTRL+D para parar):")
while True:
	try:
		linha = input("> ").strip()
		if linha == "":
			continue
		contas[linha] = True
	except EOFError:
		break

print()
print("Por favor, aguarde enquanto buscamos os dados na API do Twitter...")

contas = list(contas.keys())
threads = [ ]
lista = [ ]

permit = threading.Semaphore(MAX_CONCURRENT_REQUESTS)

for conta in contas:
	threads.append(threading.Thread(target=get_user, args=(conta,)))

for t in threads:
	t.start()

for t in threads:
	t.join()

for i in range(0, len(lista)):
	lista[i][0] = str(i + 1)

Null1 = int(Null1 * len(lista))
Null2 = int(Null2 * len(lista))
random.shuffle(lista)

for i in range(Null1):
	lista[i][1] = ""

random.shuffle(lista)

for i in range(Null2):
	lista[i][2] = "-1"

random.shuffle(lista)

lista.insert(0, ["idPessoa", "nomePessoa", "idadePessoa", "twitterPessoa"])

with open(sys.argv[1], 'w') as csvfile: # Salvar.
	spamwriter = csv.writer(csvfile, delimiter=csvDel, quotechar='', escapechar='\\', quoting=csv.QUOTE_NONE)
	spamwriter.writerows(lista)
