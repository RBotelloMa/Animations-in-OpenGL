#include "../include/resourceManager.h"





std::shared_ptr<ResourceManager> ResourceManager::mInstance = nullptr;


std::shared_ptr<Mesh> ResourceManager::LoadMesh(const std::string& filename) 
{ 
	//cambiar los return
	if (mMeshes.count(filename)==0) {
		std::shared_ptr<Mesh> aux2 = Mesh::Create(filename);//usar mismo aux
		if (aux2 != nullptr) {
			mMeshes.insert(std::pair<std::string, std::shared_ptr<Mesh>>(filename, aux2));
			return aux2;
		}
		else return nullptr;
	}
	else return mMeshes.find(filename)->second;
};

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& filename) { 
	
	//TODO: Testing this

	if (mTextures.count(filename) == 0) {
		std::shared_ptr<Texture> aux2 = Texture::Create(filename);//usar mismo aux
		if (aux2 != nullptr) {
			mTextures.insert(std::pair<std::string, std::shared_ptr<Texture>>(filename, aux2));
			return aux2;
		}
		else return nullptr;
	}
	else return mTextures.find(filename)->second;
};

void ResourceManager::FreeMeshes() { 
	mMeshes.clear();
};
void ResourceManager::FreeTextures() {
	mTextures.clear();
};
void ResourceManager::FreeResources() {
	mMeshes.clear();
	mTextures.clear();
};



	/*static std::shared_ptr<ResourceManager>mInstance;
	std::map<std::string, std::shared_ptr<Mesh>>mMeshes;
	std::map<std::string, std::shared_ptr<Texture>>mTextures;
	*/