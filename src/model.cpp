#include "../include/model.h"
#include "../include/renderer.h"


std::shared_ptr<Model> Model::Create(const std::shared_ptr<Mesh>& mesh) {
	if (mesh == nullptr) return nullptr;
	else return std::shared_ptr<Model>(new Model(mesh), Delete);
};


void Model::Render() {
	Entity::Render();
    Renderer::Instance()->Renderer::SetSkinned(mAnimating);
    if (mAnimating) {
      Renderer::Instance()->Renderer::SetAnimMatrices(mAnimMatrices);
    }
  mMesh->Render();
}

Model::Model(const std::shared_ptr<Mesh>& mesh) {
	mMesh = mesh;
  mAnimating = false;
  mFPS = 20;
  mCurrentFrame = 0;
  for (auto it = mMesh->GetBones().begin(); it != mMesh->GetBones().end(); it++) {
    mAnimMatrices.push_back(glm::mat4(1));
  }
};

//practica 5

void Model::Update(float elapsed) {
  mCurrentFrame += mFPS*elapsed;
  TEST = 0;
  if (mCurrentFrame > mMesh->GetLastFrame()) mCurrentFrame = 0;
  else if (mCurrentFrame < 0) mCurrentFrame = mMesh->GetLastFrame();
  mMesh->CalculateAnimMatrices(mCurrentFrame, mAnimMatrices);
}

	/*private:
	std::shared_ptr<Mesh> mMesh;
  bool mAnimating;
  int mFPS;
  float mCurrentFrame;
  std::vector<glm::mat4> mAnimMatrices;
*/

