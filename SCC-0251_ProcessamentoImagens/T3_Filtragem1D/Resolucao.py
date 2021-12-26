import numpy as np
import imageio
import math




#
 #	~ Trabalho 3: Filtragem 1D ~
 #
 #	Matheus Carvalho Raimundo
 #	mcarvalhor@usp.br
 #	Nº USP 10369014
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


def Apply_Filter_Space(Arr, Mask):
	"Esta função retorna uma imagem resultante da aplicação do filtro de máscara Mask sob a imagem Arr no domínio do espaço."
	Res = np.zeros(Arr.shape, dtype=np.float)
	mask_start = int(((Mask.shape[0] - 1) / 2) * -1)
	for i in range(0, Arr.shape[0]): # Para cada pixel da array resultante...
		j = mask_start
		for u in Mask: # Fazer a soma dos pesos do filtro
			Res[i] += Arr[(i - j) % Res.shape[0]] * u
			j = j + 1
	Res = np.floor(Res)
	return Res

def Apply_Filter_Frequency(Arr, Mask):
	"Esta função retorna um numpy array resultante da aplicação do filtro de máscara Mask sob o numpy array Arr no domínio das frequências."
	Res = np.zeros(Arr.shape, dtype=np.float)
	for i in range(0, Mask.shape[0]): # Copiar filtro para o começo do array
		Res[i] = Mask[i]
	Arr = DFT(Arr) # Transformada de Fourier
	Res = DFT(Res) # Filtro estendido
	Res = np.multiply(Arr, Res) # Multiplicar ponto a ponto
	Res = np.real(DFT_Inv(Res)) # Fazer a transformada de Fourier inversa e obter apenas a parte real
	return Res

def New_Gaussian_Mask(N, Sigma):
	"Esta função retorna um filtro Gaussiano gerado a partir de um tamanho N e um desvio padrão Sigma."
	Res = np.empty(N, dtype=np.float)
	Ratio1 = math.sqrt(2 * math.pi) * Sigma
	Ratio2 = 2 * Sigma * Sigma
	mask_center = int((N / 2) * -1)
	mask_half = int(N / 2)
	j = int((N / 2) * -1)
	for i in range(0, mask_half): # Gerar valores pré-centro
		Res[i] = j
		j = j + 1
	if N % 2 != 0:
		Res[mask_half] = 0 # Gerar centro
	j = 1
	for i in range(mask_center + 1, N): # Gerar valores pós-centro
		Res[i] = j
		j = j + 1
	Res = np.abs(Res) / np.sum(np.abs(Res)) # Normalizar pesos
	for i in range(0, N): # Aplicar equação de Gauss
		Res[i] = math.exp((-1 * Res[i] * Res[i]) / Ratio2) / Ratio1
	Sum = 1 / np.sum(Res)
	for i in range(0, N): # Normalizar entre 0 - 1.
		Res[i] = Res[i] * Sum
	return Res

def DFT(Arr):
	"Esta função retorna o domínio das frequências da imagem Arr, fazendo sua transformada de Fourier."
	Res = np.empty(Arr.shape, dtype=np.complex64)
	N = Arr.shape[0]
	x = np.arange(0, N) # Fixo
	for u in range(0, N): # Para cada frequência...
		Res[u] = np.sum(Arr * np.exp((-1j * 2 * np.pi * x * u) / N)) # Calcular o valor resultado da equação
	return Res

def DFT_Inv(Arr):
	"Esta função retorna o domínio de espaço da imagem Arr, fazendo sua transformada de Fourier inversa."
	Res = np.empty(Arr.shape, dtype=np.complex64)
	N = Arr.shape[0]
	x = np.arange(0, N) # Fixo
	for u in range(0, N): # Para cada frequência...
		Res[u] = np.sum(Arr * np.exp((1j * 2 * np.pi * x * u) / N)) # Calcular o valor resultado da equação
	Res = np.real(Res / N) # Valores reais
	return Res



# == MAIN ==


# Leitura dos dados da entrada padrão
file_name = str(input()).rstrip()
filter_type = int(input())
filter_n = int(input())
filter_definition = input()
filter_domain = int(input())

# Carregar imagem
LoadedImage = imageio.imread(file_name)
GeneratedImage = LoadedImage.flatten() # Tornar 1D

# Gerar filtros
if filter_type == 1: # Filtro Arbitrário
	filter_mask = np.array(list(map(np.float, filter_definition.split(' ')))) # Processar pesos e colocar em um Numpy Array
	filter_mask = np.abs(filter_mask) / np.sum(np.abs(filter_mask)) # Normalizar pesos
else:
	sigma = float(filter_definition)
	filter_mask = New_Gaussian_Mask(filter_n, sigma) # Criar filtro Gaussiano

# Aplicar filtros no domínio escolhido
if filter_domain == 1:
	GeneratedImage = Apply_Filter_Space(GeneratedImage, filter_mask)
else:
	GeneratedImage = Apply_Filter_Frequency(GeneratedImage, filter_mask)

# Voltar a imagem para 2D e em formato uint8
GeneratedImage = np.uint8(np.reshape(GeneratedImage, LoadedImage.shape))

# Calcular RMSE
RMSE = float(0)
for x in range(0, LoadedImage.shape[0]):
	for y in range(0, LoadedImage.shape[1]):
		RMSE += math.pow(float(GeneratedImage[x, y]) - float(LoadedImage[x, y]), 2)
RMSE = math.sqrt(RMSE / (LoadedImage.shape[0] * LoadedImage.shape[1]))

# Imprimir resultado do RMSE com 4 casas decimais
print("%.4f" % round(RMSE, 4))






# == DEBUGGING ==
# Imprimir minha imagem gerada e imagem lida do disco para comparação
imageio.imwrite("out_expected.png", LoadedImage)
imageio.imwrite("out_generated.png", GeneratedImage)
#print("")
#print(GeneratedImage)
#print("")
#print(LoadedImage)
#print("")


