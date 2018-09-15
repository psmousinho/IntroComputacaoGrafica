#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Pipeline {
	public:

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

		void setModel(glm::mat4 mat) {
			this->model = mat;
		}

		void setViewport(glm::mat4 mat) {
			this->viewport = mat;
		}

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
		
		glm::mat4 getModel() {
			return this->model;
		}

		glm::mat4 getView() {
			return this->view;
		}

		glm::mat4 getProjection() {
			return this->projection;
		}

		glm::mat4 getViewport() {
			return this->viewport;
		}

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

		void display(glm::mat4 mat) {
			printf("----------------------\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n----------------------", 
																	mat[0].x,mat[0].y,mat[0].z,mat[0].w,
																	mat[1].x,mat[1].y,mat[1].z,mat[1].w,
																	mat[2].x,mat[2].y,mat[2].z,mat[2].w,
																	mat[3].x,mat[3].y,mat[3].z,mat[3].w);															   	    
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

		std::vector<glm::vec4> transformMesh(std::vector<glm::vec4> vertices) {
			for(int i = 0; i < vertices.size(); i++) {
				vertices[i] = vertices[i] * getPipelineMatriz();
				vertices[i] = vertices[i] / vertices[i].w;
			}
			return vertices;
		}					
	
	private:
		glm::mat4 model = glm::mat4(1.0);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 projection = glm::mat4(1.0);
		glm::mat4 viewport = glm::mat4(1.0);
		glm::mat4 modelView = glm::mat4(1.0);
		glm::mat4 modelViewProjection = glm::mat4(1.0);
		glm::mat4 mPipeline = glm::mat4(1.0);
};	