import numpy as np
import math
import re

#
#	~ Trabalho 1: Implementação de MLP ~
#
#	Grupo:
#		João Victor Garcia Coelho (N 10349540)
#		Matheus Carvalho Raimundo (N 10369014)
#		Matheus Sanchez (N 9081453)
#
#	Introdução à Redes Neurais: SCC-0570 2019.1
#
#	 _______ _______ _______
#	|   |   |               \
#	|   |   |      \    |___|
#	|   |    \      |   |
#	|_______ _______|___|
#
#



# Constantes do programa

N_PERCEPTRONS_1ST_LAYER = 32 # Número de neurônios na camada oculta
LEARNING_RATE = 1 # Taxa de aprendizado
ERROR_THRESHOLD = 0.3 # Erro máximo pra ser considerado teste incorreto (recomendado: algo entre 0.1 e 0.3)



# Configuração inicial dos pesos w de cada neurônio de cada camada:
# 256 pontos de entrada (não considerado aqui) + N_PERCEPTRONS_1ST_LAYER neurônios na camada escondida + 10 neurônios de saída (0-9)

weights = np.array([ 0.1 * np.random.rand(N_PERCEPTRONS_1ST_LAYER, 256), 0.1 * np.random.rand(10, N_PERCEPTRONS_1ST_LAYER) ])
biases = np.array([ 0.1 * np.random.rand(N_PERCEPTRONS_1ST_LAYER), 0.1 * np.random.rand(10) ])
newWeights = np.array([ np.zeros((N_PERCEPTRONS_1ST_LAYER, 256)), np.zeros((10, N_PERCEPTRONS_1ST_LAYER)) ])
newBiases = np.array([ np.zeros(N_PERCEPTRONS_1ST_LAYER), np.zeros(10) ])

learned = 0



# Função que lê o arquivo de entrada.
def readData(filePath):
	v = [ ]
	try:
		with open(filePath, "r") as fStream:
			lines = fStream.readlines()
			for line in lines:
				aux = line.strip()
				if aux == '':
					print("Linha vazia encontrada no arquivo. Ignorando linha...")
					continue
				items = re.split(r'\s+', aux)
				if(len(items) != 266):
					print("Linha em formato inválido encontrada no arquivo. Ignorando linha...")
					continue
				v.append([ items[:256], items[256:266] ])
		for i in range(len(v)):
			for j in range(256):
				v[i][0][j] = int(float(v[i][0][j]))
			for j in range(10):
				v[i][1][j] = int(float(v[i][1][j]))
	except:
		return None
	return v

# Função de ativação (sigmóide)
def sigmoid(v, derivative = False):
	if derivative:
		return sigmoid(v) * (1 - sigmoid(v))
	return 1 / (1 + math.exp(-1 * v))

# Função que realiza o aprendizado a partir de uma única dada entrada (256 bits da imagem).
def learn(inputDigit, inputClass, activationFunction):
	global newBiases, newWeights, weights, biases, learned
	values = np.array(inputDigit, copy=True, dtype=np.float)
	x = [ ]
	for i in range(weights.shape[0]): # Para cada camada...
		v = np.empty(weights[i].shape[0], dtype=np.float)
		for j in range(weights[i].shape[0]): # Para cada neurônio na camada...
			aux = np.multiply(values, weights[i][j]) # Multiplicar valores pelos pesos.
			aux = np.sum(aux) + biases[i][j] # Somar tudo
			v[j] = aux
		x.append(v.copy())
		for j in range(weights[i].shape[0]): # Para cada neurônio na camada...
			v[j] = activationFunction(v[j])
		values = v
	# Agora, já temos os valores obtidos na iteração em "values".
	# Calcular erro com derivadas
	x = np.array(x)
	aux = np.empty(x[1].shape[0])
	for i in range(aux.shape[0]):
		aux[i] = activationFunction(x[1][i], True)
	error_2 = np.multiply(-2 * (np.array(inputClass, copy=True, dtype=np.float) - values), aux)
	aux = np.empty(x[0].shape[0])
	for i in range(aux.shape[0]):
		aux[i] = activationFunction(x[0][i])
	for i in range(weights[1].shape[0]):
		newWeights[1][i] += 1 * error_2[i] * aux
	newBiases[1] = newBiases[1] + 1 * error_2
	aux = np.empty(x[0].shape[0])
	for i in range(x[0].shape[0]):
		aux[i] = activationFunction(x[0][i], True)
	inputNp = np.array(inputDigit, copy=True)
	error_1 = 0
	for i in range(weights[0].shape[0]):
		error_1 = np.sum(error_2 * weights[1].T[i]) * aux[i]
		newWeights[0][i] += 1 * (error_1 * inputNp)
		newBiases[0][i] += 1 * error_1
	# Atualizar pesos
	biases -= LEARNING_RATE * newBiases
	weights -= LEARNING_RATE * newWeights
	newBiases *= 0
	newWeights *= 0
	learned += 1

# Função que testa uma única entrada na rede neural.
def test(inputDigit, inputClass, activationFunction):
	global weights, biases
	values = np.array(inputDigit, copy=True, dtype=np.float)
	for i in range(weights.shape[0]): # Para cada camada...
		v = np.empty(weights[i].shape[0], dtype=np.float)
		for j in range(weights[i].shape[0]): # Para cada neurônio na camada...
			aux = np.multiply(values, weights[i][j]) # Multiplicar valores pelos pesos.
			aux = np.sum(aux) + biases[i][j] # Somar tudo
			v[j] = activationFunction(aux)
		values = v
	return [ np.abs(np.array(inputClass, dtype=np.float) - values), values ]

# Função que divide o conjunto de dados em k partes.
def splitData(inputData, k):
	list = [ ]
	groupSize = int(len(inputData) / k)
	for i in range(k):
		list.append(inputData[i * groupSize:i * groupSize + groupSize])
	return list


# Implementação do menu principal

print("Bem-vinde à nossa rede neural artificial, implementada através de uma MLP.")
while True:
	print()
	print("== Opções ==")
	print("1. Treinar com conjunto de dados")
	print("2. Testar com conjunto de dados")
	print("3. Treinar e testar com conjunto de dados (k-fold)")
	print("4. Testar com conjunto de dados (com detalhes)")
	print("5. Listar pesos")
	print("6. Informações da Rede")
	print("0. Sair")
	print()
	option = 0
	option = int(input("Escolha uma opção (0-6) > "))
	if option == 1: # treinar com um arquivo de dados
		data = readData(input("Entre com um arquivo com o conjunto de dados (imagens) > ").strip())
		if data == None:
			print("Erro: o arquivo de dados não pode ser lido (não existe, sem permissões, ...) ou não está no formato adequado.")
			continue
		n = int(input("Entre com o número de iterações para aprendizado (n: 1-inf) > "))
		if n < 1:
			print("Erro: n deve ser maior ou igual à 1.")
			continue
		print()
		for it in range(n):
			print("\r%6.2lf%% processado... (não pressione nenhuma tecla ainda)" % (float(100 * it/n)), end='')
			np.random.shuffle(data)
			for item in data:
				learn(item[0], item[1], sigmoid)
		print("\r%6.2lf%% processado... (não pressione nenhuma tecla ainda)" % (float(100)), end='')
		print()
		print("# A rede aprendeu com o conjunto de dados submetido! Pesos atualizados.")
	elif option == 2: # testar com um arquivo de dados
		data = readData(input("Entre com um arquivo com o conjunto de dados (imagens) > ").strip())
		if data == None:
			print("Erro: o arquivo de dados não pode ser lido (não existe, sem permissões, ...) ou não está no formato adequado.")
			continue
		errorTotal = 0
		corrects = 0
		total = 0
		for item in data:
			itemStats = test(item[0], item[1], sigmoid)
			correct = True
			for error in itemStats[0]:
				errorTotal += error
				if error >= ERROR_THRESHOLD:
					correct = False
			if correct:
				corrects += 1
			total += 1
		print()
		print("# Teste terminou.")
		print("Acertos: %d de %d (%.2lf%%)." % (corrects, total, 100 * corrects/total))
		print("Erro: total de %.2lf e médio de %.2lf." % (errorTotal, errorTotal/total))
	elif option == 3: # k-fold através de um arquivo de dados
		data = readData(input("Entre com um arquivo com o conjunto de dados (imagens) > ").strip())
		if data == None:
			print("Erro: o arquivo de dados não pode ser lido (não existe, sem permissões, ...) ou não está no formato adequado.")
			continue
		n = int(input("Entre com o número de iterações para aprendizado (n) > "))
		if n < 1:
			print("Erro: n deve ser maior ou igual à 1.")
			continue
		k = int(input("Entre com o número de blocos usados k (k) > "))
		if k < 2 or k > len(data):
			print("Erro: k deve ser maior ou igual à 2 e deve ser menor ou igual ao número de exemplos no arquivo (" + str(len(data)) + ").")
			continue
		np.random.shuffle(data)
		errorTotal = 0
		corrects = 0
		total = 0
		groupsOfData = splitData(data, k)
		print()
		for kt in range(k):
			testData = groupsOfData[kt]
			learnData = [ ]
			for it in range(kt):
				learnData += groupsOfData[it]
			for it in range(kt + 1, k):
				learnData += groupsOfData[it]
			for it in range(n):
				print("\r%6.2lf%% processado... (não pressione nenhuma tecla ainda)" % (float(100 * (kt * n + it)/(k * n))), end='')
				for item in learnData:
					learn(item[0], item[1], sigmoid)
			errorKt = 0
			correctsKt = 0
			totalKt = 0
			for item in testData:
				itemStats = test(item[0], item[1], sigmoid)
				correct = True
				for error in itemStats[0]:
					errorKt += error
					if error >= ERROR_THRESHOLD:
						correct = False
				if correct:
					correctsKt += 1
				totalKt += 1
			errorTotal += errorKt
			corrects += correctsKt
			total += totalKt
			print("\r# A execução para a iteração k = " + str(kt + 1) + " do " + str(k) + "-fold foi completa.")
			print("Acertos: %d de %d (%.2lf%%)." % (correctsKt, totalKt, 100 * correctsKt/totalKt))
			print("Erro: total de %.2lf e médio de %.2lf." % (errorKt, errorKt/totalKt))
		print("\r%6.2lf%% processado... (não pressione nenhuma tecla ainda)" % (float(100)), end='')
		print()
		print("# A rede aprendeu com o conjunto de dados submetido! Pesos atualizados. Além disso também foi testada.")
		print("Acertos: %d de %d (%.2lf%%)." % (corrects, total, 100 * corrects/total))
		print("Erro: total de %.2lf e médio de %.2lf." % (errorTotal, errorTotal/total))
	elif option == 4: # testar com um arquivo de dados (detalhes)
		data = readData(input("Entre com um arquivo com o conjunto de dados (imagens) > ").strip())
		if data == None:
			print("Erro: o arquivo de dados não pode ser lido (não existe, sem permissões, ...) ou não está no formato adequado.")
			continue
		errorTotal = 0
		corrects = 0
		total = 0
		print()
		print("[saída esperada] -> [saída produzida] (acerto/erro)")
		for item in data:
			itemStats = test(item[0], item[1], sigmoid)
			correct = True
			errorItem = 0
			for error in itemStats[0]:
				errorItem += error
				if error >= ERROR_THRESHOLD:
					correct = False
			if correct:
				print("%s -> %s (acerto: com erro total de %.2lf)" % (str(np.array(item[1], dtype=np.float)), str(itemStats[1]), errorItem))
				corrects += 1
			else:
				print("%s -> %s (erro: de %.2lf)" % (str(np.array(item[1], dtype=np.float)), str(itemStats[1]), errorItem))
			total += 1
			errorTotal += errorItem
		print()
		print("# Teste terminou.")
		print("Acertos: %d de %d (%.2lf%%)." % (corrects, total, 100 * corrects/total))
		print("Erro: total de %.2lf e médio de %.2lf." % (errorTotal, errorTotal/total))
	elif option == 5: # pesos de um neurônio
		layer = int(input("Entre com a camada a qual deseja obter os pesos (layer: 1-" + str(weights.shape[0]) + ") > "))
		layer -= 1
		if layer < 0 or layer > 1:
			print("Erro: camada " + str(layer + 1) + " não existe.")
			continue
		i = int(input("Entre com o neurônio da camada " + str(layer) + " a qual deseja obter os pesos (i: 1-" + str(weights[layer].shape[0]) + ") > "))
		i -= 1
		n = weights[layer].shape[0]
		if i < 0 or i >= n:
			print("Erro: neurônio " + str(i + 1) + " não existe na camada " + str(layer + 1) + ".")
			continue
		print()
		print("# Pesos do neurônio " + str(i + 1) + " da camada " + str(layer + 1) + ":")
		print(weights[layer][i])
		print("Bias: " + str(biases[layer][i]))
	elif option == 6: # informações da rede neural
		print()
		print("# Informações da rede:")
		if learned > 0:
			print("Já foi treinada? Sim. Foi treinada usando um total de " + str(learned) + " exemplos.")
		else:
			print("Já foi treinada? Não. Os pesos são aleatórios.")
		print("Entrada: 256 inteiros ou floats")
		print("Saída esperada e produzida: 10 floats")
		print("Estrutura: " + str(weights.shape[0]) + " camadas, " + str(weights.shape[0] - 1) + " escondida/s e 1 de saída")
		for i in range(weights.shape[0]):
			print("\tNeurônios na camada " + str(i + 1) + ": " + str(weights[i].shape[0]))
	else: # sair
		print()
		print("# Saindo do programa...")
		break


