from sklearn.cluster import KMeans
import numpy as np
import re

#
#	~ Trabalho 2: Implementação de uma rede RBF ~
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

N_CLUSTER_CENTERS = 60 # Número de neurônios na camada oculta (cluster centers)
ERROR_THRESHOLD = 0.5 # Erro máximo pra ser considerado teste incorreto (recomendado: algo entre 0.1 e 0.5)



# Configuração inicial dos pesos w de cada neurônio da camada de saída e dos cluster centers:
weights = None
centroids = None
sigma = None
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
				v[i][0][j] = float(v[i][0][j])
			for j in range(10):
				v[i][1][j] = float(v[i][1][j])
	except:
		return None
	return v

# Função de ativação (gaussiana)
def gauss(value, center):
	global sigma
	return np.exp(np.power(np.linalg.norm(value - center), 2)/np.power(5 * sigma, 2) * -1)

# Função que realiza o aprendizado a partir de um dataset de entrada (256 bits das N imagens).
def learn(dataset, activationFunction):
	global learned, centroids, weights, sigma
	values = np.array(dataset, copy=True)
	inputs = np.empty([values.shape[0], 256], dtype=np.float)
	classes = np.empty([values.shape[0], 10], dtype=np.float)
	distances = np.empty([values.shape[0], N_CLUSTER_CENTERS], dtype=np.float)
	for i in range(values.shape[0]):
		inputs[i] = values[i][0]
		classes[i] = values[i][1]
	# K-mean
	KMi = KMeans(n_clusters=N_CLUSTER_CENTERS, max_iter=100)
	KMi.fit(inputs)
	centroids = KMi.cluster_centers_
	# Calcular sigma
	distances = np.empty([N_CLUSTER_CENTERS, N_CLUSTER_CENTERS], dtype=np.float)
	for i in range(N_CLUSTER_CENTERS):
		for j in range(N_CLUSTER_CENTERS):
			distances[i][j] = np.linalg.norm(centroids[i] - centroids[j])
	sigma = np.amax(distances) / np.sqrt(N_CLUSTER_CENTERS)
	# Ajustar pesos
	distances = np.empty([values.shape[0], N_CLUSTER_CENTERS], dtype=np.float)
	for i in range(values.shape[0]):
		for j in range(N_CLUSTER_CENTERS):
			distances[i][j] = activationFunction(inputs[i], centroids[j])
	wFac = np.dot(np.linalg.inv(np.dot(distances.T, distances)), distances.T)
	weights = np.dot(wFac, classes).T
	learned += values.shape[0]

# Função que testa uma única entrada na rede neural.
def test(inputValues, activationFunction):
	global weights, centroids
	values = np.array(inputValues, copy=True)
	distance = np.empty([N_CLUSTER_CENTERS])
	for i in range(N_CLUSTER_CENTERS):
		distance[i] = activationFunction(np.array(values[0], dtype=np.float), centroids[i])
	result = np.empty(10, dtype=np.float)
	for i in range(10):
		result[i] = np.sum(np.multiply(weights[i], distance))
	#result = np.dot(distance, weights)
	return [ np.abs(np.array(values[1], dtype=np.float) - result), result ]

# Função que divide o conjunto de dados em k partes.
def splitData(inputData, k):
	list = [ ]
	groupSize = int(len(inputData) / k)
	for i in range(k):
		list.append(inputData[i * groupSize:i * groupSize + groupSize])
	return list


# Implementação do menu principal

print("Bem-vinde à nossa rede neural artificial, implementada através de uma rede RBF.")
while True:
	print()
	print("== Opções ==")
	print("1. Treinar com conjunto de dados")
	if learned > 0:
		print("2. Testar com conjunto de dados")
	print("3. Treinar e testar com conjunto de dados (k-fold)")
	if learned > 0:
		print("4. Testar com conjunto de dados (com detalhes)")
		print("5. Listar pesos da camada de saída")
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
		#n = int(input("Entre com o número de iterações para aprendizado (n: 1-inf) > "))
		n = 1
		if n < 1:
			print("Erro: n deve ser maior ou igual à 1.")
			continue
		print()
		for it in range(n):
			print("\r%6.2lf%% processado... (não pressione nenhuma tecla ainda)" % (float(100 * it/n)), end='')
			np.random.shuffle(data)
			learn(data, gauss)
		print("\r%6.2lf%% processado... (não pressione nenhuma tecla ainda)" % (float(100)), end='')
		print()
		print("# A rede aprendeu com o conjunto de dados submetido! Pesos atualizados.")
	elif learned > 0 and option == 2: # testar com um arquivo de dados
		data = readData(input("Entre com um arquivo com o conjunto de dados (imagens) > ").strip())
		if data == None:
			print("Erro: o arquivo de dados não pode ser lido (não existe, sem permissões, ...) ou não está no formato adequado.")
			continue
		errorTotal = 0
		corrects = 0
		total = 0
		for item in data:
			itemStats = test(item, gauss)
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
		#n = int(input("Entre com o número de iterações para aprendizado (n) > "))
		n = 1
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
				learn(learnData, gauss)
			errorKt = 0
			correctsKt = 0
			totalKt = 0
			for item in testData:
				itemStats = test(item, gauss)
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
	elif learned > 0 and option == 4: # testar com um arquivo de dados (detalhes)
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
			itemStats = test(item, gauss)
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
	elif learned > 0 and option == 5: # pesos de um neurônio da camada de saída
		i = int(input("Entre com o neurônio da camada de saída a qual deseja obter os pesos (i: 1-10) > "))
		i -= 1
		if i < 0 or i >= n:
			print("Erro: neurônio " + str(i + 1) + " não existe na camada de saída.")
			continue
		print()
		print("# Pesos do neurônio " + str(i + 1) + " da camada de saída:")
		print(weights[i])
	elif option == 6: # informações da rede neural
		print()
		print("# Informações da rede:")
		if learned > 0:
			print("Já foi treinada? Sim. Foi treinada usando um total de " + str(learned) + " exemplos.")
		else:
			print("Já foi treinada? Não.")
		print("Entrada: 256 inteiros ou floats")
		print("Saída esperada e produzida: 10 floats")
		print("Estrutura: 2 camadas, 1 escondida/s e 1 de saída")
		print("Número de neurônios na camada escondida (centros de clusters): " + str(N_CLUSTER_CENTERS))
		print("Número de neurônios na camada de saída: 10")
	else: # sair
		print()
		print("# Saindo do programa...")
		break


