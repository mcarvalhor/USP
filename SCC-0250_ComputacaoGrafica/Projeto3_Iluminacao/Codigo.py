#!/usr/bin/env python
# coding: utf-8



#
 #	~ Projeto 3: Iluminação ~
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



# ### Primeiro, vamos importar as bibliotecas necessárias.
# Usaremos as bibliotecas abaixo.

# In[342]:


import glfw
from OpenGL.GL import *
import OpenGL.GL.shaders
import numpy as np
import glm
import math
from PIL import Image
import threading
import random
from time import sleep


# ### Definindo variáveis globais
# 
# Estas variáveis vão ser usadas ao longo do código.

# In[343]:


OBJ_FILE_PATH_PATTERN = "objects/%s/vertex.obj" # Onde ficarão os arquivos *.obj dos objetos que serão desenhados na tela (%s = id do objeto).
TEXTURE_FILE_PATH_PATTERN = "objects/%s/texture_%s.png" # Onde ficarão os arquivos de textura dos objetos que serão desenhados na tela (%s = id do objeto, %s = qualidade da textura).
TEXTURE_QUALITY = "low" # Qualidade das texturas carregadas, que pode ser "high", "med" ou "low".

WINDOW_WIDTH = 800 # Largura da janela.
WINDOW_HEIGHT = 800 # Altura da janela.
WINDOW_MIN_SIZE = 300 # Tamanho mínimo da janela permitido.
WINDOW_MAX_SIZE = 50000 # Tamanho máximo da janela permitido.

WINDOW_TITLE = "Projeto 3 - Iluminação" # Nome da janela.


# ### Inicializando janela

# In[344]:


glfw.init()
glfw.window_hint(glfw.VISIBLE, glfw.FALSE)
window = glfw.create_window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, None, None)
glfw.make_context_current(window)

winWidth = WINDOW_WIDTH
winHeight = WINDOW_HEIGHT

glfw.set_window_size_limits(window, WINDOW_MIN_SIZE, WINDOW_MIN_SIZE, WINDOW_MAX_SIZE, WINDOW_MAX_SIZE)


# ### GLSL (OpenGL Shading Language) para Vertex Shader e Fragment Shader
# 
# No Pipeline programável, podemos interagir com Vertex Shaders e Fragment Shaders. Abaixo, os códigos GLSL são compilados e enviados à GPU.

# In[345]:


### Vertex Shader ###
vertex_code = """
        attribute vec3 position;
        attribute vec2 texture_coord;
        attribute vec3 normals;
        
       
        varying vec2 out_texture;
        varying vec3 out_fragPos;
        varying vec3 out_normal;
                
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;        
        
        void main(){
            gl_Position = projection * view * model * vec4(position,1.0);
            out_texture = vec2(texture_coord);
            out_fragPos = vec3(model * vec4(position, 1.0));
            out_normal = normals;            
        }
        """

### Fragment Shader ###
fragment_code = """

        uniform vec3 lightPos1; // define coordenadas de posicao da luz 1 (lua)
        uniform vec3 lightPos2; // define coordenadas de posicao da luz 2 (poste)
        uniform vec3 lightPos3; // define coordenadas de posicao da luz 3 (ambiente interno)
        
        uniform bool energy; // energia ligada/desligada
        uniform bool internal; // objeto do ambiente interno/externo
        
        uniform float ka; // coeficiente de reflexao ambiente
        uniform float kd; // coeficiente de reflexao difusa

        // parametros da iluminacao especular
        uniform vec3 viewPos; // define coordenadas com a posicao da camera/observador
        uniform float ks; // coeficiente de reflexao especular
        uniform float ns; // expoente de reflexao especular
        
        vec3 lightColor = vec3(1.0, 1.0, 1.0);

        varying vec2 out_texture; // recebido do vertex shader
        varying vec3 out_normal; // recebido do vertex shader
        varying vec3 out_fragPos; // recebido do vertex shader
        uniform sampler2D samplerTexture;
        
        
        
        void main(){
            vec3 ambient = ka * lightColor;

            ////////////////////////
            // Luz #1 (lua)
            ////////////////////////
            
            // calculando reflexao difusa
            vec3 norm1 = normalize(out_normal); // normaliza vetores perpendiculares
            vec3 lightDir1 = normalize(lightPos1 - out_fragPos); // direcao da luz
            float diff1 = max(dot(norm1, lightDir1), 0.0); // verifica limite angular (entre 0 e 90)
            vec3 diffuse1 = kd * diff1 * lightColor; // iluminacao difusa
            
            // calculando reflexao especular
            vec3 viewDir1 = normalize(viewPos - out_fragPos); // direcao do observador/camera
            vec3 reflectDir1 = reflect(-lightDir1, norm1); // direcao da reflexao
            float spec1 = pow(max(dot(viewDir1, reflectDir1), 0.0), ns);
            vec3 specular1 = ks * spec1 * lightColor;
            
            vec3 diffuse2;
            vec3 specular2;
            
            if(energy && internal) {

                ////////////////////////
                // Luz #3 (ambiente interno)
                ////////////////////////

                // calculando reflexao difusa
                vec3 norm2 = normalize(out_normal); // normaliza vetores perpendiculares
                vec3 lightDir2 = normalize(lightPos3 - out_fragPos); // direcao da luz
                float diff2 = max(dot(norm2, lightDir2), 0.0); // verifica limite angular (entre 0 e 90)
                diffuse2 = kd * diff2 * lightColor; // iluminacao difusa

                // calculando reflexao especular
                vec3 viewDir2 = normalize(viewPos - out_fragPos); // direcao do observador/camera
                vec3 reflectDir2 = reflect(-lightDir2, norm2); // direcao da reflexao
                float spec2 = pow(max(dot(viewDir2, reflectDir2), 0.0), ns);
                specular2 = ks * spec2 * lightColor;
            
            } else if(energy && !internal) {
            
                ////////////////////////
                // Luz #2 (poste)
                ////////////////////////

                // calculando reflexao difusa
                vec3 norm2 = normalize(out_normal); // normaliza vetores perpendiculares
                vec3 lightDir2 = normalize(lightPos2 - out_fragPos); // direcao da luz
                float diff2 = max(dot(norm2, lightDir2), 0.0); // verifica limite angular (entre 0 e 90)
                diffuse2 = kd * diff2 * lightColor; // iluminacao difusa

                // calculando reflexao especular
                vec3 viewDir2 = normalize(viewPos - out_fragPos); // direcao do observador/camera
                vec3 reflectDir2 = reflect(-lightDir2, norm2); // direcao da reflexao
                float spec2 = pow(max(dot(viewDir2, reflectDir2), 0.0), ns);
                specular2 = ks * spec2 * lightColor;
                
                
            } else {
                diffuse2 = vec3(0, 0, 0);
                specular2 = vec3(0, 0, 0);
            
            }
            
            if(internal) { // Objeto dentro da casa não tem iluminação especular da lua.
                specular1 = vec3(0, 0, 0);
            }
            
            ////////////////////////
            // Combinando as duas fontes de luz (interna + da lua OU poste + da lua)
            ////////////////////////

            // aplicando o modelo de iluminacao
            vec4 texture = texture2D(samplerTexture, out_texture);
            vec4 result = vec4((ambient + diffuse1 + diffuse2 + specular1 + specular2), 1.0) * texture; // aplica iluminacao
            gl_FragColor = result;
            

        }
        """

# Request a program and shader slots from GPU.
program  = glCreateProgram()
vertex   = glCreateShader(GL_VERTEX_SHADER)
fragment = glCreateShader(GL_FRAGMENT_SHADER)

# Set shaders source.
glShaderSource(vertex, vertex_code)
glShaderSource(fragment, fragment_code)

# Compile Vertex Shaders.
glCompileShader(vertex)
if not glGetShaderiv(vertex, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(vertex).decode()
    print(error)
    raise RuntimeError("Erro de compilacao do Vertex Shader")

# Compile Vertex Shaders.
glCompileShader(fragment)
if not glGetShaderiv(fragment, GL_COMPILE_STATUS):
    error = glGetShaderInfoLog(fragment).decode()
    print(error)
    raise RuntimeError("Erro de compilacao do Fragment Shader")

# Attach shader objects to the program.
glAttachShader(program, vertex)
glAttachShader(program, fragment)

# Build program.
glLinkProgram(program)
if not glGetProgramiv(program, GL_LINK_STATUS):
    print(glGetProgramInfoLog(program))
    raise RuntimeError('Linking error')
    
# Make program the default program.
glUseProgram(program)


# ### Preparando dados para enviar a GPU: classe dos objetos desenhados
# 
# Nesse momento, nós compilamos nossos Vertex e Program Shaders para que a GPU possa processá-los. Por outro lado, as informações de vértices geralmente estão na CPU, e agora devem ser transmitidas para a GPU.
# 
# Para facilitar isso, fizemos uma classe que representa qualquer objeto que se deseja desenhar na tela. Dessa forma, não é necessário criar uma função exclusiva para cada um desses objetos - economizando linhas de código. Além disso, com essa classe fica mais fácil manipular os objetos.
# 
# Essa classe já é responsável por tudo, e objetiva abstrair a parte de alocação dos buffers e sincronização com a GPU.

# In[346]:


class SceneObject:

    _currentTextureIndex = 0 # Indica o id da textura atual (a cada objeto SceneObject criado, isso incrementa).
    _points = None # Armazena todos os pontos de todos os vértices dos objetos SceneObject criados, para serem enviados para a GPU.
    _textures = None # Armazena todos os pontos de todas as texturas de SceneObject criados, para serem enviados para a GPU.
    
    # Coeficientes multiplicadores (permite que o usuário alterne a iluminação dinamicamente).
    KaCoefficient = 1.0
    KdCoefficient = 1.0
    KsCoefficient = 1.0
    NsCoefficient = 1.0

    def __init__(self, objName = None, x = 0.0, y = 0.0, z = 0.0, sx = 1.0, sy = 1.0, sz = 1.0, rx = 0.0, ry = 0.0, rz = 0.0, ka = 1.0, kd = 0.5, ks = 0.0, ns = 4, internal = False, visible = True):
        ### Construtor de um objeto 3D que será desenhado na tela. ###

        self._thread = None # Inicialmente, não é necessário criar uma thread por objeto (a não ser que ele vá se mover depois).
        self._threadStop = False

        # Definir posição inicial do objeto no mundo.
        self.x = x
        self.y = y
        self.z = z

        # Definir escala inicial do objeto.
        self.sx = sx
        self.sy = sy
        self.sz = sz

        # Definir rotação inicial do objeto (em radianos).
        self.rx = rx
        self.ry = ry
        self.rz = rz

        # Definir parâmetros de iluminação Ambiente, Difusa e Especular
        self.ka = ka
        self.kd = kd
        self.ks = ks
        self.ns = ns

        # Definir como objeto interno/externo.
        self.internal = internal
        
        # Definir objeto como visível.
        self.visible = visible

        # Definição do índice de textura desse objeto, início dos vértices e término.
        self._textureIndex = None
        self._vertexIndex = None
        self._vertexLength = None

        if objName is not None:
            print("Construindo objeto '%s'... Carregando pontos... " % (objName), end = "")
            self.setVertex(OBJ_FILE_PATH_PATTERN % (objName))
            print("Carregando textura... ", end = "")
            self.setTexture(TEXTURE_FILE_PATH_PATTERN % (objName, "high"))
            print("Pronto!")

    def setVertex(self, filePath):
        ### Essa função vai definir os vértices deste objeto no mundo usando um arquivo *.obj em disco. ###
        if self._vertexIndex is not None or self._vertexLength is not None:
            raise Exception("Não é possível definir os vértices do objeto porque eles já foram definidos! Crie um novo objeto se quer definir novos vértices.")

        vertices = [ ]
        texture_coords = [ ]
        faces = [ ]
        material = None

        # Abre o arquivo *.obj para leitura.
        with open(filePath, "r") as fp:
            for line in fp: # Para cada linha do arquivo *.obj...
                if line.startswith('#'):
                    continue # Ignora comentarios.
                
                values = line.split() # Quebra a linha por espaço.
                if not values:
                    continue
                
                if values[0] == 'v': # A linha representa um vértice.
                    vertices.append(values[1:4])
                elif values[0] == 'vt': # A linha representa coordenadas de textura.
                    texture_coords.append(values[1:3])
                elif values[0] in ('usemtl', 'usemat'): # A linha representa como serão determinadas as faces.
                    material = values[1]
                elif values[0] == 'f' and material is not None: # A linha representa uma face.
                    face = []
                    face_texture = []
                    for v in values[1:]:
                        w = v.split('/')
                        face.append(int(w[0]))
                        if len(w) >= 2 and len(w[1]) > 0:
                            face_texture.append(int(w[1]))
                        else:
                            face_texture.append(0)

                    faces.append((face, face_texture, material))

        model = { }
        model['vertices'] = vertices
        model['texture'] = texture_coords
        model['faces'] = faces

        if SceneObject._points is None or SceneObject._textures is None: # Primeiro devemos preparar a GPU para receber os dados (executado apenas na criação do primeiro objeto).
            glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE)
            glEnable(GL_BLEND)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
            glEnable(GL_LINE_SMOOTH)
            glEnable(GL_TEXTURE_2D)
            SceneObject._points = [ ]
            SceneObject._textures = [ ]

        self._vertexIndex = len(SceneObject._points)
        for face in model['faces']:
            for vertice_id in face[0]:
                SceneObject._points.append(model['vertices'][vertice_id - 1])
            for texture_id in face[1]:
                SceneObject._textures.append(model['texture'][texture_id - 1])
        self._vertexLength = len(SceneObject._points) - self._vertexIndex

    def setTexture(self, filePath):
        ### Essa função vai definir a textura deste objeto no mundo usando um arquivo em disco. ###
        if self._textureIndex is None:
            self._textureIndex = SceneObject._currentTextureIndex
            SceneObject._currentTextureIndex += 1
        img = Image.open(filePath)
        img_width = img.size[0]
        img_height = img.size[1]
        image_data = img.convert("RGBA").tobytes("raw", "RGBA", 0, -1)
        #image_data = img.tobytes("raw", "RGB", 0, -1)
        #image_data = np.array(list(img.getdata()), np.uint8)
        glBindTexture(GL_TEXTURE_2D, self._textureIndex)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data)
    
    @staticmethod
    def syncGPU():
        ### Essa função sincroniza os dados dos vértices e coordenadas de textura com a GPU. Basicamente, ela envia os dados pra GPU. ###
        # Ela deve ser chamada apenas após criação de todos os objetos que serão desenhados.

        if SceneObject._points is None or SceneObject._textures is None:
            raise Exception("Nenhum objeto foi definido para ser desenhado na tela. Por favor, faça isso!")

        buffer = glGenBuffers(2) # Requisitar slots de buffer pra GPU.

        # Definir vértices.
        vertices = np.zeros(len(SceneObject._points), [("position", np.float32, 3)])
        vertices['position'] = SceneObject._points

        # Carregar vértices.
        glBindBuffer(GL_ARRAY_BUFFER, buffer[0])
        glBufferData(GL_ARRAY_BUFFER, vertices.nbytes, vertices, GL_STATIC_DRAW)
        stride = vertices.strides[0]
        offset = ctypes.c_void_p(0)
        loc_vertices = glGetAttribLocation(program, "position")
        glEnableVertexAttribArray(loc_vertices)
        glVertexAttribPointer(loc_vertices, 3, GL_FLOAT, False, stride, offset)

        # Definir coordenadas de texturas.
        textures = np.zeros(len(SceneObject._textures), [("position", np.float32, 2)]) # Duas coordenadas.
        textures['position'] = SceneObject._textures

        # Carregar coordenadas de texturas.
        glBindBuffer(GL_ARRAY_BUFFER, buffer[1])
        glBufferData(GL_ARRAY_BUFFER, textures.nbytes, textures, GL_STATIC_DRAW)
        stride = textures.strides[0]
        offset = ctypes.c_void_p(0)
        loc_texture_coord = glGetAttribLocation(program, "texture_coord")
        glEnableVertexAttribArray(loc_texture_coord)
        glVertexAttribPointer(loc_texture_coord, 2, GL_FLOAT, False, stride, offset)

    def draw(self):
        ### Essa função vai desenhar este objeto no mundo (usando a matriz model). ###

        if self._textureIndex is None or self._vertexIndex is None or self._vertexLength is None:
            raise Exception("Você precisa primeiro definir o arquivo *.obj e de textura antes de desenhar o objeto na tela!")

        if not self.visible: # Se objeto não estiver visível, não precisa desenhar.
            return
    
        matrix_transform = glm.mat4(1.0) # Instanciando uma matriz identidade.
        
        # Aplicando translação em X, Y e Z.
        matrix_transform = glm.translate(matrix_transform, glm.vec3(self.x, self.y, self.z))

        # Aplicando rotação no eixo X.
        matrix_transform = glm.rotate(matrix_transform, self.rx, glm.vec3(1.0, 0.0, 0.0))

        # Aplicando rotação no eixo Y.
        matrix_transform = glm.rotate(matrix_transform, self.ry, glm.vec3(0.0, 1.0, 0.0))

        # Aplicando rotação no eixo Z.
        matrix_transform = glm.rotate(matrix_transform, self.rz, glm.vec3(0.0, 0.0, 1.0))
        
        # Aplicando escala em X, Y e Z.
        matrix_transform = glm.scale(matrix_transform, glm.vec3(self.sx, self.sy, self.sz))
        
        matrix_transform = np.array(matrix_transform).T # Pegando a transposta da matriz (glm trabalha com ela invertida).

        loc_model = glGetUniformLocation(program, "model")
        glUniformMatrix4fv(loc_model, 1, GL_TRUE, matrix_transform)

        # Envia ka para a GPU.
        loc_ka = glGetUniformLocation(program, "ka")
        glUniform1f(loc_ka, max(min(self.ka * SceneObject.KaCoefficient, 1), 0))
        
        # Envia kd para a GPU.
        loc_kd = glGetUniformLocation(program, "kd")
        glUniform1f(loc_kd, max(min(self.kd * SceneObject.KdCoefficient, 1), 0))

        # Envia ks para a GPU.
        loc_ks = glGetUniformLocation(program, "ks")
        glUniform1f(loc_ks, max(min(self.ks * SceneObject.KsCoefficient, 1), 0))

        # Envia ns para a GPU.
        loc_ns = glGetUniformLocation(program, "ns")
        glUniform1f(loc_ns, 1 - 1 / max(min(self.ns * SceneObject.NsCoefficient, 100000), 0.001))
        #glUniform1f(loc_ns, 0.5)
        
        # Objeto do ambiente interno ou não.
        loc_internal = glGetUniformLocation(program, "internal")
        glUniform1i(loc_internal, 1 if self.internal else 0)
        
        # Define o id da textura do modelo.
        glBindTexture(GL_TEXTURE_2D, self._textureIndex)
        
        # Desenha o modelo.
        glDrawArrays(GL_TRIANGLES, self._vertexIndex, self._vertexLength) # Rendeniza na tela.

    def _alive():
        ### Essa função faz as tarefas do objeto vivo. Por exemplo, se ele se mover, será feito esse movimento nessa função. Se ele não for um objeto vivo, ela nunca é chamada. ###
        pass # Ela deve ser sobescrita por classes filhas, de forma que o código das tarefas a serem feitas pelo objeto fica aqui.

    def spawn(self):
        ### Essa função só serve para objetos que tem vida própria (vão se mover na tela por exemplo). Ela basicamente dá vida ao objeto e ele começa a se mexer. ###

        if self._thread is not None:
            raise Exception("Esse objeto já está vivo! Para ele nascer de novo, primeiro mate ele.")
        
        self._thread = threading.Thread(target = self.__threadLoop)
        self._threadStop = False
        self._thread.start()
        return self
    
    def kill(self):
        ### Essa função só serve para objetos que tem vida própria (vão se mover na tela por exemplo). Ela basicamente tira a vida de um objeto (para de se mexer). ###

        if self._thread is None:
            raise Exception("Esse objeto não está vivo! Para ele morrer, primeiro faça ele nascer.")

        self._threadStop = True
        self._thread.join()
        self._thread = None

    def __threadLoop(self):
        ### Essa função faz o loop da thread do objeto (se ele tiver vida). Se o objeto não tiver vida, ela nunca é chamada. ###
        # Observe que essa função é interna e não deve ser modificada por classes filhas.

        while(not self._threadStop):
            self._alive()


# ### Eventos para modificar a posição da câmera e controle da janela.
# 
# Os controles são descritos mais detalhadamente na última seção.

# In[347]:


CAMERA_SPEED = 0.2 # Velocidade de locomoção da câmera.
CAMERA_Y = 0.7 # Posição da câmera quando bloqueada no cenário.
CAMERA_FOVY = glm.radians(60.0) # Campo de visão (field-of-view) da câmera.
CAMERA_NEAR = 0.01 # Mínimo do campo de visão da câmera.
CAMERA_FAR = 1000 # Máximo do campo de visão da câmera.
CAMERA_X_MIN = -16
CAMERA_X_MAX = 16
CAMERA_Z_MIN = -16
CAMERA_Z_MAX = 16

polygonal_mode = False # Modo polígono ativado?
free_camera = False # Modo câmera livre ativado?
paused = False # Jogo pausado?

W_pressed = False
S_pressed = False
A_pressed = False
D_pressed = False
G_pressed = False

cameraUp = glm.vec3(0.0,  1.0,  0.0)
cameraFront = glm.vec3(0.0,  0.0, -1.0)
cameraPos = glm.vec3(5.0,  CAMERA_Y,  6.0)

moonPositionY = 50 # Posição da Lua (afeta a iluminação).
energyOn = True # Energia ligada/desligada

yaw = 0.0 
pitch = 0.0
lastX = winWidth / 2
lastY = winHeight / 2

class Camera(SceneObject):
    def _alive(self):
        global cameraPos, cameraFront, cameraUp, paused, CAMERA_SPEED
        ### A câmera se move usando uma thread separada. Esse objeto não desenha nada na tela, é usado apenas por conta da thread mesmo. ###
        # O motivo de usar uma thread separada é porque assim a câmera terá um movimento mais suave, além de que é possível apertar várias teclas ao mesmo tempo.

        if paused: # O jogo está pausado. Dorme o processo pra economizar recursos do processador.
            sleep(1.0)
            return

        if free_camera: # No modo de câmera livre, a velocidade é maior
            camera_speed = 4 * CAMERA_SPEED
        else:
            camera_speed = CAMERA_SPEED

        if W_pressed:
            cameraPos += cameraFront * camera_speed

        if S_pressed:
            cameraPos -= cameraFront * camera_speed

        if A_pressed:
            cameraPos -= glm.normalize(glm.cross(cameraFront, cameraUp)) * camera_speed

        if D_pressed:
            cameraPos += glm.normalize(glm.cross(cameraFront, cameraUp)) * camera_speed

        if not free_camera: # A câmera não está em modo livre, então vamos limitar o jogador aos limites do cenário.
            cameraPos[1] = CAMERA_Y
            cameraPos[0] = max(CAMERA_X_MIN, min(CAMERA_X_MAX, cameraPos[0]))
            cameraPos[2] = max(CAMERA_Z_MIN, min(CAMERA_Z_MAX, cameraPos[2]))

        sleep(0.05)


def key_event(window,key,scancode,action,mods): # Tecla pressionada.
    global polygonal_mode, paused, free_camera, energyOn, W_pressed, S_pressed, A_pressed, D_pressed, G_pressed, lastX, lastY
    
    if key == 87: # Tecla W.
        W_pressed = False if (action == 0) else True
    
    elif key == 83: # Tecla S.
        S_pressed = False if (action == 0) else True
    
    elif key == 65: # Tecla A.
        A_pressed = False if (action == 0) else True
        
    elif key == 68: # Tecla D.
        D_pressed = False if (action == 0) else True
        
    elif key == 71: # Tecla G.
        G_pressed = False if (action == 0) else True

    elif key == 80 and action == 0: # Botão P (modo polígono).
        polygonal_mode = not polygonal_mode

    elif key == 67 and action == 0: # Botão C (modo de câmera: bloqueado ou livre).
        free_camera = not free_camera
        
    elif key == 76 and action == 0: # Botão L (energia ligada/desligada)
        energyOn = not energyOn
        
    elif key == 89 and action != 0: # Botão Y (parâmetro ka)
        SceneObject.KaCoefficient = (SceneObject.KaCoefficient + 0.2) % 3.0
        print("Nota: multiplicando parâmetro de iluminação 'ka' por %f." % (SceneObject.KaCoefficient))
        
    elif key == 85 and action != 0: # Botão U (parâmetro kd)
        SceneObject.KdCoefficient = (SceneObject.KdCoefficient + 0.2) % 3.0
        print("Nota: multiplicando parâmetro de iluminação 'kd' por %f." % (SceneObject.KdCoefficient))
        
    elif key == 73 and action != 0: # Botão I (parâmetro ks)
        SceneObject.KsCoefficient = (SceneObject.KsCoefficient + 0.2) % 3.0
        print("Nota: multiplicando parâmetro de iluminação 'ks' por %f." % (SceneObject.KsCoefficient))
    
    elif key == 79 and action != 0: # Botão O (parâmetro ns)
        SceneObject.NsCoefficient = (SceneObject.NsCoefficient + 0.5) % 8.0
        print("Nota: multiplicando parâmetro de iluminação 'ns' por %f." % (SceneObject.NsCoefficient))
    
    elif (key == 257 or key == 256) and action == 0: # Botão ENTER ou ESC (câmera lock).
        paused = not paused
        glfw.set_cursor_pos(window, lastX, lastY)
        glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_NORMAL if paused else glfw.CURSOR_HIDDEN)

    #print("Tecla '%s' apertada com scancode = '%s' (action = %s)." % (key, scancode, action))

def mouse_event(window, xpos, ypos): # Mouse movimentado
    global paused, firstMouse, cameraFront, yaw, pitch, lastX, lastY

    if paused: # Jogo está pausado, não mudar nada na câmera.
        return

    xoffset = xpos - lastX
    yoffset = lastY - ypos

    sensitivity = 0.3 
    xoffset *= sensitivity
    yoffset *= sensitivity

    yaw += xoffset
    pitch += yoffset

    
    if pitch >= 80.0: pitch = 80.0
    if pitch <= -80.0: pitch = -80.0

    front = glm.vec3()
    front.x = math.cos(glm.radians(yaw)) * math.cos(glm.radians(pitch))
    front.y = math.sin(glm.radians(pitch))
    front.z = math.sin(glm.radians(yaw)) * math.cos(glm.radians(pitch))
    cameraFront = glm.normalize(front)

    glfw.set_cursor_pos(window, lastX, lastY) # Dúvida: isso gera uma recursão infinita? Não há informações o suficiente na internet, e meus testes indicam que não.

def window_event(window, width, height): # A janela foi redimensionada, então ajustar parâmetros.
    global winWidth, winHeight, lastX, lastY

    winWidth = width
    winHeight = height
    lastX = winWidth / 2
    lastY = winHeight / 2
    glViewport(0, 0, winWidth, winHeight)

camera = Camera()
glfw.set_key_callback(window,key_event)
glfw.set_cursor_pos_callback(window, mouse_event)
glfw.set_window_size_callback(window, window_event)
glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_HIDDEN)


# ### Matrizes View e Projection
# 
# Abaixo são gerados as matrizes View e Projection. Nota: a matriz model é gerada pelo método "draw" de cada objeto da classe SceneObject.

# In[348]:


def view():
    global cameraPos, cameraFront, cameraUp
    mat_view = glm.lookAt(cameraPos, cameraPos + cameraFront, cameraUp)
    mat_view = np.array(mat_view)
    return mat_view

def projection():
    global winHeight, winWidth, CAMERA_FOVY, CAMERA_NEAR, CAMERA_FAR
    mat_projection = glm.perspective(CAMERA_FOVY, winWidth/winHeight, CAMERA_NEAR, CAMERA_FAR)
    mat_projection = np.array(mat_projection)    
    return mat_projection


# ### Nesse momento, nós exibimos a janela.
# 
# Queremos exibir a janela antes de definir os objetos: dessa forma será mostrada uma tela de carregamento para o usuário.

# In[349]:


loadingScreen = SceneObject(objName = "loadingScreen", rx = math.pi / 2, ka = 1.0, kd = 0.0, ks = 0.0) # Criar tela de carregamento.
SceneObject.syncGPU() # Enviar tela de carregamento para GPU.

# Localização da fonte de luz 1 (lua).
loc_light_pos = glGetUniformLocation(program, "lightPos1")
glUniform3f(loc_light_pos, 45, 50, 45)

# Localização da fonte de luz 2 (poste).
loc_light_pos = glGetUniformLocation(program, "lightPos2")
glUniform3f(loc_light_pos, 5.3, 1.825, 2)

# Localização da fonte de luz 3 (ambiente interno).
loc_light_pos = glGetUniformLocation(program, "lightPos3")
glUniform3f(loc_light_pos, 8, 0.5, -0.25)

# Localização do observador a princípio (posteriormente, a câmera controlará essa variável).
loc_light_pos = glGetUniformLocation(program, "viewPos")
glUniform3f(loc_light_pos, 0, 0, 0)

# Energia desligada.
loc_energy = glGetUniformLocation(program, "energy")
glUniform1i(loc_energy, 0)



glfw.show_window(window) # Mostrar janela.

glEnable(GL_DEPTH_TEST) # Importante para rendenização 3D.

for _ in range(8): # Força redesenho (por conta de ser double-buffered).
    glfw.poll_events()
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(1.0, 1.0, 1.0, 1.0) # Fundo branco.
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) # Desativar modo polígono.
    loadingScreen.draw() # Desenhar tela de pause.

    mat_view = np.array(glm.lookAt(glm.vec3(3.0,  0.0, 0.0), glm.vec3(0.0,  0.0, 0.0), glm.vec3(0.0,  1.0, 0.0))) # View.
    loc_view = glGetUniformLocation(program, "view")
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, mat_view)

    mat_projection = projection() # Projection.
    loc_projection = glGetUniformLocation(program, "projection")
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE, mat_projection)
    glfw.swap_buffers(window)


# ### Vamos criar os objetos, carregar os vértices e texturas.
# 
# Abaixo vamos criar cada um dos objetos, carregar seus vértices dos arquivos *.obj e texturas dos arquivos de imagem. Os objetos abaixo são estáticos, ou seja, não têm vida e nem se mexem.

# In[350]:


staticObjects = [ ]

staticObjects.append( SceneObject(objName = "ground", y = -0.9, sx = 20.0, sz = 20.0, ka = 0.3, ks = 0.1, ns = 2) ) # Chão de grama.
staticObjects.append( SceneObject(objName = "mountains", x = 18.0, ry = math.pi/2, ka = 0.3, ks = 0.3, ns = 12) ) # Montanhas 1.
staticObjects.append( SceneObject(objName = "mountains", x = -18.0, ry = math.pi/2, ka = 0.3, ks = 0.3, ns = 12) ) # Montanhas 2.
staticObjects.append( SceneObject(objName = "mountains", z = 18.0, ka = 0.3, ks = 0.3, ns = 12) ) # Montanhas 3.
staticObjects.append( SceneObject(objName = "mountains", z = -18.0, ka = 0.3, ks = 0.3, ns = 12) ) # Montanhas 4.
staticObjects.append( SceneObject(objName = "oldHouse", y = 0.11, sx = 1.5, sz = 1.5, sy = 1.5, ka = 0.3, ks = 0.0) ) # Casa antiga (1).
staticObjects.append( SceneObject(objName = "abandonedHouse", y = 0.01, x = 8, sx = 0.1, sz = 0.1, sy = 0.1, ka = 0.3, ks = 0.0) ) # Casa abandonada isolada (2).
staticObjects.append( SceneObject(objName = "abandonedHouseFloor", y = -0.89, x = 7.75, z = -0.01,sx = 1.51, sz = 0.80, ka = 0.5, ks = 1, ns = 150, internal = True) ) # Piso da casa abandonada (2).
staticObjects.append( SceneObject(objName = "table", y = 0.25, x = 8, sx = 0.003, sz = 0.003, sy = 0.003, ka = 0.5, ks = 1, ns = 150, internal = True) ) # Mesa de madeira (2).
staticObjects.append( SceneObject(objName = "skull", y = 0.40, x = 8.12, z = 0.25, ry = -math.pi/2.0, sx = 0.012, sz = 0.012, sy = 0.012, ka = 0.5, ks = 0.5, ns = 200, internal = True) ) # Crânio de um esqueleto (2).
staticObjects.append( SceneObject(objName = "lantern", y = 0.4, x = 8, z = -0.25, sx = 0.2, sy = 0.2, sz = 0.2, ka = 0.5, ks = 0.5, ns = 200, internal = True) ) # Lanterna em cima da mesa - iluminação ambiente interno (2).
staticObjects.append( SceneObject(objName = "street", y = -0.89, x = 5, z = 6, sx = 0.75, sz = 8, ka = 0.3, ks = 0.8, ns = 150) ) # Rua.
staticObjects.append( SceneObject(objName = "streetLamp", y = 1, x = 5.8, z = 2, sx = 0.08, ry = math.pi/2, sy = 0.08, sz = 0.08, ka = 0.7, ks = 0.8, ns = 150) ) # Poste.

pausedScreen = SceneObject(objName = "pauseScreen", rx = math.pi / 2, ka = 1.0, kd = 0.0, ks = 0.0) # Tela de pause.


# ### Vamos criar objetos com vida (dinâmicos), carregar os vértices e texturas.
# 
# O funcionamento é identico aos objetos criados acima, mas a mudança aqui é que estes objetos possuem vida própria, ou seja, podem se mover ou realizar ações pelo mapa. Para isso, é necessário determinar o que será feito enquanto estiverem vivos, então é necessário criar uma classe filha de SceneObject que descreve os movimentos de cada um dos objetos abaixo.

# In[351]:


dynamicObjects = [ ]
    
class Gun(SceneObject): # A arma que fica dentro da casa fica girando.
    def _alive(self):
        self.ry = (self.ry + 0.1) % (2 * math.pi)
        sleep(0.1)

class Moon(SceneObject): # A lua vai nascendo e se pondo no horizonte aos poucos.
    def _alive(self):
        global moonPositionY, G_pressed
        self.y += 0.04 if not G_pressed else 1
        if self.y >= 70.0: # Fade-out da lua no céu.
            self.ka = max(min((90.0 - self.y) / 20.0, 1.0), 0.0)
            self.ks = 0.5 * max(min((90.0 - self.y) / 20.0, 1.0), 0.0)
        if self.y > 90.0:
            self.y = -10
            self.ka = 1.0
            self.ks = 0.5
        moonPositionY = self.y
        sleep(0.1)

class Sky(SceneObject): # As nuvens no céu vão se movendo lentamente.
    def _alive(self):
        self.ry = (self.ry + 0.001) % (2 * math.pi)
        sleep(0.1)

class Freddy(SceneObject): # O Fantasma do Freddy Sem Cabeça pode se locomover e voar pelo mapa.
    MOVEMENTS = ["walk", "fly", "dissapear"]
    FLOOR_Y = 0.1
    def __init__(self, objName = None, ka = 1.0, ks = 0.0, ns = 0.0):
        self.mov = None
        self.pos = None
        self.speed = None
        self.start = None
        self.end = None
        self.ka = ka
        self.kd = 0.0
        self.ks = ks
        self.ns = ns
        super(Freddy, self).__init__(objName = objName, sx = 0.06, sy = 0.06, sz = 0.06, visible = False)

    def _alive(self):
        if self.mov == None: # Determinar o que ele vai fazer: andar pelo cenário, voar pelo mapa ou desaparecer
            self.mov = random.choice(Freddy.MOVEMENTS)
            self.speed = random.uniform(0.001, 0.01)
            self.pos = 0.0
            self.start = [random.uniform(-16, 16), random.uniform(10, 20), random.uniform(-16, 16)]
            self.end = [random.uniform(-16, 16), random.uniform(10, 20), random.uniform(-16, 16)]
            self.ry = math.atan2((self.end[0] - self.start[0]), (self.end[2] - self.start[2]))
        if self.mov == "walk": # Ele está andando pelo cenário...
            self.x = self.start[0] + self.pos * (self.end[0] - self.start[0])
            self.y = Freddy.FLOOR_Y
            self.z = self.start[2] + self.pos * (self.end[2] - self.start[2])
            self.visible = True
        elif self.mov == "fly": # Ele está voando pelo mapa...
            self.x = self.start[0] + self.pos * (self.end[0] - self.start[0])
            self.y = self.start[1] + self.pos * (self.end[1] - self.start[1])
            self.z = self.start[2] + self.pos * (self.end[2] - self.start[2])
            self.visible = True
        else: # Ele está invisível (desapareceu)...
            self.pos += self.speed
            self.visible = False
        self.pos += self.speed
        if self.pos > 1.0: # A animação acabou. Restaurar para determinar a próxima animação.
            self.mov = None
        sleep(0.1)

dynamicObjects.append( Gun(objName = "gun", x = 8, y = 0.6, sx = 0.015, sy = 0.015, sz = 0.015, ka = 0.5, ks = 1, ns = 200, internal = True) )
dynamicObjects.append( Sky(objName = "sky", sx = 100.0, sy = 100.0, sz = 100.0, ka = 0.2, ks = 0.0) )
dynamicObjects.append( Moon(objName = "moon", x = 45, y = 50, z = 45, sx = 10.0, sy = 10.0, sz = 10.0, ka = 1.0, kd = 1.0, ks = 0.5, ns = 12) )
dynamicObjects.append( Freddy(objName = "freddyGhost", ka = 0.5, ks = 0.5, ns = 12) )


# ### Enviar dados para a GPU.
# 
# Agora que já definimos todos os objetos, devemos enviá-los à GPU. Nossa classe já possui um método estático que faz isso (requisitar slots de buffer -> enviar vértices -> enviar coordenadas de texturas): basta chamar esse método.

# In[352]:


print("Transferindo objetos para GPU... ", end = "")
SceneObject.syncGPU()
print("Pronto.")


# ### Loop principal da janela.
# Enquanto a janela não for fechada, esse laço será executado. É neste espaço que trabalhamos com algumas interações com a OpenGL.

# In[353]:


print("Iniciando objetos... ", end = "")

#glfw.show_window(window)
#glEnable(GL_DEPTH_TEST) # Importante para rendenização 3D.

for obj in dynamicObjects: # Dar vida aos objetos dinâmicos.
    obj.spawn()
camera.spawn() # Iniciar thread da câmera.

print("Pronto!")

glfw.set_cursor_pos(window, lastX, lastY)

while not glfw.window_should_close(window):
    glfw.poll_events() 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glClearColor(0.0, 0.0, 0.0, 1.0) # Fundo preto.

    if paused: # O jogo está pausado?
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) # Desativar modo polígono.

        pausedScreen.draw() # Desenhar tela de pause.

        loc_light_pos = glGetUniformLocation(program, "viewPos")
        glUniform3f(loc_light_pos, 0, 0, 0)
        
        # Energia desligada.
        loc_energy = glGetUniformLocation(program, "energy")
        glUniform1i(loc_energy, 0)

        mat_view = np.array(glm.lookAt(glm.vec3(3.0,  0.0, 0.0), glm.vec3(0.0,  0.0, 0.0), glm.vec3(0.0,  1.0, 0.0))) # View.
        loc_view = glGetUniformLocation(program, "view")
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, mat_view)

        mat_projection = projection() # Projection.
        loc_projection = glGetUniformLocation(program, "projection")
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, mat_projection)
        glfw.swap_buffers(window)
        continue
    
    if polygonal_mode == True: # Modo polígono está ativado?
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
    else:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
    
    # Localização da fonte de luz 1 (lua) é dinâmica.
    loc_light_pos = glGetUniformLocation(program, "lightPos1")
    glUniform3f(loc_light_pos, 45, moonPositionY, 35)
    
    loc_view_pos = glGetUniformLocation(program, "viewPos")
    glUniform3f(loc_view_pos, cameraPos[0], cameraPos[1], cameraPos[2])
    
    # Energia ligada/desligada.
    loc_energy = glGetUniformLocation(program, "energy")
    glUniform1i(loc_energy, 1 if energyOn else 0)

    mat_view = view() # View.
    loc_view = glGetUniformLocation(program, "view")
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, mat_view)

    mat_projection = projection() # Projection.
    loc_projection = glGetUniformLocation(program, "projection")
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE, mat_projection)
    
    for obj in staticObjects: # Desenhar objetos estáticos.
        obj.draw()

    for obj in dynamicObjects: # Desenhar objetos dinâmicos.
        obj.draw()
    
    glfw.swap_buffers(window)

for obj in dynamicObjects: # Tirar vida dos objetos dinâmicos.
    obj.kill()
camera.kill() # Matar thread da câmera.

glfw.terminate() # Fechar janela.


# # Controles e Explicação do Cenário
# 
# O cenário 3D é intuitivo e possui os seguintes controles:
#  - **mouse**: sentido e direção da câmera;
#  - **W**: mova-se para frente;
#  - **A**: mova-se para a esquerda;
#  - **D**: mova-se para a direita;
#  - **S**: mova-se para trás;
#  - **L**: desligar/ligar energia (afetando a luz do poste e do ambiente interno);
#  - **Y**: alternar intensidade do parâmetro "ka" (iluminação ambiente);
#  - **U**: alternar intensidade do parâmetro "kd" (iluminação difusa);
#  - **I**: alternar intensidade do parâmetro "ks" (iluminação especular);
#  - **O**: alternar intensidade do parâmetro "ns" (expoente da iluminação especular);
#  - **ESC** ou **ENTER**: pausar jogo, liberando o mouse para uso em outras janelas;
#  - **P**: alterna modo polígono ativado (permitindo ver como os objetos são rendenizados) e desativado (cenário padrão);
#  - **C**: alterna entre câmera livre (que pode se mover por todo o mapa) e câmera de personagem (que fica presa no cenário e não voa).
# 
# O objetivo era apenas criar um cenário de tema terror/horror. Sendo assim, foi escolhido um cenário escuro, com casas antigas. Além disso, há um personagem (O Fantasma do Freddy Sem Cabeça) que ocasionalmente se move e reaparece pelo cenário.
