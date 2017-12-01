#ifndef UGINE_MODEL_H
#define UGINE_MODEL_H

#include "types.h"
#include "entity.h"
#include "mesh.h"

class Model : public Entity {
public:
  float TEST=1.0f;
	static std::shared_ptr<Model> Create(const std::shared_ptr<Mesh>&
		mesh);
  virtual void Update(float elapsed);
	virtual void Render();

  //practica 5
  void Animate(bool animate) {
    if (mAnimMatrices.size() > 0) mAnimating = animate;
  }
  int GetFPS() const { return mFPS; }
  void SetFPS(int fps) { mFPS = fps; }
  float GetCurrentFrame() const { return mCurrentFrame; }
  void SetCurrentFrame(float frame) { mCurrentFrame = frame; }

protected:
	Model(const std::shared_ptr<Mesh>& mesh);
	virtual ~Model() {}
private:
	std::shared_ptr<Mesh> mMesh;

  //practica 5
  bool mAnimating;
  int mFPS;
  float mCurrentFrame;
  std::vector<glm::mat4> mAnimMatrices;

};

#endif