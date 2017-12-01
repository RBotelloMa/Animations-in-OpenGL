#include"../include/camera.h"


std::shared_ptr<Camera> Camera::Create() {
		return std::shared_ptr<Camera>(new Camera(), Delete);
}

int32 Camera::GetViewportX() const { 
	return mVX; 
};
int32 Camera::GetViewportY() const {
	return mVY;
};

uint16 Camera::GetViewportWidth() const {
	return mVW;
};
uint16 Camera::GetViewportHeight() const {
	return mVH;
};

void Camera::SetViewport(int32 x, int32 y, uint16 w, uint16 h) {
	mVX = x;
	mVY = y;
	mVW = w;
	mVH = h;
};
void Camera::SetProjection(const glm::mat4& proj) {
	mProjMatrix = proj;
};
const glm::mat4& Camera::GetProjection() const {
	return mProjMatrix;
};
const glm::mat4& Camera::GetView() const {
	return mViewMatrix;
};
const glm::vec3& Camera::GetColor() const {
	return mColor;
};
void Camera::SetColor(const glm::vec3& color) {
	mColor = color;
};
bool Camera::GetUsesTarget() const {
	return mUsesTarget;
};
void Camera::SetUsesTarget(bool usesTarget) {
	mUsesTarget = usesTarget;
};
const glm::vec3& Camera::GetTarget() const {
	return mTarget;
};
glm::vec3& Camera::DefTarget() {
	return mTarget;
};

void Camera::Prepare() {
	Renderer::Instance()->SetViewport(mVX, mVY, mVW, mVH);
	if (mUsesTarget)
	{
		mViewMatrix= glm::lookAt(GetPosition(), mTarget, GetRotation()*glm::vec3(0, 1, 0));
	}
	else
	{
		mViewMatrix = glm::lookAt(GetPosition(), -GetRotation()*glm::vec3(0, 0, 1), GetRotation()*glm::vec3(0, 1, 0));
	}
	
	Renderer::Instance()->ClearColorBuffer(mColor);
	Renderer::Instance()->ClearDepthBuffer();
  Renderer::Instance()->SetEyePos(this->GetPosition());
};

Camera::Camera() {};

/*private:
	glm::mat4 mProjMatrix;
	glm::mat4 mViewMatrix;
	int32 mVX, mVY;

	uint16 mVW, mVH;
	glm::vec3 mColor;
	bool mUsesTarget;
	glm::vec3 mTarget;
*/