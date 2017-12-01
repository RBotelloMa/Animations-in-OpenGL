#include "../include/texture.h"



std::shared_ptr<Texture> Texture::Create(const std::string& filename) {
	std::shared_ptr<Texture> ret = std::shared_ptr<Texture>(new Texture(filename), Delete);
	if (ret->GetHandle() == 0) return nullptr;
	else return ret;
};

std::shared_ptr<Texture> Texture::Create(const std::string& left, const std::string& right, const std::string& front,
            const std::string& back, const std::string& top, const std::string& bottom) {
  
  std::shared_ptr<Texture> ret = std::shared_ptr<Texture>(new Texture(left, right, front, back, top, bottom), Delete);
  if (ret->GetHandle() == 0) return nullptr;
  else return ret;
}



const std::string& Texture::GetFilename() const {
	return mFilename;
}

uint32 Texture::GetHandle() const {
	return mHandle;
}
uint32 Texture::GetWidth() const {
	return mWidth;
}
uint32 Texture::GetHeight() const {
	return mHeight;
}


Texture::Texture(const std::string& filename) {
	mFilename = filename;
	mHandle = Renderer::Instance()->Renderer::LoadTexture(filename, mWidth, mHeight);
  mIsCubeMap = false;
};

Texture::Texture(const std::string& left, const std::string& right,
                 const std::string& front, const std::string& back,
                 const std::string& top, const std::string& bottom) {
  mRight = right;
  mLeft = left;
  mBottom = bottom;
  mTop = top;
  mFront = front;
  mBack = back;
  mHandle = Renderer::Instance()->Renderer::LoadCubeTexture(left, right, front, back, top, bottom, mWidth, mHeight);
  mIsCubeMap = true;
};


Texture::~Texture() {};


bool Texture::IsCubeMap() const {
  return mIsCubeMap;
};



