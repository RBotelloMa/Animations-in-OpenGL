#ifndef UGINE_RENDERER_H
#define UGINE_RENDERER_H

#include "types.h"

#define MAX_BONES 75
#define BASETEX_UNIT GL_TEXTURE0
#define BASECUBETEX_UNIT GL_TEXTURE1
#define REFLECTTEX_UNIT GL_TEXTURE2
#define REFRACTTEX_UNIT GL_TEXTURE3
#define NORMALTEX_UNIT GL_TEXTURE4
#define DEPTHSAMPLER_UNIT GL_TEXTURE15


class Renderer {
public:
	static const std::shared_ptr<Renderer>& Instance();
	// Setup
	void Setup3D();
	void SetMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void SetViewport(int x, int y, int w, int h);
	
	// Drawing
	void ClearColorBuffer(const glm::vec3& color);
	void ClearDepthBuffer();

	// VBO
	uint32 CreateBuffer();
	void FreeBuffer(uint32 buffer);
	void SetVertexBufferData(uint32 vertexBuffer, const void* data, uint32 dataSize);
	void SetIndexBufferData(uint32 indexBuffer, const void* data, uint32 dataSize);
	void DrawBuffers(uint32 vertexBuffer, uint32 indexBuffer, uint32 numIndices);

	// Shaders
	uint32 CreateProgram(const std::string& vertex, const std::string& fragment);
	void FreeProgram(uint32 program);
	void UseProgram(uint32 program);
	const std::string& GetProgramError();

	//Texture
	uint32 LoadTexture(const std::string& filename, uint32& width, uint32& height);
	void FreeTexture(uint32 tex);
	//void SetTexture(uint32 tex, bool hayTextura);

  void SetTextures(uint32 baseTexture, uint32 reflectionTexture,
                    uint32 refractionTexture, uint32 normalTexture, bool isBaseTexCube);


	//Metodos Luces
	void SetDiffuse(const glm::vec4& color);
	void SetAmbient(const glm::vec3& color);
	void SetShininess(uint8 shininess);
	void EnableLighting(bool enable);
	void EnableLight(uint32 index, bool enabled);
	void SetLightData(uint32 index,	const glm::vec4& vector, const glm::vec3& color, float attenuation);

  //Practica 5
  void SetSkinned(bool skinned);
  void SetAnimMatrices(const std::vector<glm::mat4>& matrices);

  //Practica 6
  enum BlendMode {
    ALPHA,
    MULTIPLICATIVE,
    ADDITIVE
  };
  void SetBlendMode(BlendMode mode);
  void SetCulling(bool enable);
  void SetDepthWriting(bool enable);
  void SetEyePos(const glm::vec3& pos);
  void SetRefractCoef(float coef);
  uint32 LoadCubeTexture(const std::string& left, const std::string& right,
    const std::string& front, const std::string& back,
    const std::string& top, const std::string& bottom,
    uint32& width, uint32& height);




protected:
	Renderer();
	~Renderer() {}
	static void Delete(Renderer* r) { delete r; }
private:
	static std::shared_ptr<Renderer> mInstance;
	int mMVPLoc;
	
	int mVPosLoc;
	int mVTexPos;
	

	//practica iluminación

	int mVNormalPos;

	int mMV;
	int mMNormal;
	int mVDiffuse;
	int mVAmbient;
	int mVShininess;

	int mVlightingEnabled;
	int mVlightEnabled[MAX_LIGHTS];
	int mVlightPos[MAX_LIGHTS];
	int mVlightColor[MAX_LIGHTS];
	int mVlightAtt[MAX_LIGHTS];

  //flag textura
  int mVHayTextura;

	uint32 mDefaultProgram;
	std::string mProgramError;

  //practica 5
  int mSkinnedLoc;
  int mAnimMatricesLoc;
  int mVBoneIndicesLoc;
  int mVBoneWeightsLoc;

  //practica 6
  int mModelLoc;
  int mEyePosLoc;
  int mRefractCoefLoc;
  int mBaseTexModeLoc;
  int mUseReflectTexLoc;
  int mUseRefractTexLoc;
  int mUseNormalTexLoc;
  int mVTangentLoc;

  
  //tex samplers
  int mVTexSampler;
  int mVBaseCubeSampler;
  int mVReflectCubeSampler;
  int mVRefractCubeSampler;
  int mVNormalTexSampler;



};

inline const std::shared_ptr<Renderer>& Renderer::Instance() {
	if ( !mInstance )
		mInstance = std::shared_ptr<Renderer>(new Renderer(), Delete);
	return mInstance;
}

#endif // UGINE_RENDERER_H
