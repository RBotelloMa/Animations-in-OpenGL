#ifndef UGINE_TEXTURE
#define UGINE_TEXTURE

#include "types.h"
#include "renderer.h"

class Texture {
public:
	static std::shared_ptr<Texture> Create(const std::string& filename);
  static std::shared_ptr<Texture> Create(const std::string& left,
    const std::string& right, const std::string& front,
    const std::string& back, const std::string& top,
    const std::string& bottom);
	const std::string& GetFilename() const;
	uint32 GetHandle() const;
	uint32 GetWidth() const;
	uint32 GetHeight() const;
  //prac6
  bool IsCubeMap() const;
protected:
	Texture(const std::string& filename);
  Texture(const std::string& left, const std::string& right,
          const std::string& front, const std::string& back,
          const std::string& top, const std::string& bottom);
	~Texture();
	static void Delete(Texture* t) { delete t; }
private:
	std::string mFilename;
	uint32 mHandle;
	uint32 mWidth;
	uint32 mHeight;

  //p6
  bool mIsCubeMap;
  std::string mRight;
  std::string mLeft;
  std::string mFront;
  std::string mBack;
  std::string mTop;
  std::string mBottom;


};


#endif // UGINE_TEXTURE