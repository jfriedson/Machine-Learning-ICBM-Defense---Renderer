// this include
#include "model.h"

// std includes
#include <iostream>

// project includes
#include "vertex.h"


Model::Model(const char* modelFile, glm::vec3 pos, glm::vec3 org, glm::vec3 rot, glm::vec3 scale)
{
	this->ModelMatrix = glm::mat4(1.f);
	this->position = pos;
	this->origin = org;
	this->rotation = rot;
	this->scale = scale;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);



	if (!scene)
	{
		std::cout << "Model " << modelFile << " failed to load: " << importer.GetErrorString() << std::endl;
		return;
	}

	const aiMesh* model = scene->mMeshes[0];

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < model->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(model->mVertices[i]);
		const aiVector3D* pNormal = &(model->mNormals[i]);
		const aiVector3D* pTangent = &(model->mTangents[i]);
		const aiVector3D* pTexCoord = model->HasTextureCoords(0) ? &(model->mTextureCoords[0][i]) : &aiZeroVector;

		Vertex vert(glm::vec3(pPos->x, pPos->y, pPos->z),
			glm::vec2(pTexCoord->x, -pTexCoord->y),
			glm::vec3(pNormal->x, pNormal->y, pNormal->z),
			glm::vec3(pTangent->x, pTangent->y, pTangent->z));

		vertices.push_back(vert);
	}

	for (unsigned int i = 0; i < model->mNumFaces; i++)
	{
		const aiFace& face = model->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	mesh.LoadMesh(vertices, indices);

	/*
		aiMaterial* material = scene->mMaterials[model->mMaterialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Embedded textures
				if (auto texture = scene->GetEmbeddedTexture(path.C_Str()))
				{
					diffuseTex.LoadTexture(texture);
				}
				else
				{
					//int idx = std::string(path.data).rfind("/");
					//std::string filename = std::string(path.data).substr(idx + 1);

					std::string texPath = std::string("res/textures/") + (path.C_Str());

					diffuseTex.LoadTexture(texPath.c_str());
				}
			}
			else
			{
				diffuseTex.LoadTexture("res/textures/tile/tile_diff.jpg");
			}
		}
		else
		{
			diffuseTex.LoadTexture("res/textures/tile/tile_diff.jpg");
		}
	}
	else
	{
		struct stat buffer;

		if (diffFile != "")
			if (stat(diffFile, &buffer) == 0)
				diffuseTex.LoadTexture(diffFile);

		if (specFile != "")
			if (stat(specFile, &buffer) == 0)
				specularTex.LoadTexture(specFile);

		if (dispFile != "")
			if (stat(dispFile, &buffer) == 0)
				dispTex.LoadTexture(dispFile);
	}
	*/

	std::cout << "loaded model " << modelFile << std::endl;

	updateModelMatrix();
}

Model::~Model()
{

}


void Model::render(Shader* shader, bool tessellate)
{
	shader->setMat4fv(this->ModelMatrix, "ModelMatrix");

	shader->use();

	mesh.RenderMesh(tessellate);

	shader->unuse();
}


void Model::setPosition(glm::vec3 position)
{
	this->position = position;

	updateModelMatrix();
}

void Model::setOrigin(glm::vec3 origin)
{
	this->origin = origin;


	updateModelMatrix();
}

void Model::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;


	updateModelMatrix();
}

void Model::setScale(glm::vec3 scale)
{
	this->scale = scale;


	updateModelMatrix();
}


void Model::setModelMatrix(glm::mat4 modelMatrix)
{
	this->ModelMatrix = modelMatrix;
}

void Model::updateModelMatrix()
{
	this->ModelMatrix = glm::mat4(1.f);
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}