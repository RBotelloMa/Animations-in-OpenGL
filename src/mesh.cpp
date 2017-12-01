#include "../include/mesh.h"
#include "../include/material.h"


std::shared_ptr<Mesh> Mesh::Create() {
	return std::shared_ptr<Mesh>(new Mesh(), Delete);
};

std::shared_ptr<Mesh> Mesh::Create(const std::string& filename) {
	return std::shared_ptr<Mesh>(new Mesh(filename), Delete);

};

const std::string& Mesh::GetFilename() const {
	return mFilename;
};

void Mesh::AddSubmesh(const std::shared_ptr<Submesh>& submesh) {
	mSubmeshes.push_back(submesh);
	submesh->Rebuild();
};

void Mesh::RemoveSubmesh(uint32 i) {
	mSubmeshes.erase(mSubmeshes.begin() + i);
};

void Mesh::RemoveSubmesh(const std::shared_ptr<Submesh>& submesh) {
	mSubmeshes.erase(std::find(mSubmeshes.begin(), mSubmeshes.end(), submesh));
};

uint32 Mesh::NumSubmeshes() const {
	return mSubmeshes.size();
};

const std::shared_ptr<Submesh>& Mesh::GetSubmesh(uint32 i) {
	return mSubmeshes[i];
};

void Mesh::Render() {
	
	for (auto it = mSubmeshes.begin(); it != mSubmeshes.end();++it) {
		
		it->get()->Render();
		
	}
};


Mesh::Mesh() {};

Mesh::Mesh(const std::string& filename) {
	mFilename = filename;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(mFilename.c_str());
	if (result) {
		pugi::xml_node meshNode = doc.child("mesh");
//Material
    pugi::xml_node materialsNode = doc.child("mesh").child("materials");
    pugi::xml_node materialNode = materialsNode.child("material");
    Material material;

    
    if (materialNode!=pugi::xml_node()) {
        //textura
      pugi::xml_node auxNode;
      auxNode = materialNode.child("texture");
      std::vector<std::string> textureStr = SplitString(materialNode.child("texture").text().as_string(), ',');
      std::string path = ExtractPath(filename);

      if (materialNode.child("texture") == pugi::xml_node()) {
        material = Material::Material();
      }
      else if (textureStr.size() == 0) {
        material = Material::Material();
      }
      else if (textureStr.size() == 1) {
        std::shared_ptr<Texture> textura = Texture::Create(path + "/" + textureStr[0]);
        material = Material::Material(textura);
      }
      else if (textureStr.size() == 6) {
        std::shared_ptr<Texture> textura = Texture::Create(path + "/" + textureStr[0], path + "/" + textureStr[1],
          path + "/" + textureStr[2], path + "/" + textureStr[3],
          path + "/" + textureStr[4], path + "/" + textureStr[5]);
        material = Material::Material(textura);
      }
      else {
        material = Material::Material();
      }

      //blend
      std::vector<std::string> blendStr = SplitString(materialNode.child("blend").text().as_string(), ',');

      if (materialNode.child("blend") == pugi::xml_node()) {
        material.SetBlendMode(Renderer::BlendMode::ALPHA);
      }
      else if (blendStr.size() == 0) {
        material.SetBlendMode(Renderer::BlendMode::ALPHA);
      }
      else {
        if (blendStr[0] == "alpha") {
          material.SetBlendMode(Renderer::BlendMode::ALPHA);
        }
        else if (blendStr[0] == "add") {
          material.SetBlendMode(Renderer::BlendMode::ADDITIVE);
        }
        else {
          material.SetBlendMode(Renderer::BlendMode::MULTIPLICATIVE);
        }
      }

      //diffuse
      std::vector<std::string> diffuseStr = SplitString(materialNode.child("diffuse").text().as_string(), ',');
      material.SetBaseColor(glm::vec4(NumberFromString<float>(diffuseStr[0]),
        NumberFromString<float>(diffuseStr[1]),
        NumberFromString<float>(diffuseStr[2]),
        NumberFromString<float>(diffuseStr[3])));

      //normal_texture
      std::vector<std::string> normalTextureStr = SplitString(materialNode.child("normal_texture").text().as_string(), ',');


      if (materialNode.child("normal_texture") == pugi::xml_node()) {
      }
      else if (normalTextureStr.size() == 0) {
      }
      else {
        std::shared_ptr<Texture> textura = Texture::Create(path + "/" + normalTextureStr[0]);
        material.SetNormalTexture(textura);
      }

      //reflect_texture
      std::vector<std::string> reflectTextureStr = SplitString(materialNode.child("reflect_texture").text().as_string(), ',');
      
      if (materialNode.child("reflect_texture") == pugi::xml_node()) {
      }
      else if (reflectTextureStr.size() == 0) {
      }
      else {
        std::shared_ptr<Texture> textura = Texture::Create(path + "/" + reflectTextureStr[0], path + "/" + reflectTextureStr[1],
          path + "/" + reflectTextureStr[2], path + "/" + reflectTextureStr[3],
          path + "/" + reflectTextureStr[4], path + "/" + reflectTextureStr[5]);

        material.SetReflectionTexture(textura);
      }


      //refract_texture
      std::vector<std::string> refractTextureStr = SplitString(materialNode.child("refract_texture").text().as_string(), ',');

      if (materialNode.child("refract_texture") == pugi::xml_node()) {
      }
      else if (refractTextureStr.size() == 0) {

      }
      else {
        std::shared_ptr<Texture> textura = Texture::Create(path + "/" + refractTextureStr[0], path + "/" + refractTextureStr[1],
          path + "/" + refractTextureStr[2], path + "/" + refractTextureStr[3],
          path + "/" + refractTextureStr[4], path + "/" + refractTextureStr[5]);

        material.SetRefractionTexture(textura);
      }

      //refract_coef
      std::vector<std::string> refractCoefStr = SplitString(materialNode.child("refract_coef").text().as_string(), ',');


      if (materialNode.child("refract_coef") == pugi::xml_node()) {
        material.SetRefractionCoef(1);
      }
      else if (refractCoefStr.size() == 0) {
        material.SetRefractionCoef(1);
      }
      else {
        material.SetRefractionCoef(NumberFromString<float>(refractCoefStr[0]));
      }

      //shininess
      std::vector<std::string> shininessStr = SplitString(materialNode.child("shininess").text().as_string(), ',');

      if (materialNode.child("shininess") == pugi::xml_node()) {
        material.SetShininess(0);
      }
      if (shininessStr.size() == 0) {
        material.SetShininess(255);
      }
      else {
        material.SetShininess(NumberFromString<uint8>(shininessStr[0]));
      }

      //culling
      std::vector<std::string> cullingStr = SplitString(materialNode.child("culling").text().as_string(), ',');

      if (materialNode.child("culling") == pugi::xml_node()) {
        material.SetCulling(true);
      }
      if (cullingStr.size() == 0) {
        material.SetCulling(true);
      }
      else {
        material.SetCulling(NumberFromString<bool>(cullingStr[0]));
      }

      //depthwrite
      std::vector<std::string> depthwriteStr = SplitString(materialNode.child("depthwrite").text().as_string(), ',');


      if (materialNode.child("depthwrite") == pugi::xml_node()) {
        material.SetDepthWrite(true);
      }
      if (depthwriteStr.size() == 0) {
        material.SetDepthWrite(true);
      }
      else {
        material.SetDepthWrite(NumberFromString<bool>(depthwriteStr[0]));
      }
    }
 
//submeshes
		pugi::xml_node submeshesNode = doc.child("mesh").child("submeshes");
		for (pugi::xml_node submeshNode = submeshesNode.child("submesh");
			submeshNode;
			submeshNode = submeshNode.next_sibling("submesh"))
			{
        std::shared_ptr<Submesh> subMesh;
				// Iteramos por todos los elementos “submesh” contenidosen el elemento “submeshes”
				//material
        if (materialNode == pugi::xml_node()) {
          std::string textureStr = submeshNode.child("texture").text().as_string();
          std::string path = ExtractPath(filename);
          std::shared_ptr<Texture> textura = Texture::Create(path + "/" + textureStr);
          Material newMaterial(textura);
          newMaterial.SetBaseColor(glm::vec4(1, 1, 1, 1));
          newMaterial.SetShininess(255);
        
          subMesh = Submesh::Create(newMaterial);
        }
        else {
          subMesh = Submesh::Create(material);
        }
       
				//indices
					
				std::vector<std::string> indicesVec = SplitString(submeshNode.child("indices").text().as_string(), ',');
				for (int i = 0; i < indicesVec.size(); i = (i + 3)) {
					uint16 indice0 = NumberFromString<uint16>(indicesVec[i]);
					uint16 indice1 = NumberFromString<uint16>(indicesVec[i + 1]);
					uint16 indice2 = NumberFromString<uint16>(indicesVec[i + 2]);
					subMesh->AddTriangle(indice0, indice1, indice2);
				}
				//coords 
				
        std::vector<std::string> coordsVec = SplitString(submeshNode.child("coords").text().as_string(), ',');
				std::vector<std::string> texcoordsVec = SplitString(submeshNode.child("texcoords").text().as_string(), ',');
				std::vector<std::string> normsVec = SplitString(submeshNode.child("normals").text().as_string(), ',');
        std::vector<std::string> boneIndex = SplitString(submeshNode.child("bone_indices").text().as_string(), ',');
        std::vector<std::string> boneWeights = SplitString(submeshNode.child("bone_weights").text().as_string(), ',');
        std::vector<std::string> tangents = SplitString(submeshNode.child("tangents").text().as_string(), ',');
				
        int n1 = coordsVec.size()/3;
				int n2 = texcoordsVec.size() / 2;
				int n3 = normsVec.size() / 3;
        int n4 = boneIndex.size() / 4;
        int n5 = boneWeights.size() / 4;
        int n6 = tangents.size() / 3;
				int maxim = std::max(std::max(std::max(std::max(n1, n2), std::max(n3, n2)),n5),n6);
				
        for (int i = 0; i < maxim; i++) {
					float coord0, coord1, coord2;
					float texcoord0, texcoord1;
					float norm0, norm1, norm2;
          float boneIdnx0, boneIdnx1, boneIdnx2, boneIdnx3;
          float boneW0, boneW1, boneW2, boneW3;
          float tang0, tang1, tang2;
					
          if (i < n1) {
						coord0 = NumberFromString<float>(coordsVec[3 * i]);
						coord1 = NumberFromString<float>(coordsVec[3 * i + 1]);
						coord2 = NumberFromString<float>(coordsVec[3 * i + 2]);
					}
					else {
						coord0 = coord1 = coord2 = 0;
					}

					if (i < n2) {
						texcoord0 = NumberFromString<float>(texcoordsVec[2 * i]);
						texcoord1 = NumberFromString<float>(texcoordsVec[2 * i + 1]);
					}
					else {
            texcoord0 = i;
            texcoord1 = 0;
					}

					if (i < n3) {
						norm0 = NumberFromString<float>(normsVec[3 * i]);
						norm1 = NumberFromString<float>(normsVec[3 * i + 1]);
						norm2 = NumberFromString<float>(normsVec[3 * i + 2]);
					}
					else {
						norm0 = norm1 = norm2 = 0;
					}

          if (i < n4) {
            boneIdnx0 = NumberFromString<float>(boneIndex[4 * i]);
            boneIdnx1 = NumberFromString<float>(boneIndex[4 * i+1]);
            boneIdnx2 = NumberFromString<float>(boneIndex[4 * i+2]);
            boneIdnx3 = NumberFromString<float>(boneIndex[4 * i+3]);
          }
          else {
            boneIdnx0 = boneIdnx1 = boneIdnx2 = boneIdnx3 = -1;
          }

          if (i < n5) {
            boneW0 = NumberFromString<float>(boneWeights[4 * i]);
            boneW1 = NumberFromString<float>(boneWeights[4 * i + 1]);
            boneW2 = NumberFromString<float>(boneWeights[4 * i + 2]);
            boneW3 = NumberFromString<float>(boneWeights[4 * i + 3]);
          }
          else {
            boneW0 = boneW1 = boneW2 = boneW3 = 0;
          }
          if (i < n6) {
            
            tang0 = NumberFromString<float>(tangents[3 * i]);
            tang1 = NumberFromString<float>(tangents[3 * i + 1]);
            tang2 = NumberFromString<float>(tangents[3 * i + 2]);
          }
          else {
            tang0 = tang1 = tang2 = 0;
          }


					subMesh.get()->AddVertex(Vertex(
            glm::vec3(coord0,coord1,coord2), 
            glm::vec2(texcoord0, texcoord1), 
            glm::vec3(norm0, norm1, norm2),
            glm::vec4(boneIdnx0,boneIdnx1,boneIdnx2,boneIdnx3),
            glm::vec4(boneW0,boneW1,boneW2,boneW3),
            glm::vec3(tang0, tang1, tang2)));
				}
				AddSubmesh(subMesh);
			}

   //practica 5
   pugi::xml_node lastFrameNode = doc.child("mesh").child("last_frame");
   mLastFrame = lastFrameNode.text().as_int();
   
   pugi::xml_node bonesNode = doc.child("mesh").child("bones");
   for (pugi::xml_node boneNode = bonesNode.child("bone");
      boneNode;
      boneNode = boneNode.next_sibling("bone"))
    {
      std::string boneName = boneNode.child("name").text().as_string();
      std::string dadName = boneNode.child("parent").text().as_string();
      int dadId = GetBoneIndex(dadName);
      std::shared_ptr<Bone> bone = Bone::Create(boneName, dadId);
      
      //inv pose
      std::vector<std::string> invPoseVec = SplitString(boneNode.child("inv_pose").text().as_string(), ',');
      glm::mat4 InvP;
      for (int i = 0; i < invPoseVec.size()/4; i = i++) {
        InvP[i][0] = NumberFromString<float>(invPoseVec[4*i]);
        InvP[i][1] = NumberFromString<float>(invPoseVec[4 * i+1]);
        InvP[i][2] = NumberFromString<float>(invPoseVec[4 * i+2]);
        InvP[i][3] = NumberFromString<float>(invPoseVec[4 * i+3]);
      }
      bone->SetInversePoseMatrix(InvP);
      
      //positions
      std::vector<std::string> positionsVec = SplitString(boneNode.child("positions").text().as_string(), ',');
      for (int i = 0; i < positionsVec.size(); i = (i + 4)) {
        int f = NumberFromString<float>(positionsVec[i]);
        glm::vec3 pos = glm::vec3(NumberFromString<float>(positionsVec[i + 1]), NumberFromString<float>(positionsVec[i + 2]), NumberFromString<float>(positionsVec[i + 3]));
        bone->AddPosition(f, pos);
      }

      //rotations
      std::vector<std::string> rotationsVec = SplitString(boneNode.child("rotations").text().as_string(), ',');
      for (int i = 0; i < rotationsVec.size(); i = (i + 5)) {
        int f = NumberFromString<float>(rotationsVec[i]);
        glm::quat rot = glm::quat(NumberFromString<float>(rotationsVec[i + 1]), NumberFromString<float>(rotationsVec[i + 2]), NumberFromString<float>(rotationsVec[i + 3]), NumberFromString<float>(rotationsVec[i + 4]));
        bone->AddRotation(f, rot);
      }

      //scale
      std::vector<std::string> scalesVec = SplitString(boneNode.child("scales").text().as_string(), ',');
      for (int i = 0; i < scalesVec.size(); i = (i + 4)) {
        int f = NumberFromString<float>(scalesVec[i]);
        glm::vec3 scl = glm::vec3(NumberFromString<float>(scalesVec[i + 1]), NumberFromString<float>(scalesVec[i + 2]), NumberFromString<float>(scalesVec[i + 3]));
        bone->AddScale(f, scl);
      }

      AddBone(bone);
    }
	}
};

//practica 5

void Mesh::CalculateAnimMatrices(float frame, std::vector<glm::mat4>& animMatrices) {
  for (int i = 0; i < mBones.size(); ++i) {
    int dad = mBones[i]->GetParentIndex();
    if (dad == -1) animMatrices[i] = mBones[i]->CalculateAnimMatrix(frame);
    else animMatrices[i] = animMatrices[dad] * mBones[i]->CalculateAnimMatrix(frame);
  }
  for (int i = 0; i< animMatrices.size(); ++i) {
    animMatrices[i] = animMatrices[i] * mBones[i]->GetInversePoseMatrix();
  }
};


int Mesh::GetBoneIndex(const std::string& name) {
  int i;
  for (i = 0; i < mBones.size(); ++i) {
    if (mBones[i]->GetName() == name) return i;
  }
  return -1;
};


/*private:
	std::string mFilename;
	std::vector<std::shared_ptr<Submesh>> mSubmeshes;
*/