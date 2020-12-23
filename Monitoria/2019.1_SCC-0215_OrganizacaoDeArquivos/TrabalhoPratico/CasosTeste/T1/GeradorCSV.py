import csv
import sys
import random
import unicodedata
import re


csvDel = ',' # Delimitador do CSV de saída
maxCount = 5000 #maximo de itens
Null1 = 100 # Qtd de campos NOME nulos 
Null2 = 100 # Qtd de campos CARGO nulos
Null3 = 200 # Qtd de campos TELEFONE nulos
Null4 = 500 # Qtd de campos SALARIO nulos
tamVariaveis = 2000 # Tamanho maximo total dos dois campos variaveis

if len(sys.argv) != 3 and len(sys.argv) != 4:
    print("Passe o nome do arquivo csv de entrada de cadastro, de salarios/remoneracoes, e o nome do inicio do csv de saida respectivamente junto ao terminal.")
    print("\tpython3 codigo.py CADASTRO.CSV SALARIOS.CSV SAIDA")
    sys.exit()


def gerarFone():
    phone = '('
    phone += str(random.randint(11, 99)) # ddds disponiveis no BR
    phone += ')9' # celulares comecam com 9
    phone += str(random.randint(8000, 9999)) # 4 digitos
    phone += '-'
    phone += str(random.randint(1000, 9999)) # +4 digitos
    return phone

def truncarCamposVariaveis(nome, cargo):
    # remove acentuação e trunca tamanho
    nome = ''.join(c for c in unicodedata.normalize('NFD', nome)
                  if unicodedata.category(c) != 'Mn')
    cargo = ''.join(c for c in unicodedata.normalize('NFD', cargo)
                  if unicodedata.category(c) != 'Mn')
    nome = nome.upper().replace('Ç', 'C').replace(csvDel, '').strip()
    cargo = cargo.upper().replace('Ç', 'C').replace(csvDel, '').strip()
    nome = re.sub(r'[^a-zA-Z0-9\-\_\s\.]+', ' ', nome)
    cargo = re.sub(r'[^a-zA-Z0-9\-\_\s\.]+', ' ', cargo)
    if len(nome+cargo) <= tamVariaveis:
        return [nome, cargo]
    i = 0
    while len(nome+cargo) > tamVariaveis:
        nomeT = re.split(r'\s+', nome)
        cargoT = re.split(r'\s+', cargo)
        if i > 30 or len(nomeT) < 2 or len(cargoT) < 2:
            if i > 30:
                print("parece que um campo não foi devidamente trucado")
            nome = nome[:int(tamVariaveis/2)]
            cargo = cargo[:int(tamVariaveis/2)]
            break
        i += 1
        rnd = random.randint(1, len(nomeT) - 1)
        nomeT[rnd] = nomeT[rnd][0]
        rnd = random.randint(1, len(cargoT) - 1)
        cargoT[rnd] = cargoT[rnd][0]
        nome = ' '.join(nomeT)
        cargo = ' '.join(cargoT)
    return [nome, cargo]


listI = { }

ignore = True
count = 0

with open(sys.argv[1]) as csvfile: # cadastro de servidores
    spamreader = csv.reader(csvfile, delimiter=';')
    for row in spamreader:
        if ignore:
            ignore = False
            continue
        if not row[0] or row[0].lower().strip().startswith('sem '): #id
            continue
        if not row[1] or row[1].lower().strip().startswith('sem'): #nome
            continue
        if not row[4] or row[4].lower().strip().startswith('sem') or row[4].lower().strip().startswith('inv'): #cargo
            continue
        listI[row[0]] = [ row[0], row[1], row[4], "", 0 ]
        count += 1

i = 0
count = 0

with open(sys.argv[2]) as csvfile: # salarios
    spamreader = csv.reader(csvfile, delimiter=';')
    for row in spamreader:
        if row[2] in listI:
            if not row[5] or re.search(r'^\s*[0-9\.\,]+\s*$', row[5]) is None:
                listI[row[2]][4] = -1
            else:
                listI[row[2]][4] = float(row[5].replace(",", "."))
            count += 1
        i += 1

listValues = listI.values()
listI = list(listValues)

random.shuffle(listI)

listI = listI[:maxCount]

for item in listI:
    item[3] = gerarFone()
    camposVariaveis = truncarCamposVariaveis(item[1], item[2])
    item[1] = camposVariaveis[0]
    item[2] = camposVariaveis[1]

i = 0
for item in listI:
    if i >= Null1:
        break
    i += 1
    item[1] = ""
random.shuffle(listI)

i = 0
for item in listI:
    if i >= Null2:
        break
    i += 1
    item[2] = ""
random.shuffle(listI)

i = 0
for item in listI:
    if i >= Null3:
        break
    i += 1
    item[3] = ""
random.shuffle(listI)

i = 0
for item in listI:
    if i >= Null4:
        break
    i += 1
    item[4] = -1
random.shuffle(listI)

listI.insert(0, ['idServidor', 'nome', 'cargo', 'telefone', 'salario'])


if len(sys.argv) != 4:
    print(listI)
    sys.exit()

# == TURMA A

for item in listI:
    nome = item[1]
    cargo = item[2]
    telefone = item[3]
    salario = item[4]
    item[1] = salario
    item[2] = telefone
    item[3] = nome
    item[4] = cargo

with open(sys.argv[3] + "A.csv", 'w') as csvfile: # salvar
    spamwriter = csv.writer(csvfile, delimiter=csvDel, quotechar='', escapechar='\\', quoting=csv.QUOTE_NONE)
    spamwriter.writerows(listI)


# == TURMA B

for item in listI:
    nome = item[3]
    cargo = item[4]
    telefone = item[2]
    salario = item[1]
    item[1] = telefone
    item[2] = salario
    item[3] = cargo
    item[4] = nome

with open(sys.argv[3] + "B.csv", 'w') as csvfile: # salvar
    spamwriter = csv.writer(csvfile, delimiter=csvDel, quotechar='', escapechar='\\', quoting=csv.QUOTE_NONE)
    spamwriter.writerows(listI)

