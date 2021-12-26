import numpy as np
#import imageio
import random
import math
import sys




#
 #	~ Trabalho 1: Gerador de Imagens ~
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


def NPMax(Arr):
	"Esta função calcula o máximo valor de uma matriz numpy."
	Max = sys.float_info.min # Começa com FLOAT_MIN
	N = len(Arr)
	for x in range(0, N):
		for y in range(0, N): # Percorrer matriz achando maior valor
			if Arr[x, y] > Max:
				Max = Arr[x, y]
	return Max

def NPMin(Arr):
	"Esta função calcula o mínimo valor de uma matriz numpy."
	Min = sys.float_info.max # Começa com FLOAT_MAX
	N = len(Arr)
	for x in range(0, N):
		for y in range(0, N): # Percorrer matriz achando menor valor
			if Arr[x, y] < Min:
				Min = Arr[x, y]
	return Min

def NPMaxIn(Arr, i, j, d):
	"Esta função calcula o máximo valor dentro de uma submatriz de uma matriz numpy maior."
	#if d <= 1: # Caso em que não há submatriz
		#return Arr[i, j]
	Max = sys.float_info.min # Começa com FLOAT_MIN
	xstart = i*d
	xend = xstart + d
	ystart = j*d
	yend = ystart + d
	for x in range(xstart, xend):
		for y in range(ystart, yend): # Percorrer submatriz da matriz principal 'Arr', encontrando maior valor
			if Arr[x, y] > Max:
				Max = Arr[x, y]
	return Max


def Func1(C):
	"f(x, y) = x + y"
	Arr = np.empty([C, C], dtype=np.float) # Alocar matriz
	for x in range(0, C):
		for y in range(0, C):
			Arr[x, y] = np.float(x + y)
	return Arr

def Func2(C, Q):
	"f(x, y) = | sin(x/Q) + sin(y/Q) |"
	Arr = np.empty([C, C], dtype=np.float) # Alocar matriz
	for x in range(0, C):
		for y in range(0, C):
			Arr[x, y] = np.float(math.fabs(math.sin(x/Q) + math.sin(y/Q)))
	return Arr

def Func3(C, Q):
	"f(x, y) = [ x/Q - sqrt(y/Q) ]"
	Arr = np.empty([C, C], dtype=np.float) # Alocar matriz
	Q = np.float(Q)
	for x in range(0, C):
		for y in range(0, C):
			Arr[x, y] = np.float((x/Q) - math.sqrt(y/Q))
	return Arr

def Func4(C, S):
	"f(x, y) = random [0.0, 1.0]"
	Arr = np.empty([C, C], dtype=np.float) # Alocar matriz
	random.seed(S)
	for x in range(0, C):
		for y in range(0, C):
			Arr[x, y] = np.float(random.uniform(0, 1))
	return Arr

def Func5(C, S):
	"f(x, y) = random_walk"
	Arr = np.zeros([C, C], dtype=np.float) # Alocar matriz zerada
	counter = int(1 + (C*C)/2) # Contador de passos
	random.seed(S)
	x = y = 0
	Arr[x, y] = np.float(1)
	while counter >= 0:
		x = (x + random.randrange(-1, 2)) % C # Soma +1 ou -1 a x
		Arr[x, y] = np.float(1)
		y = (y + random.randrange(-1, 2)) % C # Soma +1 ou -1 a y
		Arr[x, y] = np.float(1)
		counter = counter - 1
	return Arr




# == MAIN ==


# Leitura dos dados da entrada padrão
file_name = str(input()).rstrip()
C = int(input())
f = int(input())
Q = int(input())
N = int(input())
B = int(input())
S = int(input())

# Variável 'd' de relação entre os tamanhos da Imagem Cena e da Imagem Digital
d = int(C/N)

# Processar funções e gerar matriz Cena 'Shot'
if f == 1:
	Shot = Func1(C) # f(x,y) = x + y
elif f == 2:
	Shot = Func2(C, Q) # f(x, y) = | sin(x/Q) + sin(y/Q) |
elif f == 3:
	Shot = Func3(C, Q) # f(x, y) = [ (x/Q) - sqrt(y/Q) ]
elif f == 4:
	Shot = Func4(C, S) # f(x, y) = random [0.0, 1.0]
else:
	Shot = Func5(C, S) # f(x, y) = random_walk

# Achar valores máximo e mínimo presentes na matriz
Min = NPMin(Shot)
Max = NPMax(Shot)

# Normalizar valores entre 0 e 65535
Razao = np.float(65535 / (Max - Min))
for x in range(0, C):
	for y in range(0, C):
		Shot[x, y] = np.float((Shot[x, y] - Min) * Razao)

# Gerar imagem digital g 'Digital'
Digital = np.empty([N, N], dtype=np.float)
for x in range(0, N):
	for y in range(0, N):
		Digital[x, y] = NPMaxIn(Shot, x, y, d)

# Normalizar valores entre 0 e 255
Min = NPMin(Digital)
Max = NPMax(Digital)
Ratio = np.float(255 / (Max - Min))
for x in range(0, N):
	for y in range(0, N):
		Digital[x, y] = np.float((Digital[x, y] - Min) * Ratio)

# Converter de float para uint8
Digital = Digital.astype(dtype=np.uint8)

# Considerar apenas os primeiros B bits menos significativos
NBits = 8 - B
for x in range(0, N):
	for y in range(0, N):
		Digital[x, y] = np.uint8(Digital[x, y] >> NBits)

# Carregar imagem do disco e converter para tipo uint8
LoadedImg = np.load(file_name)
LoadedImg = LoadedImg.astype(np.uint8)

# Calcular RMSE
RMSE = float(0)
for x in range(0, N):
	for y in range(0, N):
		RMSE += math.pow(float(Digital[x, y]) - float(LoadedImg[x, y]), 2)
RMSE = math.sqrt(RMSE / (N*N))

# Imprimir resultado do RMSE com 4 casas decimais
print("%.4f" % round(RMSE, 4))






# == DEBUGGING ==
# Imprimir minha imagem gerada e imagem lida do disco para comparação
#imageio.imwrite("c/out_expected.png", LoadedImg)
#imageio.imwrite("c/out_generated.png", Digital)
#print("")
#print(Digital)
#print("")
#print(LoadedImg)
#print("")


