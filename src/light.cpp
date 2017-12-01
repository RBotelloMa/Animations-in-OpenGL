#include "../include/light.h"
#include "../include/renderer.h"
#include "../include/scene.h"


bool Light::mLightsUsed[MAX_LIGHTS];

std::shared_ptr<Light> Light::Create() {
	return std::shared_ptr<Light>(new Light(), Delete);
}

Light::Type Light::GetType() const {
	return mType;
};

void Light::SetType(Light::Type type) {
	mType = type;
};

const glm::vec3& Light::GetColor() const {
	return mColor;
};
void Light::SetColor(const glm::vec3& color) {
	mColor = color;
};

float Light::GetAttenuation() const {
	return mAttenuation;
};
void Light::SetAttenuation(float att) {
	mAttenuation = att;
};

void Light::Prepare() {
	Renderer::Instance()->EnableLight(mIndex,true);
	Renderer::Instance()->SetLightData(mIndex, Scene::Instance()->GetCurrentCamera()->GetView()*glm::vec4(GetPosition(),mType), mColor, mAttenuation);
};



Light::Light() {
	int i = 0;
	bool b=true;
	while ((i < MAX_LIGHTS)&&b) {
		if (!(Light::mLightsUsed[i])) 
    {
			Light::mLightsUsed[i] = true;
			b = false;
			mIndex = i;
		}
		else ++i;
	}
};

	//TODO: + robusto (por si mas de 8 luces)

Light::~Light() {
	Light::mLightsUsed[mIndex] = false;
};

/*private:
	static bool mLightsUsed[MAX_LIGHTS];
	int mIndex;
	Type mType;
	glm::vec3 mColor;
	float mAttenuation;
*/