#ifndef UGINE_VERTEX_H
#define UGINE_VERTEX_H

#include "types.h"

struct Vertex {
	glm::vec3 mPosition;
	glm::vec2 mTexture;
	glm::vec3 mNormal;
  glm::vec4 mBoneIndices;
  glm::vec4 mBoneWeights;
  glm::vec3 mTangent;
	Vertex(const glm::vec3& position, const glm::vec2& texture, const glm::vec3& normal,const  glm::vec4& boneIndices, const glm::vec4 & boneWeights, const glm::vec3 & tangent) :
		mPosition(position),
		mTexture(texture), 
		mNormal(normal),
    mBoneIndices(boneIndices),
    mBoneWeights(boneWeights),
    mTangent(tangent){};
};

#endif // UGINE_VERTEX_H
