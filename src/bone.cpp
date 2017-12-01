#include "../include/bone.h"



std::shared_ptr<Bone> Bone::Create(const std::string& name, int parentIndex) {
  return std::shared_ptr<Bone>(new Bone(name, parentIndex), Delete);
};
const std::string& Bone::GetName() const {
  return mName;
};
int Bone::GetParentIndex() const {
  return mParentIndex;
};
void Bone::SetInversePoseMatrix(const glm::mat4& matrix) {
  mInvPoseMatrix = matrix;
};
const glm::mat4& Bone::GetInversePoseMatrix() const {
  return mInvPoseMatrix;
};

glm::mat4 Bone::CalculateAnimMatrix(float frame) const{
  //TODO needs testing
  float TOL=0.000001f;
  glm::mat4 rotacion;
  glm::quat q = GetRotation(frame);
  if (abs(q.w-1.0f) <= TOL || abs(q.w + 1.0f) <= TOL) {
    rotacion = glm::mat4(1);
  }
  else {
    float sqw2, x, y, z, angle;
    sqw2 = glm::sqrt(1 - q.w * q.w);
    angle = 2.0f * glm::acos(q.w);
    x = q.x / sqw2;
    y = q.y / sqw2;
    z = q.z / sqw2;
    rotacion = glm::rotate(glm::mat4(1), angle, glm::vec3(x, y, z)); 
  }
  glm::mat4 escala = glm::scale(glm::mat4(1), GetScale(frame));
  glm::mat4 posicion = glm::translate(glm::mat4(1), GetPosition(frame));
  
  return posicion*rotacion*escala;
};


void Bone::AddPosition(uint32 frame, const glm::vec3& position) {
  mPositions.push_back(std::pair<uint32, glm::vec3>(frame, position));
};

void Bone::AddRotation(uint32 frame, const glm::quat& rotation) {
  mRotations.push_back(std::pair<uint32, glm::quat>(frame,rotation));
};

void Bone::AddScale(uint32 frame, const glm::vec3& scale) {
  mScales.push_back(std::pair<uint32, glm::vec3>(frame, scale));
};



Bone::Bone(const std::string& name, uint32 parentIndex) {
  mName = name;
  mParentIndex = parentIndex;
};

void Bone::Delete(Bone* b) {
  delete b;
};



glm::vec3 Bone::GetPosition(float frame) const {
  for (auto it = mPositions.begin(); it != mPositions.end(); it++) {
    if ((*it).first == frame) return (*it).second;
    else if ((*it).first > frame) {
      return glm::mix( (*(it - 1)).second , (*it).second , (frame - (*(it - 1)).first)/((*it).first - (*(it - 1)).first));
    }
  }
  if (mPositions.size() == 0) return glm::vec3();
  else return (*(mPositions.end() - 1)).second;
};


glm::quat Bone::GetRotation(float frame) const {
  for (auto it = mRotations.begin(); it != mRotations.end(); it++) {
    if ((*it).first == frame) return (*it).second;
    else if ((*it).first > frame) {
      glm::quat q1 = (*(it - 1)).second;
      glm::quat q2 = (*it).second;
      auto it2 = it - 1;
      float prop1 = (frame - (*(it - 1)).first);
      float prop2 = ((*it).first - (*(it - 1)).first);
      float prop = prop1 / prop2;
      glm::quat ret = glm::slerp(q1, q2, prop1/prop2);
      return ret;
    }
  }
  if (mRotations.size() == 0) return glm::quat();
  else return (*(mRotations.end() - 1)).second;
};


glm::vec3 Bone::GetScale(float frame) const {
  for (auto it = mScales.begin(); it != mScales.end(); it++) {
    if ((*it).first == frame) return (*it).second;
    else if ((*it).first > frame) {
      return glm::mix((*(it - 1)).second, (*it).second, (frame - (*(it - 1)).first) / ((*it).first - (*(it - 1)).first));
    }
  }
  if (mScales.size() == 0) return glm::vec3();
  else return (*(mScales.end() - 1)).second;
};