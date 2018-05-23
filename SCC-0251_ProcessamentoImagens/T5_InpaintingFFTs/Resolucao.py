import numpy as np
import imageio




#
 #	~ Trabalho 5: Inpainting usando FFTs ~
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


def Normalize(Arr):
	"Esta função normaliza a imagem Arr entre 0-255."
	Res = np.copy(Arr) # Criar cópia da array que será normalizada
	Res -= np.min(Res) # Alinhar em 0
	Res *= 255 / np.max(Res) # Agora o máximo é 255
	return Res

def Apply_Filter_Frequency(Arr, Mask):
	"Aplica um filtro Mask sob uma imagem Arr no domínio das frequências."
	Res = np.zeros(Arr.shape, dtype=np.float)
	Res[: Mask.shape[0], : Mask.shape[1]] += Mask # Filtro do tamanho da imagem
	Res = np.fft.fft2(Res) # FFT do filtro estendido
	Res = np.multiply(Arr, Res) # Multiplicar ponto a ponto
	return Res

def Apply_GerchbergPapoulis(Arr, Mask, T):
	"Aplica o algoritmo Gerchberg-Papoulis na imagem Arr usando uma máscara Mask e T interações."
	Convolution_Mask = np.ones([7, 7], dtype=np.float) / (7 * 7) # Criar filtro de média
	M = np.fft.fft2(Mask) # Máscara no domínio das frequências
	Mask = Mask.astype(np.float) / 255 # É esperado que seja 0 ou 1
	Last = Arr # Começa com a imagem original
	for _ in range(T): # T interações
		G = np.fft.fft2(Last) # Faz a transformada
		G = np.where(M < 0.01 * np.max(G), G, 0) # Filtro "passa-alta" (ii)
		G = np.where(M < 0.9 * np.max(M), G, 0) # Filtro "passa-baixa" (i)
		G = Apply_Filter_Frequency(G, Convolution_Mask) # Aplicar o filtro de média 7x7
		G = np.real(np.fft.ifft2(G)) # Transformada inversa
		G = Normalize(G) # [0 - 255]
		G = np.multiply(1 - Mask, Arr) + np.multiply(Mask, G) # Estimativa k
		Last = G
	return Last



# == MAIN ==


# Leitura dos dados da entrada padrão
filename_i = str(input()).rstrip()
filename_g = str(input()).rstrip()
filename_m = str(input()).rstrip()
T = int(input())

# Carregamento das imagens
i = imageio.imread(filename_i) # Carregar imagem original
g = imageio.imread(filename_g) # Carregar imagem que precisa ser restaurada
m = imageio.imread(filename_m) # Carregar máscara para restauração

# Executar o algorítmo
g = Apply_GerchbergPapoulis(g, m, T)

# Converter para o tipo inteiro de 8 bits
g = np.uint8(g)

# Calcular RMSE
RMSE = np.sqrt(np.sum(np.power(i - g, 2)) / i.size)

# Imprimir resultado do RMSE com 5 casas decimais
print("%.5f" % round(RMSE, 5))




# == DEBUGGING ==
#imageio.imwrite("out_expected.png", i)
#imageio.imwrite("out_generated.png", g)


