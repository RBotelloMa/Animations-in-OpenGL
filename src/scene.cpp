#include "../include/scene.h"


std::shared_ptr<Scene> Scene::mInstance = nullptr;


const std::shared_ptr<Camera>& Scene::GetCurrentCamera() {
	return mCurrentCamera;
};

const glm::mat4& Scene::GetModel() const {
	return mModelMatrix;
};

void Scene::SetModel(const glm::mat4& m) {
	mModelMatrix = m;
	Renderer::Instance()->SetMatrices(mModelMatrix = m, mCurrentCamera->GetView(), mCurrentCamera->GetProjection());
}

void Scene::AddEntity(const std::shared_ptr<Entity>& entity) {
	mEntities.push_back(entity);
	if (std::dynamic_pointer_cast<Camera> (entity) != nullptr) {
		mCameras.push_back(std::dynamic_pointer_cast<Camera> (entity));
	}
	if (std::dynamic_pointer_cast<Light> (entity) != nullptr) {
		mLights.push_back(std::dynamic_pointer_cast<Light> (entity));
	}
};

void Scene::RemoveEntity(const std::shared_ptr<Entity>& entity) {
	mEntities.erase(std::find(mEntities.begin(),mEntities.end(),entity));
	if (std::dynamic_pointer_cast<Camera> (entity) != nullptr) {
		mCameras.erase(std::find(mCameras.begin(), mCameras.end(), entity));
	}
	if (std::dynamic_pointer_cast<Light> (entity) != nullptr) {
		mLights.erase(std::find(mLights.begin(), mLights.end(), entity));
	}
};
uint32 Scene::GetNumEntities() const {
	return mEntities.size();
};
const std::shared_ptr<Entity>& Scene::GetEntity(uint32 index) {
	return mEntities[index];
};

void Scene::Update(float elapsed) {
	for (auto it = mEntities.begin(); it != mEntities.end(); it++) {
		it->get()->Update(elapsed);
	}
};
void Scene::Render() {

	Renderer::Instance()->EnableLighting(true);
	Renderer::Instance()->SetAmbient(mAmbient);

	for (auto it = mCameras.begin(); it != mCameras.end(); it++) {
		mCurrentCamera = (*it);
		mCurrentCamera.get()->Prepare();
		for (auto it1 = mLights.begin(); it1 != mLights.end(); it1++) {
			(*it1)->Prepare();
		}
		for (auto it2 = mEntities.begin(); it2 != mEntities.end(); it2++) {
			it2->get()->Render();
		}
	}
	Renderer::Instance()->EnableLighting(false);

}

Scene::Scene() {
	Renderer::Instance()->Setup3D();
};


//practica 3
void Scene::SetAmbient(const glm::vec3& ambient) {
	mAmbient = ambient;
};




/*private:
	static std::shared_ptr<Scene> mInstance;
	std::shared_ptr<Camera> mCurrentCamera;
	glm::mat4 mModelMatrix;
	std::vector<std::shared_ptr<Camera>> mCameras;
	std::vector<std::shared_ptr<Entity>> mEntities;
*/