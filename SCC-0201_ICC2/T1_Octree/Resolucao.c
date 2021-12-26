#include <stdio.h>
#include <stdlib.h>


/*
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |   |
 *	|   |    \      |   |___|
 *	|_______ _______|___|
 *
*/


// Definições e macros

#define IS_POINT_INSIDE_BOX(Point, Box) \
	( (Point.x>=Box.Min.x && Point.x<=Box.Max.x) \
	&& (Point.y>=Box.Min.y && Point.y<=Box.Max.y) \
	&& (Point.z>=Box.Min.z && Point.z<=Box.Max.z) )



// Enumerações e estruturas

typedef enum { false = 0, true } bool;	// Pseudo-booleano

typedef struct {
	float x,y,z;			// Um ponto é definido por suas coordenadas em x, y e z.
} POINT;

typedef struct {
	POINT Max;			// Uma caixa é definida por seus pontos mínimo (Xmin, Ymin, Zmin) e máximo (Xmax, Ymax, Zmax).
	POINT Min;
} BOX;

typedef struct {
	POINT Coord[3];			// Um triângulo é definido por 3 pontos.
} TRIANGLE;

typedef struct __node {
	BOX BoundingBox;		// Dimensões da caixa delimitadora do octante.
	bool PointHere;			// Indica se o objeto p está nesse octante ou não.
	struct __node *Boxes;		// Este é o vetor das oito caixas do interior (octantes) dos próximos níveis, se o objeto p estiver aqui.
} node;

typedef struct {
	POINT Point;			// Ponto p.
	TRIANGLE *Triangles;		// Vetor com todos os triângulos da malha (objeto O).
	int TrianglesC;			// Número de triângulos na malha.
	int Level;			// Número de níveis máximo de divisões (nível da Octree).
	node Root;			// Primeiro nível da octree.
} octree;



// Declaração de funções

void ReadInput(octree *,bool);
void DeployOctree(node *,int,octree);
bool FindCollisions(node,octree);
void DestroyOctree(node *);



// ~~ Início do código ~~
int main(int argc, char **argv){
	/*
	*  Meu programa vai ler as entradas, montar a octree e tentar encontrar colisões nesta respectivamente.
	*
	*  Após, será feita a liberação dos nós da octree da memória.
	*  Com exceção da leitura das entradas, todas as outras funções são implantadas de forma recursiva.
	*/
	octree MainOctree;
	bool CreateBoundingBox = (getchar()=='0') ? false : true; // É necessário implantar a caixa delimitadora?
	ReadInput(&MainOctree,CreateBoundingBox); // Ler entradas, alocar memória na HEAP (dinamicamente) e armazenar os dados na Octree.
	DeployOctree(&MainOctree.Root,MainOctree.Level,MainOctree); // Chamar recursiva para montar toda a estrutura da Octree.
	if(FindCollisions(MainOctree.Root,MainOctree)==true){ // Chamar recursiva para encontrar uma colisão nessa Octree, se houver.
		printf("1\n"); // Houve colisão.
	}else{
		printf("0\n"); // Não houve colisão.
	}
	DestroyOctree(&MainOctree.Root); // Chamar recursiva para limpar os octantes da memória.
	free(MainOctree.Triangles); // Limpar vetor de triângulos que compõem a malha total.
	return EXIT_SUCCESS;
}



void ReadInput(octree *Octree, bool CreateBoundingBox){
	/*
	*  Esta função vai ler todas as entradas de 'stdin' e salvar em seus locais apropriados na Octree (dinamicamente).
	*/
	int i,j;
	POINT R;
	scanf("%d %d %f %f %f",&Octree->Level,&Octree->TrianglesC,&Octree->Point.x,&Octree->Point.y,&Octree->Point.z); // Ler o nível da octree, o número de triângulos e ler as coordenadas do objeto p.
	Octree->Triangles=(TRIANGLE *)malloc(Octree->TrianglesC*sizeof(TRIANGLE)); // Alocar a memória para os triângulos.
	if(CreateBoundingBox==false){
		for(i=0;i<Octree->TrianglesC;i++){ // Para cada triângulo...
			for(j=0;j<3;j++){ // e para cada ponto deste...
				scanf("%f %f %f",&Octree->Triangles[i].Coord[j].x,&Octree->Triangles[i].Coord[j].y,&Octree->Triangles[i].Coord[j].z); // Ler os pontos do triângulo.
			}
		}
		scanf("%f %f %f",&Octree->Root.BoundingBox.Min.x,&Octree->Root.BoundingBox.Min.y,&Octree->Root.BoundingBox.Min.z); // Ler o primeiro ponto da caixa delimitadora.
		Octree->Root.BoundingBox.Max=Octree->Root.BoundingBox.Min;
		for(i=1;i<8;i++){ // Ler os outros pontos.
			scanf("%f %f %f",&R.x,&R.y,&R.z);
			if(R.x>Octree->Root.BoundingBox.Max.x){
				Octree->Root.BoundingBox.Max.x=R.x;
			}else if(R.x<Octree->Root.BoundingBox.Min.x){
				Octree->Root.BoundingBox.Min.x=R.x;
			}
			if(R.y>Octree->Root.BoundingBox.Max.y){
				Octree->Root.BoundingBox.Max.y=R.y;
			}else if(R.y<Octree->Root.BoundingBox.Min.y){
				Octree->Root.BoundingBox.Min.y=R.y;
			}
			if(R.z>Octree->Root.BoundingBox.Max.z){
				Octree->Root.BoundingBox.Max.z=R.z;
			}else if(R.z<Octree->Root.BoundingBox.Min.z){
				Octree->Root.BoundingBox.Min.z=R.z;
			}
		}
	}else{
		// É necessário implantar a caixa delimitadora.
		// Nesse caso vamos ler o primeiro triângulo individualmente e o resto dos triângulos em um 'loop for'.
		scanf("%f %f %f",&Octree->Triangles[0].Coord[0].x,&Octree->Triangles[0].Coord[0].y,&Octree->Triangles[0].Coord[0].z); // Ler o primeiro ponto do primeiro triângulo.
		Octree->Root.BoundingBox.Min=Octree->Root.BoundingBox.Max=Octree->Triangles[0].Coord[0];
		for(j=1;j<3;j++){ // Ler os outros dois pontos do primeiro triângulo.
			scanf("%f %f %f",&Octree->Triangles[0].Coord[j].x,&Octree->Triangles[0].Coord[j].y,&Octree->Triangles[0].Coord[j].z);
			if(Octree->Triangles[0].Coord[j].x>Octree->Root.BoundingBox.Max.x){ // Verificar se o ponto ultrapassa a caixa delimitadora, e se sim, atualizá-la.
				Octree->Root.BoundingBox.Max.x=Octree->Triangles[0].Coord[j].x;
			}else if(Octree->Triangles[0].Coord[j].x<Octree->Root.BoundingBox.Min.x){
				Octree->Root.BoundingBox.Min.x=Octree->Triangles[0].Coord[j].x;
			}
			if(Octree->Triangles[0].Coord[j].y>Octree->Root.BoundingBox.Max.y){
				Octree->Root.BoundingBox.Max.y=Octree->Triangles[0].Coord[j].y;
			}else if(Octree->Triangles[0].Coord[j].y<Octree->Root.BoundingBox.Min.y){
				Octree->Root.BoundingBox.Min.y=Octree->Triangles[0].Coord[j].y;
			}
			if(Octree->Triangles[0].Coord[j].z>Octree->Root.BoundingBox.Max.z){
				Octree->Root.BoundingBox.Max.z=Octree->Triangles[0].Coord[j].z;
			}else if(Octree->Triangles[0].Coord[j].z<Octree->Root.BoundingBox.Min.z){
				Octree->Root.BoundingBox.Min.z=Octree->Triangles[0].Coord[j].z;
			}
		}
		for(i=1;i<Octree->TrianglesC;i++){ // Para os outros triângulos da entrada...
			for(j=0;j<3;j++){ // e para cada ponto de cada um desses triângulo...
				scanf("%f %f %f",&Octree->Triangles[i].Coord[j].x,&Octree->Triangles[i].Coord[j].y,&Octree->Triangles[i].Coord[j].z); // Ler ponto e atribuir ele ao triângulo.
				if(Octree->Triangles[i].Coord[j].x>Octree->Root.BoundingBox.Max.x){ // Verificar se o ponto ultrapassa a caixa delimitadora, e se sim, atualizá-la.
					Octree->Root.BoundingBox.Max.x=Octree->Triangles[i].Coord[j].x;
				}else if(Octree->Triangles[i].Coord[j].x<Octree->Root.BoundingBox.Min.x){
					Octree->Root.BoundingBox.Min.x=Octree->Triangles[i].Coord[j].x;
				}
				if(Octree->Triangles[i].Coord[j].y>Octree->Root.BoundingBox.Max.y){
					Octree->Root.BoundingBox.Max.y=Octree->Triangles[i].Coord[j].y;
				}else if(Octree->Triangles[i].Coord[j].y<Octree->Root.BoundingBox.Min.y){
					Octree->Root.BoundingBox.Min.y=Octree->Triangles[i].Coord[j].y;
				}
				if(Octree->Triangles[i].Coord[j].z>Octree->Root.BoundingBox.Max.z){
					Octree->Root.BoundingBox.Max.z=Octree->Triangles[i].Coord[j].z;
				}else if(Octree->Triangles[i].Coord[j].z<Octree->Root.BoundingBox.Min.z){
					Octree->Root.BoundingBox.Min.z=Octree->Triangles[i].Coord[j].z;
				}
			}
		}
	}
}


void DeployOctree(node *OctNode,int OctLevel,octree Octree){
	/*
	*  Esta função implanta toda a Octree até "OctLevel".
	*
	*  Ela é recursiva, e só implanta os octantes aos quais o objeto p está presente.
	*  Com isso, a função evita uso desnecessário de CPU e memória, visto que os outros octantes não nos interessam.
	*/
	if(OctLevel<=0){ // Fim da Octree. Encerrar recursão.
		OctNode->Boxes=NULL;
		return;
	}
	int i;
	OctNode->Boxes=(node *)malloc(8*sizeof(node)); // Alocar memória para os oito octantes do interior desse nó (node).
	{
		POINT MiddlePoint; // Ponto do meio de todo o octante.
		MiddlePoint.x=(OctNode->BoundingBox.Max.x+OctNode->BoundingBox.Min.x)/(float)2;
		MiddlePoint.y=(OctNode->BoundingBox.Max.y+OctNode->BoundingBox.Min.y)/(float)2;
		MiddlePoint.z=(OctNode->BoundingBox.Max.z+OctNode->BoundingBox.Min.z)/(float)2;
		OctNode->Boxes[0].BoundingBox.Min=OctNode->BoundingBox.Min; // Formar a caixa delimitadora do octante 1.
		OctNode->Boxes[0].BoundingBox.Max=MiddlePoint;
		OctNode->Boxes[1].BoundingBox.Min.x=MiddlePoint.x; // Formar a caixa delimitadora do octante 2.
		OctNode->Boxes[1].BoundingBox.Min.y=OctNode->BoundingBox.Min.y;
		OctNode->Boxes[1].BoundingBox.Min.z=OctNode->BoundingBox.Min.z;
		OctNode->Boxes[1].BoundingBox.Max.x=OctNode->BoundingBox.Max.x;
		OctNode->Boxes[1].BoundingBox.Max.y=MiddlePoint.y;
		OctNode->Boxes[1].BoundingBox.Max.z=MiddlePoint.z;
		OctNode->Boxes[2].BoundingBox.Min.x=OctNode->BoundingBox.Min.x; // ...
		OctNode->Boxes[2].BoundingBox.Min.y=MiddlePoint.y;
		OctNode->Boxes[2].BoundingBox.Min.z=OctNode->BoundingBox.Min.z;
		OctNode->Boxes[2].BoundingBox.Max.x=MiddlePoint.x;
		OctNode->Boxes[2].BoundingBox.Max.y=OctNode->BoundingBox.Max.y;
		OctNode->Boxes[2].BoundingBox.Max.z=MiddlePoint.z;
		OctNode->Boxes[3].BoundingBox.Min.x=OctNode->BoundingBox.Min.x; // ...
		OctNode->Boxes[3].BoundingBox.Min.y=OctNode->BoundingBox.Min.y;
		OctNode->Boxes[3].BoundingBox.Min.z=MiddlePoint.z;
		OctNode->Boxes[3].BoundingBox.Max.x=MiddlePoint.x;
		OctNode->Boxes[3].BoundingBox.Max.y=MiddlePoint.y;
		OctNode->Boxes[3].BoundingBox.Max.z=OctNode->BoundingBox.Max.z;
		OctNode->Boxes[4].BoundingBox.Min.x=OctNode->BoundingBox.Min.x; // ...
		OctNode->Boxes[4].BoundingBox.Min.y=MiddlePoint.y;
		OctNode->Boxes[4].BoundingBox.Min.z=MiddlePoint.z;
		OctNode->Boxes[4].BoundingBox.Max.x=MiddlePoint.x;
		OctNode->Boxes[4].BoundingBox.Max.y=OctNode->BoundingBox.Max.y;
		OctNode->Boxes[4].BoundingBox.Max.z=OctNode->BoundingBox.Max.z;
		OctNode->Boxes[5].BoundingBox.Min.x=MiddlePoint.x; // ...
		OctNode->Boxes[5].BoundingBox.Min.y=OctNode->BoundingBox.Min.y;
		OctNode->Boxes[5].BoundingBox.Min.z=MiddlePoint.z;
		OctNode->Boxes[5].BoundingBox.Max.x=OctNode->BoundingBox.Max.x;
		OctNode->Boxes[5].BoundingBox.Max.y=MiddlePoint.y;
		OctNode->Boxes[5].BoundingBox.Max.z=OctNode->BoundingBox.Max.z;
		OctNode->Boxes[6].BoundingBox.Min.x=MiddlePoint.x; // ...
		OctNode->Boxes[6].BoundingBox.Min.y=MiddlePoint.y;
		OctNode->Boxes[6].BoundingBox.Min.z=OctNode->BoundingBox.Min.z;
		OctNode->Boxes[6].BoundingBox.Max.x=OctNode->BoundingBox.Max.x;
		OctNode->Boxes[6].BoundingBox.Max.y=OctNode->BoundingBox.Max.y;
		OctNode->Boxes[6].BoundingBox.Max.z=MiddlePoint.z;
		OctNode->Boxes[7].BoundingBox.Min=MiddlePoint; // Formar a caixa delimitadora do octante 8.
		OctNode->Boxes[7].BoundingBox.Max=OctNode->BoundingBox.Max;
	}
	for(i=0;i<8;i++){ // Para cada um destes 8 octantes criados...
		if( IS_POINT_INSIDE_BOX(Octree.Point,OctNode->Boxes[i].BoundingBox) ) { // Verificar se o objeto p está dentro do octante...
			OctNode->Boxes[i].PointHere=true; // Se sim, marcar este octante e chamar mais um nível de recursão.
			DeployOctree(&OctNode->Boxes[i],OctLevel-1,Octree);
		}else{
			OctNode->Boxes[i].PointHere=false; // Se não, ignorar este octante.
			OctNode->Boxes[i].Boxes=NULL;
		}
	}
}

bool FindCollisions(node OctNode,octree Octree){
	/*
	*  Está função percorre todos os octantes ao qual o objeto p faz parte e verifica se há algum triângulo interseccionando p.
	*
	*  Ela é recursiva e só vai parar até encontrar uma intersecção no nível final da octree ou
	*  até percorrer todos os octantes e nada for encontrado.
	*/
	if(OctNode.PointHere==false)	return false; // O objeto p não está nesse octante.
	int i;
	if(OctNode.Boxes==NULL){ // Chegou no fim da octree. Verificar se há algum triângulo aí que intersecciona junto com o objeto p.
		for(i=0;i<Octree.TrianglesC;i++){ // Para cada triângulo de toda a malha...
			POINT TriangleVertices[3]; // Criar os pontos no centro de sua aresta.
			TriangleVertices[0].x=(Octree.Triangles[i].Coord[0].x+Octree.Triangles[i].Coord[1].x)/(float)2;
			TriangleVertices[0].y=(Octree.Triangles[i].Coord[0].y+Octree.Triangles[i].Coord[1].y)/(float)2;
			TriangleVertices[0].z=(Octree.Triangles[i].Coord[0].z+Octree.Triangles[i].Coord[1].z)/(float)2;
			TriangleVertices[1].x=(Octree.Triangles[i].Coord[1].x+Octree.Triangles[i].Coord[2].x)/(float)2;
			TriangleVertices[1].y=(Octree.Triangles[i].Coord[1].y+Octree.Triangles[i].Coord[2].y)/(float)2;
			TriangleVertices[1].z=(Octree.Triangles[i].Coord[1].z+Octree.Triangles[i].Coord[2].z)/(float)2;
			TriangleVertices[2].x=(Octree.Triangles[i].Coord[2].x+Octree.Triangles[i].Coord[0].x)/(float)2;
			TriangleVertices[2].y=(Octree.Triangles[i].Coord[2].y+Octree.Triangles[i].Coord[0].y)/(float)2;
			TriangleVertices[2].z=(Octree.Triangles[i].Coord[2].z+Octree.Triangles[i].Coord[0].z)/(float)2;
			if(IS_POINT_INSIDE_BOX(TriangleVertices[0],OctNode.BoundingBox) ||  IS_POINT_INSIDE_BOX(TriangleVertices[1],OctNode.BoundingBox) ||
			IS_POINT_INSIDE_BOX(TriangleVertices[2],OctNode.BoundingBox) || IS_POINT_INSIDE_BOX(Octree.Triangles[i].Coord[0],OctNode.BoundingBox) || IS_POINT_INSIDE_BOX(Octree.Triangles[i].Coord[1],OctNode.BoundingBox) ||
			IS_POINT_INSIDE_BOX(Octree.Triangles[i].Coord[2],OctNode.BoundingBox) ){ // Verificar se os vértices do triângulo ou se algum dos pontos no centro de suas arestas está dentro do octante.
				return true; // Se sim, encerrar recursão porque já sabemos que há colisão. Se não, verificar próximo triângulo da malha.
			}
		}
		return false; // Nenhum triângulo aqui dentro interseccionando com o objeto p. Encerrar porque não há colisão.
	}
	for(i=0;i<8;i++) // Para cada um dos oito octantes do interior...
		if(FindCollisions(OctNode.Boxes[i],Octree)==true) // Verificar se há colisão.
			return true; // Se sim, encerrar recursão porque já sabemos que há colisão. Se não, verificar o próximo octante.
	return false; // Nenhum octante aqui dentro colide com o objeto p. Encerrar porque não há colisão.
}

void DestroyOctree(node *OctNode){
	/*
	*  Está função vai limpar da memória os octantes do interior de "OctNode" e o próprio "OctNode".
	*
	*  Ela é recursiva e só para quando tiver tudo limpo da memória no interior de "OctNode",
	*  limpando, por fim, o próprio vetor de octantes de "OctNode" da memória.
	*/
	if(OctNode==NULL || OctNode->Boxes==NULL)	return; // Este octante já está limpo.
	int i;
	for(i=0;i<8;i++) // Para cada octante do interior...
		DestroyOctree(&OctNode->Boxes[i]); // destruir tal octante.
	free(OctNode->Boxes); // Destruir o vetor principal de octantes.
}


