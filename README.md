# Introdução À Computação Gráfica 2018.1 


#### Conteudo:
[1. Primitivas](https://github.com/psmousinho/IntroComputacaoGrafica#primitivas) <br>
[2. Modelos](https://github.com/psmousinho/IntroComputacaoGrafica#modelos) <br>


### Introdução:
O objetivo da primeira parte deste projeto é a rasterização de primitivas usando o algoritmo de Brasenham. Para esta tarefa utilizamos um framework desenvolvido pelo Prof. Christian Pagot que emula o acesso direto a memória de vídeo do computador via o ponteiro FBptr. Na segunda parte deste projeto, emulamos o pipleline gráfico para podermos rasterizar modelos mais complexos utilizando as primitivas ja desenvolvidas. 

## Primitivas:
### Rasterização de um ponto:
Desenvolvemos uma função chamada putPixel que recebe como parâmetro  dois inteiros representando as coordenadas de um pixel e um array de quatro inteiros que representam os valore de RGBA que queremos representar nessas coordenadas.

```C++
void putPixel(int x, int y, color c) {
	FBptr[x*4 + y*4*IMAGE_WIDTH] = c[0];
	FBptr[x*4 + y*4*IMAGE_WIDTH + 1] = c[1];
	FBptr[x*4 + y*4*IMAGE_WIDTH + 2] = c[2];
	FBptr[x*4 + y*4*IMAGE_WIDTH + 3] = c[3];
}
```
Cada pixel é representado por quatro bytes onde cada byte especifíca uma intensidade de uma das cores RGBA nesta ordem. Como esses bytes estão armazenados linearmente na memória de vídeo, cada pixel  está disposto quatro posições a frente do pixel anterior. A posição inicial de cada pixel é dada pela função Offset = 4*x + 4*y*w onde x e y são as coordenadas do pixel e w é o comprimento da tela.


### Rasterização de uma linha:
Para rasterizar linhas, utilizamos o algoritmo de Bresenham generalizando-o para todos octantes do plano cartesiano.

#### Algoritmo de Breseham:
Esse algoritmo nos da uma função incremental para a rasterização de uma reta, mas em sua forma natural ele é limitado ao primeiro octante do plano cartesiano(retas com inclinação entre 0 e 1). Ele é implementado em código da seguinte forma:

```C++
void drawLine(pixel i,pixel f, color c) {
	// Coordenadas iniciais
	int x = i.x;
	int y = i.y;
	//Deslocamentos
	int dx = f.x - i.x;
	int dy = f.y - i.y;
	
	int d = 2*dy - dx;
	int e_inc = 2*dy;
	int ne_inc = 2*(dy - dx);

	putPixel(i,c);			
	while(x < f.x) {
		if(d <= 0){
			x++;
			d += e_inc;			
		}
		else{
			x++;
			y++;
			d += ne_inc;			
		}
		putPixel(x,y,c);
	}
}
```

#### Como foi feito a generalização:

Para podermos desenhar retas em todos os octantes, precisamos fazer algumas observações:

![alt text](https://github.com/avillasilva/CG/raw/master/Fotos/octantes.png "octantes")

Podemos observar que se invertermos o ponto inicial e ponto final de retas que se encontram no 5° octante teremos retas que se enquadram no 1° octante. Isso também ocorre entre o 3° e 7°, 4° e 8° , 6° e 2° octantes. Devido a isso basta implementar os casos de retas nos 1°, 2°, 7° e 8° octantes, todos os outros casos podem ser feitos por espelhamento.

Em sua Forma final, a função drawLine ficou assim:

```C++
void drawLine(pixel i,pixel f, color c) {
	// Coordenadas iniciais
	int x = i.x;
	int y = i.y;
	//Deslocamentos
	int dx = f.x - i.x;
	int dy = f.y - i.y;
	
	if(abs(dx) > abs(dy)){
		//1 Octante
		if(dx > 0 && dy > 0) {
			int d = 2*dy - dx;
			int e_inc = 2*dy;
			int ne_inc = 2*(dy - dx);
			putPixel(i,c);			
			while(x < f.x){
				if(d <= 0){
					x++;
					d += e_inc;			
				}
				else{
					x++;
					y++;
					d += ne_inc;			
				}
				putPixel(x,y,c);
			}
		}
		//4 e 5 Octantes 
		else if(dx < 0) {
			drawLine(f,i,c);
		}
		//8 Octante e Linhas Horizontais
		else {
			int d = 2*dy + dx;
			int e_inc = 2*dy;
			int se_inc = 2*(dy + dx);
			putPixel(i,c);			
			while(x < f.x){
				if(d <= 0){
					x++;
					y--;
					d += se_inc;			
				}
				else{
					x++;
					d += e_inc;			
				}
				putPixel(x,y,c);
			}
		}
	}
	else {
		//7 Octante
		if(dx > 0 && dy < 0) {
			int d = dy + 2*dx;
			int s_inc =  2*dx;
			int se_inc = 2*(dy + dx);
			putPixel(x,y,c);
			while(y > f.y){
				if(d <= 0){
					y--;
					d += s_inc;
				}
				else {
					x++;
					y--;
					d += se_inc;
				}
				putPixel(x,y,c);
			}
		}
		//3 e 6 Octantes
		else if(dx < 0 || (i.x == f.x && i.y > f.y)){
			drawLine(f,i,c);
		}
		//2 Octante e linhas verticais
		else {
			int d = dy + 2*-dx;
			int n_inc =  2*-dx;
			int ne_inc = 2*(dy - dx);
			
			putPixel(x,y,c);
			while(y < f.y){
				if(d <= 0){
					y++;
					x++;
					d += ne_inc;
				}
				else {
					y++;
					d += n_inc;
				}
				putPixel(x,y,c);
			}	
		}

	} 
}
```

### Raterização de Triângulos:
Para rasterizar essa primitiva, apenas desenhamos três retas ligando três pontos recebidos como parametro com a cor requisitada.

```C++
	void drawTriangle(pixel v1,pixel v2, pixel v3, color c) {
	drawLine(v1,v2,c);
	drawLine(v1,v3,c);
	drawLine(v2,v3,c);
}
```

### Interpolação de cores:
A interpolção de cores foi feita a parti de uma nova função que define a intensidade da cor inicial e da cor final num dado ponto  baseada num fator de porcentagem (distancia parcial percorrida entre ponto atual e ponto final divido pela distancia total).
```C++
void interpolar(float per,color atual, color inicial, color fin) {
	for(int i = 0; i < 4; i++) {
		atual[i] = (per * inicial[i]) + ((1-per) * fin[i]);	
	}
} 
```

Esta função é chamada dentro da função drawLine antes do posicionamento de cada pixel para definir a cor deste pixel;
```C++
...
interpolar(per,atual,cInicial,cFinal);
putPixel(x,y,atual);
...
```

### Resultados:
Os resultados desse projeto podem ser vistos nas imagens abaixo. Os maiores desafios desse projeto foram o desenvolvimento matematico da generalização do algoritmo de Breseham e o algoritmo de interpolação de cores.

![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/retas.png "Retas")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/triangulo.png "Triângulo")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/reatasinter.png "Retas interpoladas")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/triangulointer.png "Triângulo interpolado")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/demo.png "Demostração")

## Modelos:

### Pipeline Gráfico: 
O pipeline gráfico é uma sequência de transformações geométricas que levam a descrição matemática de um modelo do espaço onde ele é desenvolvido (espaco do objeto) para o espaço que representa a tela do computador(espaço de tela). Nosso pipeline é composto de 4 multiplicações por matrizes e uma divisão por escalar. Esse pipeline foi implementado dentro de uma classe. Dentro desta classe temos métodos para Configurar e acessar as 4 matrizes utilizadas no pipeline.

#### Matriz model:
essa matriz é responsável por levar a descrição do objeto do espaço do objeto paras o espaço do mundo onde são colocados todos os  objetos que queremos dentro de uma cena. Essa matriz pode ser acessada e modificada pelos métodos a seguir:

```C++
void setModel(glm::mat4 mat) {
	this->model = mat;
}

glm::mat4 getModel() {
	return this->model;
}
```

#### Matrizes view e projection:
Essas matrizes têm haver com como iremos observar a cena. A matriz view é definida pela posição da câmera, pela direção em que ela está olhando e qual a direção de cima dela. A matriz projection é a responsável por criar a ilusão de perspectiva,” o que está mais perto se torna maior e o que está mais longe se torna menor”, Ela é definida pela distância focal da câmera. Essas duas matrizes podem ser configuradas e acessadas pelos métodos a seguir:

```C++
void setCamera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, float focalDistance) {
	glm::vec3 Z_axis = -(lookAt - position);
	Z_axis = glm::normalize(Z_axis);

	glm::vec3 X_axis = glm::cross(up,Z_axis);
	X_axis = glm::normalize(X_axis);

	glm::vec3 Y_axis = glm::cross(Z_axis,X_axis);
	Y_axis = glm::normalize(Y_axis);

	this->view[0] = glm::vec4(X_axis, dot(X_axis,-position));
	this->view[1] = glm::vec4(Y_axis, dot(Y_axis,-position));
	this->view[2] = glm::vec4(Z_axis, dot(Z_axis,-position));;
	this->view[3] = glm::vec4(0,0,0,1);

	this->projection[0] = glm::vec4(1,0,0,0);
	this->projection[1] = glm::vec4(0,1,0,0);
	this->projection[2] = glm::vec4(0,0,1,focalDistance);
	this->projection[3] = glm::vec4(0,0,-(1/focalDistance),0);
}

glm::mat4 getView() {
	return this->view;
}

glm::mat4 getProjection() {
	return this->projection;
}
```

#### Matriz viewport:
Essa matriz é responsável por mapear a cena na tela do computador. Ela é definida pela largura e altura da tela. Ela pode ser configurada durante a criação do objeto pipeline ou pelo metodo a seguir:

```C++
Pipeline(int width, int height) {
	glm::mat4 S1 = glm::mat4(1.0);
	S1[1] = glm::vec4(0,-1,0,0);

	glm::mat4 T = glm::mat4(1.0);
	T[0] = glm::vec4(1,0,0,1);
	T[1] = glm::vec4(0,1,0,1);

	glm::mat4 S2 = glm::mat4(1.0);
	S2[0] = glm::vec4((width-1)/2,0,0,0);
	S2[1] = glm::vec4(0,(height-1)/2,0,0);

	this->viewport = S1 * T * S2 ;
}

void setViewport(glm::mat4 mat) {
	this->viewport = mat;
}

glm::mat4 getViewport() {
	return this->viewport;
}
```

#### Composição de transformações:
Devido a natureza das matrizes, podemos comprimir essas transformações em apenas uma matriz de forma que o resultado permanece o mesmo. A seguir os métodos que retornam essas compreensões:
```C++
glm::mat4 getModelView() {
	glm::mat4 MV = this-> model * this->view;
	return MV;
}

glm::mat4 getModelViewProjection() {
	glm::mat4 MVP = this->model * this->view * this->projection ;
	return MVP;
}

glm::mat4 getPipelineMatriz() {
	glm::mat4 MVPV = this->model * this->view * this->projection * this->viewport;
	return MVPV;
}
````

#### Carregar modelos e aplicar o pipeline :
Dentro da classe pipeline foi desenvolvido um  método que lê uma descrição de um modelo a partir de um arquivo externo com ajuda da biblioteca assimp e um método que recebe uma descrição e implementa todas as transformações do pipeline:
```C++
std::vector<glm::vec4> loadMesh(const std::string &file_name ) {
	std::ifstream fin(file_name.c_str() );
	std::vector<glm::vec4> vertices(0);

	if( !fin.fail() )
		fin.close();
	else {
		std::cerr << "Couldn't open file: " << file_name << std::endl;
		return vertices;
	}

	Assimp::Importer assimp_importer;
	const aiScene* assimp_scene_ = assimp_importer.ReadFile( file_name, aiProcess_Triangulate);

	if( !assimp_scene_ ) {
		std::cerr << assimp_importer.GetErrorString() << std::endl;
		return vertices;
	}

	if( assimp_scene_->HasMeshes() ) {

		for( unsigned int mesh_id = 0; mesh_id < assimp_scene_->mNumMeshes; mesh_id++ ) {
			const aiMesh *mesh_ptr = assimp_scene_->mMeshes[mesh_id];

			for( unsigned int vertex_id = 0; vertex_id < mesh_ptr->mNumVertices; vertex_id += 3 ) {
				const aiVector3D *vertex_ptr = &mesh_ptr->mVertices[vertex_id];

				glm::vec4 v0( vertex_ptr[0].x, vertex_ptr[0].y, vertex_ptr[0].z,1 );
				glm::vec4 v1( vertex_ptr[1].x, vertex_ptr[1].y, vertex_ptr[1].z,1 );
				glm::vec4 v2( vertex_ptr[2].x, vertex_ptr[2].y, vertex_ptr[2].z,1 );

				vertices.push_back(v0);
				vertices.push_back(v1);
				vertices.push_back(v2);
			}

		}

	}

	return vertices;
}

std::vector<glm::vec4> transformMesh(std::vector<glm::vec4> vertices) {
	for(int i = 0; i < vertices.size(); i++) {
		vertices[i] = vertices[i] * getPipelineMatriz();
		vertices[i] = vertices[i] / vertices[i].w;
	}
	return vertices;
}
```

### Resultados:
Aqui podemos ver os resultados do nosso pipeline em comparação com a rasterização do openGl. Nossos modelos estão em azul e os do openGl em vermelho.

![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/SuzannePOV1.png "Suzanne POV 2")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/SuzannePOV2.png "Suzzanne POV 2")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/SuzanneEscala.png "Suzanne Escalada ")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/EsferaPOV1 "Esfera POV 1")
![alt text](https://github.com/psmousinho/IntroComputacaoGrafica/raw/master/Fotos/EsferaPOV2.png "Esfera POV 2")



## Bibliografia:
[1] https://medium.com/@biancaamoriim/rasteriza%C3%A7%C3%A3o-de-primitivas-em-opengl-8680a76fdda5
