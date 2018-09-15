#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <bits/stdc++.h>
#include <glm/glm.hpp>

//---------------------------------------------------------------
void free_mem(void)
{
	std::clog << "Exiting...\n";
}
//---------------------------------------------------------------
void DrawAxis(void)
{
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0);
		glVertex3f( 0.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0);
		glVertex3f( 0.0f, 0.0f, 0.0f);
		glVertex3f( 0.0f, 1.f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0);
		glVertex3f( 0.0f, 0.0f, 0.0f);
		glVertex3f( 0.0f, 0.0f, 1.0f);

	glEnd();
}

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

//---------------------------------------------------------------
void display(void)
{

	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 512, 512);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(90.0f, 1.0f, 0.1f, 100.0f );
	gluLookAt(	0.0f, 3.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
		        0.0f, 1.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-1);
	//glScalef(3,0.5,-1);

	
	std::string file_name = "Modelos//Esfera.obj";
	std::vector<glm::vec4> vertices = loadMesh(file_name);
	
	for (int i = 0; i < vertices.size(); i+=3) {
		glBegin(GL_LINE_LOOP);
	        glColor3f(1.0f, 0.0f, 0.0f);
	        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

	        glColor3f(1.0f, 0.0f, 0.0f);
	        glVertex3f(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);

	        glColor3f(1.0f, 0.0f, 0.0f);
	        glVertex3f(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
	    glEnd();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

//---------------------------------------------------------------
int main(int argc, char **argv)
{
	std::clog << "Begin...\n";

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL: Pipeline Inspection");

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glutDisplayFunc(display);

	atexit(free_mem);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutMainLoop();

	return 0;
}