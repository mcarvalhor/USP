import numpy as np
import imageio
import sys




#
 #	~ Trabalho 4: Filtragem 2D ~
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
	"Aplica um filtro Mask sob uma imagem Arr no domínio do espaço."
	Res = np.zeros(Arr.shape, dtype=np.float)
	mask_start = [
		int(((Mask.shape[0] - 1) / 2) * -1),
		int(((Mask.shape[1] - 1) / 2) * -1) ]
	for i in range(Arr.shape[0]):
		for j in range(Arr.shape[1]): # Para cada pixel da array resultante...
			for x in range(Mask.shape[0]):
				for y in range(Mask.shape[1]): # Para cada peso da máscara...
					ui = i - (x + mask_start[0])
					uj = j - (y + mask_start[1])
					if ui < 0 or ui >= Arr.shape[0] or uj < 0 or uj >= Arr.shape[1]: # Se for sob uma posição válida...
						continue
					Res[i, j] += Arr[ui, uj] * Mask[x, y] # Somar peso
	Res = np.floor(Res)
	return Res

def Apply_Filter_Frequency(Arr, Mask):
	"Aplica um filtro Mask sob uma imagem Arr no domínio das frequências."
	Res = np.zeros(Arr.shape, dtype=np.float)
	Res[: Mask.shape[0], : Mask.shape[1]] += Mask # Filtro do tamanho da imagem
	Arr = np.fft.fft2(Arr) # Transformada de Fourier
	Res = np.fft.fft2(Res) # Filtro estendido
	Res = np.multiply(Arr, Res) # Multiplicar ponto a ponto
	return Res

def New_Laplace_Filter(N, Sigma):
	"Cria um filtro Laplaciana da Gaussiana de tamanho N e desvio padrão Sigma."
	Res = np.empty([N, N], dtype=np.float)
	xyinc = 10 / (N - 1)
	Ratio1 = np.pi * (Sigma ** 4)
	Ratio2 = 2 * (Sigma * Sigma)
	x = -5
	for i in range(N): # Para cada linha do filtro...
		y = -5
		for j in range(N): # Para cada coluna do filtro..
			Res[i, j] = (1 - ((x * x + y * y) / Ratio2)) * np.exp(-1 * ((x * x + y * y) / Ratio2)) # Aplicar equação LoG
			y += xyinc
		x += xyinc
	Res = -Res / Ratio1
	Ratio3 = -1 * np.sum(np.where(Res > 0, Res, 0)) / np.sum(np.where(Res < 0, Res, 0)) # Criar razão para normalizar
	Res = (np.where(Res < 0, Res, 0) * Ratio3) + np.where(Res > 0, Res, 0) # Normalizar
	return Res



# == MAIN ==


# Leitura dos dados da entrada padrão
file_name = str(input()).rstrip()
filter_option = int(input())


loaded_image = imageio.imread(file_name) # Carregar imagem


if filter_option == 1: # Filtro Arbitrário
	filter_size = np.array(input().split(' '), dtype=np.int) # Ler dimensões do filtro
	filter_mask = np.empty(filter_size, dtype=np.float) # Alocar filtro arbitrário
	for i in range(filter_size[0]):
		filter_mask[i] = np.array(input().split(' '), dtype=np.float) # Ler pesos
	generated_image = Apply_Filter_Frequency(loaded_image, filter_mask) # Aplicar filtro
elif filter_option == 2: # Filtro Laplaciana da Gaussiana
	filter_size = int(input())
	filter_sigma = float(input())
	filter_mask = New_Laplace_Filter(filter_size, filter_sigma) # Criar filtro
	generated_image = Apply_Filter_Frequency(loaded_image, filter_mask) # Aplicar filtro
else:
	filter_mask_x = np.array([ # Filtro Ix
		[1, 0, -1],
		[2, 0, -2],
		[1, 0, -1]
	], dtype=np.float)
	filter_mask_y = np.array([ # Filtro Iy
		[1, 2, 1],
		[0, 0, 0],
		[-1, -2, -1]
	], dtype=np.float)
	generated_image_x = Apply_Filter_Space(loaded_image, filter_mask_x) # Aplicar filtro em Ix
	generated_image_y = Apply_Filter_Space(loaded_image, filter_mask_y) # Aplicar filtro em Iy
	generated_image = np.hypot(generated_image_x, generated_image_y) # Gerar Iout a partir da equação
	generated_image = np.fft.fft2(generated_image) # Transformada de Fourier
	del generated_image_x # Permitir que Garbage Collector libere da memória
	del generated_image_y


positions = np.array(input().split(' '), dtype=np.float) # Posições de corte
generated_image_1 = generated_image[0 : generated_image.shape[0] // 2, 0 : generated_image.shape[1] // 2]
positions[0:2] *= generated_image_1.shape[0]
positions[2:4] *= generated_image_1.shape[1]
positions = np.array(positions, dtype=np.int)
generated_image_2 = generated_image_1[positions[0] : positions[1], positions[2] : positions[3]]

del generated_image # Permitir limpeza do GC nestas matrizes
del generated_image_1


dataset = np.load(input().rstrip()) # Carregar vetores para comparação
dataset_labels = np.load(input().rstrip()) # Carregar labels



# == Algorítmo K-NN de Machine Learning ==

generated_image_2 = generated_image_2.flatten() # Transformar para vetor 1D
nearest_distance = sys.maxsize # INT_MAX
nearest_index = -1

for i in range(dataset.shape[0]): # Para cada vetor...
	distance = np.sqrt(np.sum(np.power(np.abs(generated_image_2 - dataset[i]), 2))) # Calcular distância euclidiana
	if distance < nearest_distance: # e verificar se é a menor distância
		nearest_distance = distance
		nearest_index = i

print(dataset_labels[nearest_index]) # Imprimir label
print(nearest_index) # Impimir índice




# == DEBUGGING ==
# Nesse trabalho não tem como fazer um "debugging".


