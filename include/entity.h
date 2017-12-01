#ifndef UGINE_ENTITY_H
#define UGINE_ENTITY_H

#include "types.h"


class Entity {
public:
	static std::shared_ptr<Entity> Create();
	const glm::vec3& GetPosition() const;
	glm::vec3& DefPosition();
	const glm::quat& GetRotation() const;
	glm::quat& DefRotation();
	const glm::vec3& GetScale() const;
	glm::vec3& DefScale();
	void Move(const glm::vec3& speed);
	virtual void Update(float elapsed) {}
	virtual void Render();
protected:
	Entity();
	virtual ~Entity() {}
	static void Delete(Entity* e) { delete e; }
private:
	glm::vec3 mPosition=glm::vec3(0,0,0);
	glm::quat mRotation;
	glm::vec3 mScale=glm::vec3(1, 1, 1);
};

#endif //UGINE_ENTITY_H