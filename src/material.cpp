
#include "../include/material.h"



Material::Material(const std::shared_ptr<Texture>& baseTexture) {
  mBaseTexture = baseTexture;
};
const glm::vec4& Material::GetBaseColor() const {
  return mBaseColor;
};
void Material::SetBaseColor(const glm::vec4& color) {
  mBaseColor = color;
};
const std::shared_ptr<Texture>& Material::GetBaseTexture() {
  return mBaseTexture;
};
void Material::SetBaseTexture(const std::shared_ptr<Texture>& tex) {
  mBaseTexture = tex;
};
const std::shared_ptr<Texture>& Material::GetReflectionTexture() {
  return mReflectionTexture;
};
void Material::SetReflectionTexture(const std::shared_ptr<Texture>& tex) {
  mReflectionTexture = tex;
};
const std::shared_ptr<Texture>& Material::GetRefractionTexture() {
  return mRefractionTexture;
};
void Material::SetRefractionTexture(const std::shared_ptr<Texture>& tex) {
  mRefractionTexture = tex;
};
const std::shared_ptr<Texture>& Material::GetNormalTexture() {
  return mNormalTexture;
};
void Material::SetNormalTexture(const std::shared_ptr<Texture>& tex) {
  mNormalTexture = tex;
};
float Material::GetRefractionCoef() const {
  return mRefractCoef;
};
void Material::SetRefractionCoef(float coef) {
  mRefractCoef = coef;
};
Renderer::BlendMode Material::GetBlendMode() const {
  return mBlendMode;
};
void Material::SetBlendMode(Renderer::BlendMode mode) {
  mBlendMode = mode;
};
bool Material::GetCulling() const {
  return mCulling;
};
void Material::SetCulling(bool enable) {
  mCulling = enable;
};
bool Material::GetDepthWrite() const {
  return mDepthWrite;
};
void Material::SetDepthWrite(bool enable) {
  mDepthWrite = enable;
};
uint8 Material::GetShininess() const {
  return mShininess;
};
void Material::SetShininess(uint8 shininess) {
  mShininess = shininess;
};
void Material::Prepare() const {
  Renderer::Instance()->SetBlendMode(mBlendMode);
  Renderer::Instance()->SetDiffuse(mBaseColor);
  Renderer::Instance()->SetCulling(mCulling);
  Renderer::Instance()->SetDepthWriting(mDepthWrite);
  Renderer::Instance()->SetShininess(mShininess);
  Renderer::Instance()->SetRefractCoef(mRefractCoef);

  uint32 aux1, aux2, aux3, aux4, bool1;

  if (mBaseTexture == nullptr) {
    aux1 = 0;
    bool1 = false;
  }
  else {
    aux1 = mBaseTexture.get()->GetHandle();
    bool1 = mBaseTexture.get()->IsCubeMap();
  }

  if (mReflectionTexture == nullptr) {
    aux2 = 0;
  }
  else aux2 = mReflectionTexture.get()->GetHandle();

  if (mRefractionTexture == nullptr) {
    aux3 = 0;
  }
  else aux3 = mRefractionTexture.get()->GetHandle();

  if (mNormalTexture == nullptr) {
    aux4 = 0;
  }
  else aux4 = mNormalTexture.get()->GetHandle();

  Renderer::Instance()->SetTextures(aux1, aux2, aux3, aux4, bool1);

};




/* glm::vec4 mBaseColor;
  std::shared_ptr<Texture> mBaseTexture;
  std::shared_ptr<Texture> mReflectionTexture;
  std::shared_ptr<Texture> mRefractionTexture;
  std::shared_ptr<Texture> mNormalTexture;
  Renderer::BlendMode mBlendMode;
  bool mCulling;
  bool mDepthWrite;
  uint8 mShininess;
  float mRefractCoef;*/
















