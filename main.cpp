#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{	

	//Cria o pipeline
	Pipeline pipeline = Pipeline(IMAGE_WIDTH,IMAGE_HEIGHT);
	//Configura a posicao, o lookAt, o up e a dstancial focal da camera
	pipeline.setCamera(glm::vec3(0,3,3),glm::vec3(0,0,0),glm::vec3(0,1,0),1);
	//lê o model
	std::string file_name = "Visualizacao no openGL//Modelos//Suzanne.obj";
	std::vector<glm::vec4> vertices = pipeline.loadMesh(file_name);
	//transformar o modelo para espaço de tela
	vertices = pipeline.transformMesh(vertices);
	//renderiza o modelo
	drawMesh(vertices);
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}
