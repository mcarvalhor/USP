import numpy as np
import imageio
import math




#
 #	~ Trabalho 2: Realce e Superresolução ~
 #
 #	Matheus Carvalho Raimundo
 #	Nº USP: 10369014
 #	mcarvalhor@usp.br
 #	Processamento de Imagens: SCC-0251 2018.1
 #
 #	 _______ _______ _______
 #	|   |   |               \
 #	|   |   |      \    |___|
 #	|   |    \      |   |
 #	|_______ _______|___|
 #
#





# == DEFINIÇÃO DE FUNÇÕES ==

def Histogram(Arr):
	"Calcula o histograma acumulado da imagem 'Arr'"
	Sum = np.zeros(256, dtype=np.float) # Alocar matriz do histograma zerada
	for x in range(0, Arr.shape[0]): # Somar valores
		for y in range(0, Arr.shape[1]):
			Sum[int(Arr[x, y])] += 1
	for i in range(1, 256): # Fazer cumulativo
		Sum[i] += Sum[i - 1]
	return Sum

def HistogramList(ArrList):
	"Calcula o histograma acumulado das imagens em 'ArrList'"
	Sum = np.zeros(256, dtype=np.float) # Alocar matriz do histograma zerada
	for i in range(0, len(ArrList)): # Para cada imagem na lista...
		for x in range(0, ArrList[i].shape[0]): # Somar valores
			for y in range(0, ArrList[i].shape[1]):
				Sum[int(ArrList[i][x, y])] += 1
	for i in range(1, 256): # Fazer cumulativo
		Sum[i] += Sum[i - 1]
	return Sum

def Equalise(Arr, Histogram, Count):
	"Realiza a equalização do histograma 'Histogram' (feito a partir de 'Count' imagens) na imagem 'Arr', retornando uma imagem resultado."
	NewArr = np.empty(Arr.shape, dtype=np.float) # Aloca a imagem resultado
	Ratio = float(255/(NewArr.shape[0] * NewArr.shape[1] * Count))
	for x in range(0, NewArr.shape[0]): # Equalizar
		for y in range(0, NewArr.shape[1]):
			NewArr[x, y] = np.float(Ratio * Histogram[int(Arr[x, y])])
	return NewArr

def Gamma(Arr, Param):
	"Aplica a correção de Gamma na imagem 'Arr' de acordo com o parâmetro 'Param', retornando uma imagem resultado"
	NewArr = np.empty(Arr.shape, dtype=np.float) # Aloca a imagem resultado
	Power = float(1/Param)
	for x in range(0, NewArr.shape[0]): # Aplicar correção de Gamma
		for y in range(0, NewArr.shape[1]):
			NewArr[x, y] = np.float(math.floor(255 * pow(Arr[x, y]/255.0, Power)))
	return NewArr



# == MAIN ==

# Leitura dos dados da entrada padrão
imglow = str(input()).rstrip()
imghigh = str(input()).rstrip()
method = int(input())
param = float(input())

# Carregamento das imagens
Images = [	imageio.imread(imglow + "1.png"),
			imageio.imread(imglow + "2.png"),
			imageio.imread(imglow + "3.png"),
			imageio.imread(imglow + "4.png")]

# Processar realce
if method == 1:
	for i in range(0, 4):
		HistogramSum = Histogram(Images[i])
		Images[i] = Equalise(Images[i], HistogramSum, 1)
elif method == 2:
	HistogramSum = HistogramList(Images)
	for i in range(0, 4):
		Images[i] = Equalise(Images[i], HistogramSum, 4)
elif method == 3:
	for i in range(0, 4):
		Images[i] = Gamma(Images[i], param)

# Alocar imagem de alta resolução
HighResImage = np.empty([Images[0].shape[0]*2, Images[0].shape[1]*2], dtype=np.float)

# Gerar imagem de alta resolução
for x in range(0, Images[0].shape[0]):
	for y in range(0, Images[0].shape[1]): # Carregar a matriz 2x2 a partir das 4 imagens originais
		HighResImage[x*2, y*2] = np.float(Images[0][x, y])
		HighResImage[x*2, y*2 + 1] = np.float(Images[1][x, y])
		HighResImage[x*2 + 1, y*2] = np.float(Images[2][x, y])
		HighResImage[x*2 + 1, y*2 + 1] = np.float(Images[3][x, y])

# Agora não precisamos mais das imagens de menor resolução. Permitir que o Garbage Collector libere elas
del Images

# Converter para o tipo uint8
HighResImage = HighResImage.astype(np.uint8)

# Carregar imagem original do disco e converter para tipo uint8
LoadedImg = imageio.imread(imghigh + ".png")
LoadedImg = LoadedImg.astype(np.uint8)

# Calcular RMSE
RMSE = float(0)
for x in range(0, LoadedImg.shape[0]):
	for y in range(0, LoadedImg.shape[1]):
		RMSE += math.pow(float(HighResImage[x, y]) - float(LoadedImg[x, y]), 2)
RMSE = math.sqrt(RMSE / (LoadedImg.shape[0]*LoadedImg.shape[1]))

# Imprimir resultado do RMSE com 4 casas decimais
print("%.4f" % round(RMSE, 4))






# == DEBUGGING ==
# Salvar minhas imagens geradas e imagem lida do disco para comparação
#imageio.imwrite("out_expected.png", LoadedImg)
#imageio.imwrite("out_generated0.png", HighResImage)
#imageio.imwrite("out_generated1.png", Images[0].astype(dtype=np.uint8))
#imageio.imwrite("out_generated2.png", Images[1].astype(dtype=np.uint8))
#imageio.imwrite("out_generated3.png", Images[2].astype(dtype=np.uint8))
#imageio.imwrite("out_generated4.png", Images[3].astype(dtype=np.uint8))


