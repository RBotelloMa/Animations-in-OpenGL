#ifndef UGINE_SUBMESH_H
#define UGINE_SUBMESH_H

#include "types.h"
#include "vertex.h"
#include "texture.h"
#include "material.h"

class Submesh {
public:
	static std::shared_ptr<Submesh> Create(Material & material);
	void AddVertex(const Vertex& v);
	void AddTriangle(uint16 v0, uint16 v1, uint16 v2);
	//const std::shared_ptr<Texture>& GetTexture() const;
	//void SetTexture(const std::shared_ptr<Texture>& tex);
  
  Material& gMaterial();

	const std::vector<Vertex>& GetVertices() const;
	std::vector<Vertex>& GetVertices();
	void Rebuild();
	void Render();
	
  //practica 3
	//const glm::vec3& GetColor() const;
	//void SetColor(const glm::vec3& color);
	//uint8 GetShininess() const;
	//void SetShininess(uint8 shininess);
  //bool HayTextura();
	
protected:
	Submesh(Material& material);
	~Submesh();
	static void Delete(Submesh* s) { delete s; }
private:
	//std::shared_ptr<Texture> mTexture;
  Material mMaterial;
	uint32 mVertexBuffer;
	uint32 mIndexBuffer;
	std::vector<Vertex> mVertices;
	std::vector<uint16> mIndices;
	
  //practica 3
	//glm::vec3 mColor;
	//uint8 mShininess;
  //extra
  //bool hayTextura;
};

#endif // UGINE_SUBMESH_H