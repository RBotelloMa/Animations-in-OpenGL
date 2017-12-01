#include "../include/entity.h"
#include "../include/scene.h"

std::shared_ptr<Entity> Entity::Create() {
	return std::shared_ptr<Entity>(new Entity(), Delete);
}

const glm::vec3& Entity::GetPosition() const {
	return mPosition;
}

glm::vec3 & Entity::DefPosition() {
	return mPosition;
};

const glm::quat& Entity::GetRotation() const {
	return mRotation;
};
glm::quat& Entity::DefRotation() {
	return mRotation;
};
const glm::vec3& Entity::GetScale() const {
	return mScale;
};

glm::vec3& Entity::DefScale() {
	return mScale;
};

void Entity::Move(const glm::vec3& speed) {
	mPosition += mRotation*speed;
};

void Entity::Render() {
	float angle = 2 * glm::acos(mRotation.w);
	glm::mat4 rotation;
	if (angle != 0) {
		 rotation = glm::rotate(glm::mat4(1), angle, glm::vec3(
			mRotation.x / glm::sqrt(1 - mRotation.w*mRotation.w),
			mRotation.y / glm::sqrt(1 - mRotation.w*mRotation.w),
			mRotation.z / glm::sqrt(1 - mRotation.w*mRotation.w)
		));
	}
	glm::mat4 scale = glm::scale(glm::mat4(1), mScale);
	glm::mat4 translate = glm::translate(glm::mat4(1), mPosition);
	Scene::Instance()->SetModel(translate*rotation*scale);
	//Los hijos hacen más cosas
};

Entity::Entity() {};

	

/*private:
	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;
*/