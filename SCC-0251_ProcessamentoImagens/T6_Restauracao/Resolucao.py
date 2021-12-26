import numpy as np
import imageio




#
 #	~ Trabalho 6: Restauração de imagens ~
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


def Res_AdaptiveFilter(Arr, N, sigmaNoisy):
	"Esta função retorna a restauração por filtro adaptativo de redução de ruído local da imagem Arr usando um tamanho N e uma distribuição de ruído sigmaNoisy."
	Temp = np.lib.pad(Arr.astype(np.float), N // 2, 'wrap') # Imagem estendida temporária
	sigmaN = np.empty(Arr.shape, dtype=np.float) # Variância
	mN = np.empty(Arr.shape, dtype=np.float) # Média aritmética
	Res = np.copy(Arr).astype(np.float) # Iout
	sigmaNoisy = sigmaNoisy ** 2
	for x in range(Arr.shape[0]):
		for y in range(Arr.shape[1]): # Obter médias e variâncias
			mN[x, y] = np.average(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
			sigmaN[x, y] = np.var(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
	sigmaN = np.where(sigmaN == 0, 1, sigmaN) # Evitar divisão por zero
	Res = np.multiply((sigmaNoisy / sigmaN), Arr) + np.multiply(1 - (sigmaNoisy / sigmaN), mN) # Aplicar equação
	return Res

def Res_Median(Arr, N, M):
	"Esta função retorna a restauração por filtro adaptativo de mediana da imagem Arr usando um tamanho N e um tamanho máximo M."
	Temp = np.lib.pad(Arr.astype(np.float), N // 2, 'symmetric') # Imagem estendida temporária
	Res = np.empty(Arr.shape, dtype=np.float) # Iout
	for x in range(Arr.shape[0]):
		for y in range(Arr.shape[1]): # Algoritmo
			zMin = np.min(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
			zMax = np.max(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
			zMed = np.median(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
			A1 = zMed - zMin
			A2 = zMed - zMax
			while A1 <= 0 or A2 >= 0:
				N += 1
				Temp = np.lib.pad(Arr.astype(np.float), N // 2, 'symmetric')
				zMin = np.min(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
				zMax = np.max(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
				zMed = np.median(Temp[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
				A1 = zMed - zMin
				A2 = zMed - zMax
				if N > M:
					Res[x, y] = zMed
					break
			B1 = Arr[x, y] - zMin
			B2 = zMed - zMax
			if B1 <= 0 or B2 >= 0 or Arr[x, y] == zMax:
				Res[x, y] = zMed
	return Res

def Res_ContraharmonicMean(Arr, N, Q):
	"Esta função retorna a restauração por filtro da média contra-harmônica da imagem Arr usando um tamanho N e um parâmetro Q."
	Temp = np.lib.pad(Arr.astype(np.float), N // 2, 'constant', constant_values=0) # Imagem estendida temporária
	Res = np.empty(Arr.shape, dtype=np.float) # Iout
	Temp1 = np.power(Temp, Q + 1) # Elevar tudo a Q + 1
	Temp2 = np.power(np.where(Temp == 0, 1, Temp), Q) # Elevar tudo a Q, eliminando chances de divisão por zero
	for x in range(Arr.shape[0]):
		for y in range(Arr.shape[1]): # Para cada pixel da imagem resultante...
			Res[x, y] = np.sum(Temp1[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N]) # Fazer somatórios e dividir
			Res[x, y] /= np.sum(Temp2[N // 2 + x:N // 2 + x + N, N // 2 + y:N // 2 + y + N])
	return Res




# == MAIN ==


# Leitura dos dados da entrada padrão
filename_Icomp = str(input()).rstrip()
filename_Inoisy = str(input()).rstrip()
filter_method = int(input())
filter_size = int(input())
filter_param = input()

# Carregamento das imagens
Icomp = imageio.imread(filename_Icomp) # Carregar imagem para comparação
Inoisy = imageio.imread(filename_Inoisy) # Carregar imagem que precisa ser restaurada

# Executar o algorítmo específico
if filter_method == 1:
	Inoisy = Res_AdaptiveFilter(Inoisy, filter_size, np.float(filter_param))
elif filter_method == 2:
	Inoisy = Res_Median(Inoisy, filter_size, int(filter_param))
else:
	Inoisy = Res_ContraharmonicMean(Inoisy, filter_size, np.float(filter_param))

# Converter para o tipo inteiro de 8 bits
Inoisy = np.uint8(Inoisy)

# Calcular RMSE
RMSE = np.sqrt(np.sum(np.power(Icomp - Inoisy, 2)) / Icomp.size)

# Imprimir resultado do RMSE com 4 casas decimais
print("%.4f" % round(RMSE, 4))




# == DEBUGGING ==
#imageio.imwrite("out_expected.png", Icomp)
#imageio.imwrite("out_generated.png", Inoisy)


