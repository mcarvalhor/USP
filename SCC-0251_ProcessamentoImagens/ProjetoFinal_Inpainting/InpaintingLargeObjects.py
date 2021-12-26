from threading import Thread, Semaphore
import numpy as np
import imageio
import sys



#
 #	~ Projeto Final: Inpainting de Objetos Largos ~
 #
 #	Gabriel Romualdo Silveira Pupo (Nº USP 9896250)
 #	Matheus Carvalho Raimundo (Nº USP 10369014)
 #
 #	Processamento de Imagens: SCC-0251 2018.1
 #
 #	 _______ _______ _______
 #	|   |   |               \
 #	|   |   |      \    |___|
 #	|   |    \      |   |
 #	|_______ _______|___|
 #
#



# == VARIAVEIS GLOBAIS ==

semaphoreSavePatch = Semaphore() # Semáfaro de sincronização multi-threading.



# == DEFINIÇÃO DE FUNÇÕES ==

def computePatchPriorities(inpaintingMask, textureSize):
	"Esta função vai definir os patches (blocos) que serão processados neste momento no algoritmo."
	inpaintingMaskPoints = { }
	finalPoints = { }
	for point in list(map(tuple, np.argwhere(inpaintingMask != 0))): # Obter todos os pontos da máscara.
		if (point[0] - 1 >= 0 and inpaintingMask[point[0] - 1, point[1]].all() == 0 or # Verificar se ponto é de extremidade.
			point[1] - 1 >= 0 and inpaintingMask[point[0], point[1] - 1].all() == 0 or
			point[0] - 1 >= 0 and point[1] - 1 >= 0 and inpaintingMask[point[0] - 1, point[1] - 1].all() == 0 or
			point[0] + 1 < inpaintingMask.shape[0] and inpaintingMask[point[0] + 1, point[1]].all() == 0 or
			point[1] + 1 < inpaintingMask.shape[1] and inpaintingMask[point[0], point[1] + 1].all() == 0 or
			point[0] + 1 < inpaintingMask.shape[0] and point[1] + 1 < inpaintingMask.shape[1] and inpaintingMask[point[0] + 1, point[1] + 1].all() == 0 or
			point[0] - 1 >= 0 and point[1] + 1 < inpaintingMask.shape[1] and inpaintingMask[point[0] - 1, point[1] + 1].all() == 0 or
			point[0] + 1 < inpaintingMask.shape[0] and point[1] - 1 >= 0 and inpaintingMask[point[0] + 1, point[1] - 1].all() == 0):
			inpaintingMaskPoints[(point[0], point[1])] = 1 # Caso sim, adicionar para análise.
	for point in inpaintingMaskPoints.copy(): # Verificar pontos repetidos (não há necessidade de processar um mesmo patch mais de uma vez).
		if inpaintingMaskPoints[point[0], point[1]] == 0:
			continue
		inpaintingMaskPoints[point[0], point[1]] = 1
		for i in range(textureSize):
			inpaintingMaskPoints[(point[0] - i - 1, point[1])] = 0
			inpaintingMaskPoints[(point[0] + i + 1, point[1])] = 0
			inpaintingMaskPoints[(point[0], point[1] - i - 1)] = 0
			inpaintingMaskPoints[(point[0], point[1] + i + 1)] = 0
			for j in range(textureSize):
				inpaintingMaskPoints[(point[0] - i - 1, point[1] + j + 1)] = 0
				inpaintingMaskPoints[(point[0] + i + 1, point[1] + j + 1)] = 0
				inpaintingMaskPoints[(point[0] - i - 1, point[1] - j - 1)] = 0
				inpaintingMaskPoints[(point[0] + i + 1, point[1] - j - 1)] = 0
	for point in inpaintingMaskPoints: # Remover pontos repetidos.
		if inpaintingMaskPoints[point] != 0:
			finalPoints[point] = 1
	return list(finalPoints)

def processPatch(originalImage, maskImage, textureSize, analysisWindowRatio, processingPoint):
	"Esta função vai processar um único patch e realizar o Inpainting na imagem original."
	# Realizar cortes da janela de análise.
	slicedImage = originalImage[max(0, processingPoint[0] - textureSize * analysisWindowRatio):processingPoint[0] + textureSize * analysisWindowRatio + 1, max(0, processingPoint[1] - textureSize * analysisWindowRatio):processingPoint[1] + textureSize * analysisWindowRatio + 1]
	slicedMask = maskImage[max(0, processingPoint[0] - textureSize * analysisWindowRatio):processingPoint[0] + textureSize * analysisWindowRatio + 1, max(0, processingPoint[1] - textureSize * analysisWindowRatio):processingPoint[1] + textureSize * analysisWindowRatio + 1]
	# Realizar cortes do patch.
	processingWindow = originalImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1]
	processingMask = maskImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1]
	processingMask = np.where(processingMask > 0, 1, 0)
	bestMatchWindow = None
	bestMatch = 10000000 # Como se fosse INT_MAX.
	for x in range(slicedImage.shape[0] - processingMask.shape[0]): # Para cada pixel da janela de análise.
		for y in range(slicedImage.shape[1] - processingMask.shape[1]):
			if np.sum(slicedMask[x:x + processingMask.shape[0], y:y+processingMask.shape[1]]) > 0: # Verificar se não está dentro do patch em análise na máscara.
				continue
			auxWindow = slicedImage[x : x + processingMask.shape[0], y : y + processingMask.shape[1]] # Cortar um patch a partir deste pixel para comparação.
			auxRMSE = np.power(auxWindow - processingWindow, 2) # Calcular semelhança.
			auxRMSE = np.sqrt(np.sum(np.multiply(auxRMSE, 1 - processingMask)))
			if auxRMSE < bestMatch: # Se for o mais semelhante, definir como o melhor match com o patch em análise até o momento.
				bestMatchWindow = auxWindow
				bestMatch = auxRMSE
	if bestMatchWindow is None: # Não foi possível realizar o Inpainting desse patch nesse momento. Nas próximas iterações apenas.
		return False
	semaphoreSavePatch.acquire() # Aguardar semáfaro.
	'''originalImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1] = np.multiply(1 - processingMask, originalImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1]).astype(np.uint8)
	originalImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1] += np.multiply(processingMask, bestMatchWindow).astype(np.uint8)'''
	# Realizar Inpainting: substituição bruta de conteúdo.
	originalImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1] = bestMatchWindow.astype(np.uint8)
	# Marcar patch como solucionado na máscara.
	maskImage[max(0, processingPoint[0] - textureSize) : processingPoint[0] + textureSize + 1, max(0, processingPoint[1] - textureSize) : processingPoint[1] + textureSize + 1] = 0
	semaphoreSavePatch.release() # Liberar semáfaro para outras Threads.
	return True # Inpainting realizado com sucesso.






# == MAIN ==

# Ler entradas.
originalImageName = input("Entre com o nome da imagem original = ").rstrip()
maskImageName = input("Entre com a imagem máscara = ").rstrip()
textureSize = int(input("Entre com o tamanho máximo de uma textura que será remendada (> 0) = "))
analysisWindowSize = int(input("Entre com o tamanho da janela de análise (> 1) = "))
outputImageName = input("Entre com o nome da imagem final que será salva = ").rstrip()
print()

# Ler imagens do disco.
originalImage = imageio.imread(originalImageName)
maskImage = imageio.imread(maskImageName)

# Validar entradas.
if textureSize < 1:
	print("Por favor, insira um tamanho máximo de textura maior que zero! Recomendado: algo entre 3-15.")
	sys.exit(-1)
elif analysisWindowSize < 2:
	print("Por favor, entre com um tamanho de janela maior que um! Recomendado: algo entre 3-5.")
	sys.exit(-1)
elif originalImage.shape != maskImage.shape:
	print("Suas imagens não tem tamanhos iguais, ou uma é colorida e a outra não. Entre com os arquivos corretos, por favor!")
	print("Dimensões: %s e %s" % (originalImage.shape, maskImage.shape))
	sys.exit(-1)

# Executar algoritmo do Inpainting.
print("\rCalculando janelas...", end='')
patches = computePatchPriorities(maskImage, textureSize)
while len(patches) > 0:
	processingThreads = [ ]
	print("\rProcessando %d janelas..." % len(patches), end='')
	for i in patches: # Criar Threads para os patches.
		processingThreads.append(Thread(target=processPatch, args=(originalImage, maskImage, textureSize, analysisWindowSize * 2, i)))
	for thread in processingThreads: # Executar Threads.
		thread.start()
	counter = 0
	for thread in processingThreads: # Aguardar execução das Threads, reportando andamento para o usuário.
		thread.join()
		counter += 1
		print("\rProcessando %d janelas: %3.2f%%" % (len(patches), 100 * counter / len(patches)), end='', flush=True)
	print("\rProcessando %d janelas: %3.2f%%. Pronto." % (len(patches), 100))
	print("\rCalculando janelas...", end='')
	patches = computePatchPriorities(maskImage, textureSize) # Obter novos patches, se houverem.

# Imagem final gerada a partir da imagem original.
outputImage = originalImage

# Salvar imagem final.
print("Salvando em disco...")
imageio.imwrite(outputImageName, outputImage)

print("Processamento concluído.")


