import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy as np
import random as rand
import math



#
 #	~ Projeto 1: Mola 2D ~
 #
 #	Lucas Veiga Betoni (Nº USP 9875952)
 #	Matheus Carvalho Raimundo (Nº USP 10369014)
 #
 #	Computação Gráfica: SCC-0250 2020.1
 #
 #	 _______ _______ _______
 #	|   |   |               \
 #	|   |   |      \    |___|
 #	|   |    \      |   |
 #	|_______ _______|___|
 #
#



# Inicializar janela

glfw.init()
glfw.window_hint(glfw.VISIBLE, glfw.FALSE);
window = glfw.create_window(800, 800, "Trabalho Pratico 1", None, None)
glfw.make_context_current(window)


# Programa Vertex Shader

vertex_code = """
        attribute vec2 position;
        uniform mat4 mat;
        void main(){
            gl_Position = mat * vec4(position, 0.0, 1.0);
        }
"""


# Programa Fragment Shader

fragment_code = """
        void main(){
            gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
"""


# Requisitar slotes do Fragment Shader e Vertex Shader para a GPU

program  = glCreateProgram()
vertex   = glCreateShader(GL_VERTEX_SHADER)
fragment = glCreateShader(GL_FRAGMENT_SHADER)

# Associar código-fonte aos slotes requisitados

glShaderSource(vertex, vertex_code)
glShaderSource(fragment, fragment_code)


# Compilar Vertex Shader

glCompileShader(vertex)
if not glGetShaderiv(vertex, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(vertex).decode()
    print(error)
    raise RuntimeError("Erro de compilação do Vertex Shader!")


# Compilar Fragment Shader

glCompileShader(fragment)
if not glGetShaderiv(fragment, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(fragment).decode()
    print(error)
    raise RuntimeError("Erro de compilação do Fragment Shader!")


# Associar programas compilados ao programa principal

glAttachShader(program, vertex)
glAttachShader(program, fragment)


# Linkagem dos programas

glLinkProgram(program)
if not glGetProgramiv(program, GL_LINK_STATUS):
    print(glGetProgramInfoLog(program))
    raise RuntimeError("Erro de linkagem dos programas!")

glUseProgram(program)



# == MODELANDO A MOLA ==


# Variáveis editáveis da mola

num_vertices = 200 # Define a "qualidade" dos circulo da mola
radius = 0.1 # Raio da mola
loop = 5 # Número de círculos na mola
y_scale = 0.02 # Escala na variável y


vertices = np.zeros(num_vertices, [("position", np.float32, 2)])
pi = math.pi
counter = 0
angle = 0.0

for counter in range(num_vertices):
    angle += loop*2*pi/num_vertices
    x = math.sin(angle)*radius
    y = angle*y_scale/loop
    vertices[counter]['position'] = [x,y]


# Requisitar slot para enviar dados para GPU

buffer = glGenBuffers(1)
glBindBuffer(GL_ARRAY_BUFFER, buffer)


# Enviar pontos gerados da mola

glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_DYNAMIC_DRAW)
glBindBuffer(GL_ARRAY_BUFFER, buffer)


# Associar variáveis do GLSL (Vertex Shaders) com os nossos dados enviados

stride = vertices.strides[0]
offset = ctypes.c_void_p(0)

loc = glGetAttribLocation(program, "position")
glEnableVertexAttribArray(loc)

glVertexAttribPointer(loc, 2, GL_FLOAT, False, stride, offset)



# = CAPTURAR ENTRADAS DO TECLADO =


space_pressed = False

def key_event(window,key,scancode,action,mods):
    global space_pressed

    if key == 32 and action == 1: # Apertou a tecla de espaço.
        space_pressed = True
    elif key == 32 and action == 0: # Soltou a tecla de espaço.
        space_pressed = False

glfw.set_key_callback(window,key_event)


# Exibir janela

glfw.show_window(window)



# == DDEFINIÇÃO DE FUNÇÕES IMPORTANTES ==


# Essa função multiplica duas matrizes (a e b), retornando o resultado da multiplicação.

def multiplica_matriz(a,b):
    m_a = a.reshape(4,4)
    m_b = b.reshape(4,4)
    m_c = np.dot(m_a,m_b)
    c = m_c.reshape(1,16)
    return c


# Essa função desenha na tela a mola com as devidas transformações.

def desenhar(translation_x = 0.0, translation_y = 0.0, scale_x = 1.0, scale_y = 1.0, rotation = 0.0):
    global glfw, vertices, y_start
    sin_rotation = math.sin(rotation)
    cos_rotation = math.cos(rotation)

    translation_y += y_start

    glfw.poll_events()
    glClear(GL_COLOR_BUFFER_BIT)
    glClearColor(1.0, 1.0, 1.0, 1.0)

    mat_scale = np.array([     scale_x, 0.0, 0.0, 0.0,
                                0.0, scale_y, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 1.0], np.float32)

    mat_rotation = np.array([  cos_rotation, -sin_rotation, 0.0, 0.0,
                               sin_rotation, cos_rotation, 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0,
                               0.0, 0.0, 0.0, 1.0], np.float32)

    mat_translation = np.array([  1.0, 0.0, 0.0, translation_x,
                                  0.0, 1.0, 0.0, translation_y,
                                  0.0, 0.0, 1.0, 0.0,
                                  0.0, 0.0, 0.0, 1.0], np.float32)

    mat_transform = multiplica_matriz(mat_scale, mat_rotation)
    mat_transform = multiplica_matriz(mat_translation, mat_transform)

    loc = glGetUniformLocation(program, "mat")
    glUniformMatrix4fv(loc, 1, GL_TRUE, mat_transform)

    glDrawArrays(GL_LINE_STRIP, 0, len(vertices))

    glfw.swap_buffers(window)



# == CONSTANTES DE ANIMAÇÃO DA MOLA ==


compress_rate = 0.005 # Velocidade de compressão da mola quando [ESPAÇO] é pressionado.
compress_min = 0.1 # Compressão mínima permitida na mola - se não chegar nisso, ela não pula.
compress_max = 0.75 # Compressão máxima permitida na mola - se chegar nisso, ela para o mais longe possível.

bounce_random_probability = 0.5 # 0.0 = só pula pra esquerda, 1.0 = só pula pra direita, 0.5 = random com chances iguais

bounce_rate = 0.005 # Velocidade de pulo da mola.
bounce_min_x_distance = 0.2 # Distância mínima (eixo x) que a mola para após o pulo.
bounce_min_y_distance = 0.2 # Distância mínima (eixo y) que a mola para após o pulo.
bounce_max_x_distance = 0.85 # Distância máxima (eixo x) que a mola para após o pulo.
bounce_max_y_distance = 1.4 # Distância máxima (eixo y) que a mola para após o pulo.

restart_wait = 60 # Tempo que o programa espera depois que a mola pousou antes de retomar sua posição inicial.
y_start = -0.8 # Posição de "spawn" da mola no eixo y



# == LOOP PRINCIPAL DA JANELA ==


while not glfw.window_should_close(window):

    if(space_pressed == False): # Enquanto não apertar [ESPAÇO], só desenha a mola normal parada.
        desenhar()
        continue

    current_compression = 0.0
    while space_pressed == True: # Apertou [ESPAÇO], então começar a compressionar a mola
        if current_compression < compress_max:
            current_compression += compress_rate
        desenhar(scale_y = 1.0 - current_compression)

    if current_compression < compress_min:
        continue # Não compressionou a mola o bastante! Sem forças para pular.

    # Colocou forças o bastante na compressão! Definir agora se vai pular para esquerda ou direita.
    direction = rand.uniform(0.0, 1.0)
    if(direction > bounce_random_probability):
        direction = -1.0 # Vai pular para esquerda
    else:
        direction = 1.0 # Vai pular para direita

    compression_normalized = (current_compression - compress_min) / (compress_max - compress_min) # Normaliza entre 0..1.
    posx = 0.0 # Posição começa em 0.
    posx_max = bounce_min_x_distance + compression_normalized * (bounce_max_x_distance - bounce_min_x_distance) # A força de compressão decide a posição x máxima.
    posy_max = bounce_min_y_distance + compression_normalized * (bounce_max_y_distance - bounce_min_y_distance) # A força de compressão decide a posição y máxima.
    posy_factor = posy_max / math.pow(posx_max / 2.0, 2) # Isso é apenas para fazer um efeito de força gravidade no eixo y.
    while posx < posx_max: # Loop que faz o pulo efetivamente.
        posx += bounce_rate # Aumenta a posição no eixo x.
        posy = -posy_factor * math.pow(posx - posx_max / 2, 2) + posy_max # Calcula a posição no eixo y.
        normalized_x = max(0.0, min(1.0, posx / posx_max)) * 2.0 - 1.0 # Normaliza posição x entre -1..1
        angle = math.acos(normalized_x) # Calcula o ângulo.
        desenhar(translation_x = direction * posx, translation_y = posy, rotation = direction * angle)

    c = 0.0
    while c < current_compression: # Animação de quando bate no chão - parte 1 (compressionando)
        desenhar(translation_x = direction * posx, scale_y = 1.0 - c)
        c += 10 * compress_rate

    while c > 0.0: # Animação de quando bate no chão - parte 2 (retornando ao estado normal)
        desenhar(translation_x = direction * posx, scale_y = 1.0 - c)
        c -= 10 * compress_rate

    time = 0
    while time < restart_wait: # Espera um pouco antes de retornar a mola para a posição original.
        time += 1
        desenhar(translation_x = direction * posx)

    space_pressed = False # Força que o usuário pressiona o [ESPAÇO] novamente, caso queira uma nova animação.


glfw.terminate()


