#include "../lib/glew/glew.h"
#include "../include/renderer.h"
#include "../include/vertex.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../lib/stb_image.h"

std::shared_ptr<Renderer> Renderer::mInstance = nullptr;

Renderer::Renderer() {
	mDefaultProgram = CreateProgram(ReadString("shaders/vertex.glsl"), ReadString("shaders/fragment.glsl"));
	UseProgram(mDefaultProgram);
}

void Renderer::Setup3D() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glDepthFunc(GL_LEQUAL);

  //prac 6
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  

}

void Renderer::SetMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(view * model));*/
	
	glUniformMatrix4fv(mMVPLoc, 1, false, glm::value_ptr(projection * view * model));
	glUniformMatrix4fv(mMV, 1, false, glm::value_ptr(view * model));
	glUniformMatrix4fv(mMNormal, 1, false, (glm::value_ptr(glm::transpose(glm::inverse(view*model)))));
  glUniformMatrix4fv(mModelLoc, 1, false, glm::value_ptr(model));

}

void Renderer::SetViewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
}

void Renderer::ClearColorBuffer(const glm::vec3& color) {
	glClearColor(color.r, color.g, color.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepthBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

uint32 Renderer::CreateBuffer() {
	uint32_t buffer;
	glGenBuffers(1, &buffer);
	return buffer;
}

void Renderer::FreeBuffer(uint32 buffer) {
	glDeleteBuffers(1, &buffer);
}

void Renderer::SetVertexBufferData(uint32 vertexBuffer, const void* data, uint32 dataSize) {
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(mVPosLoc);
	glEnableVertexAttribArray(mVTexPos);
	glVertexAttribPointer(mVPosLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mPosition)));
	glVertexAttribPointer(mVTexPos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mTexture)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(mVPosLoc);
	glDisableVertexAttribArray(mVTexPos);


}

void Renderer::SetIndexBufferData(uint32 indexBuffer, const void* data, uint32 dataSize) {
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::DrawBuffers(uint32 vertexBuffer, uint32 indexBuffer, uint32 numIndices) {
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableVertexAttribArray(mVPosLoc);
	glEnableVertexAttribArray(mVTexPos);
  glEnableVertexAttribArray(mVNormalPos);
  glEnableVertexAttribArray(mVBoneIndicesLoc);
  glEnableVertexAttribArray(mVBoneWeightsLoc);
  glEnableVertexAttribArray(mVTangentLoc);
	glVertexAttribPointer(mVPosLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mPosition)));
	glVertexAttribPointer(mVTexPos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mTexture)));
	glVertexAttribPointer(mVNormalPos, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mNormal)));
  glVertexAttribPointer(mVBoneIndicesLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mBoneIndices)));
  glVertexAttribPointer(mVBoneWeightsLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mBoneWeights)));
  glVertexAttribPointer(mVTangentLoc, 4, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, Vertex::mTangent)));
  //TODO: Cambiar vetices


	//añadimos variables practica 3?



	
	// Indicamos el puntero a los vertices. Cuando usemos VBOs, el ultimo parametro debe ser
	// el offset a las posiciones dentro del buffer: offsetof(Vertex, mPosition)
	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, mPosition)));

	
	// Indicamos el puntero a los vertices. Cuando usemos VBOs, el ultimo parametro debe ser 0,
	// ya que ya hemos enlazado el buffer donde se encuentran los indices

	
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<const void*>(0));
	
	// Aqui podemos desenlazar los buffers que hemos enlazado al comienzo
	// ...
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(mVPosLoc);
	glDisableVertexAttribArray(mVTexPos);
	glDisableVertexAttribArray(mVNormalPos);
  glDisableVertexAttribArray(mVBoneIndicesLoc);
  glDisableVertexAttribArray(mVBoneWeightsLoc);
  glDisableVertexAttribArray(mVTangentLoc);


}

uint32 Renderer::CreateProgram(const std::string& vertex, const std::string& fragment) {
	//Creamos el vertex shader
	GLint retCode;
	const char* vshaderSrc = vertex.c_str();
	char ErrorLog[1024];
	uint32_t vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, &vshaderSrc, nullptr);
	glCompileShader(vshader);
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vshader, sizeof(ErrorLog), NULL, ErrorLog);
		mProgramError = ErrorLog;
		return 0;
	}

	// Creamos fragment shader
	const char* fshaderSrc = fragment.c_str();
	uint32_t fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, &fshaderSrc, nullptr);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fshader, sizeof(ErrorLog), NULL, ErrorLog);
		mProgramError = ErrorLog;
		return 0;
	}

	// Creamos el programa
	uint32_t program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
		mProgramError = ErrorLog;
		return 0;
	}
	return program;
}

void Renderer::FreeProgram(uint32 program) {
	glDeleteProgram(program);
}

void Renderer::UseProgram(uint32 program) {
	glUseProgram(program);

	mMVPLoc = glGetUniformLocation(program, "MVP");
	
  //samplers
  mVTexSampler = glGetUniformLocation(program, "texSampler");
  mVBaseCubeSampler= glGetUniformLocation(program, "baseCubeSampler");
  mVReflectCubeSampler= glGetUniformLocation(program, "reflectCubeSampler");
  mVRefractCubeSampler = glGetUniformLocation(program, "refractCubeSampler");
  mVNormalTexSampler = glGetUniformLocation(program, "normalTexSampler");

	mVPosLoc = glGetAttribLocation(program, "vpos");
	mVTexPos = glGetAttribLocation(program, "vuv");
	mVNormalPos = glGetAttribLocation(program, "vnormal");
  //p6
  mVTangentLoc = glGetAttribLocation(program, "vtangent");

	//añadimos matriz modelo-vista y matriz normal
	mMV = glGetUniformLocation(program, "modelView");
	mMNormal = glGetUniformLocation(program, "normalMatrix");
	
	//añadimos variable uniformes de iluminación
	mVDiffuse = glGetUniformLocation(program, "diffuse");
	mVAmbient = glGetUniformLocation(program, "ambient");
	mVShininess = glGetUniformLocation(program, "shininess");
	//variables de luces
	
	mVlightingEnabled = glGetUniformLocation(program, "lightingEnabled");
  for (int it = 0; it < MAX_LIGHTS; it++) {

    mVlightEnabled[it] = glGetUniformLocation(program, std::string("lightEnabled["+std::to_string(it)+"]").c_str());
    mVlightPos[it] = glGetUniformLocation(program, std::string("lightPos[" + std::to_string(it) + "]").c_str());
    mVlightColor[it] = glGetUniformLocation(program, std::string("lightColor[" + std::to_string(it) + "]").c_str());
    mVlightAtt[it] = glGetUniformLocation(program, std::string("lightAtt[" + std::to_string(it) + "]").c_str());
  }
  //flag textura
  //mVHayTextura = glGetUniformLocation(program, "fhayTextura");

  //practica 5
  mSkinnedLoc = glGetUniformLocation(program, "skinned");
  mAnimMatricesLoc = glGetUniformLocation(program, "animMatrices");

  mVBoneIndicesLoc = glGetAttribLocation(program, "vboneIndices");
  mVBoneWeightsLoc = glGetAttribLocation(program, "vboneWeights");

  //practica 6
  mModelLoc = glGetUniformLocation(program, "model");
  mEyePosLoc = glGetUniformLocation(program, "eyePos");
  mRefractCoefLoc = glGetUniformLocation(program, "refractCoef");

  mBaseTexModeLoc = glGetUniformLocation(program, "baseTexMode");
  mUseReflectTexLoc = glGetUniformLocation(program, "useReflectTex");
  mUseRefractTexLoc = glGetUniformLocation(program, "useRefractTex");
  mUseNormalTexLoc = glGetUniformLocation(program, "useNormalTex");


}

const std::string& Renderer::GetProgramError() {
	return mProgramError;
}

//Texture
uint32 Renderer::LoadTexture(const std::string& filename, uint32& width, uint32& height)
{
	//Cargamos textura con stb_image
	std::shared_ptr<uint8_t> texBuffer = std::shared_ptr<uint8_t>(stbi_load(filename.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
	if (texBuffer != nullptr) {
		std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
		for (int line = 0; line < height; ++line) {
			memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
		}
		// Generamos textura
		uint32_t glTex;
		glGenTextures(1, &glTex);
		glBindTexture(GL_TEXTURE_2D, glTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mirroredTexBuffer.get());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);


		//TODO: añadir stbi image free

		//retornamos textura
		return glTex;
	}
	else return 0;
};

void Renderer::FreeTexture(uint32 tex) {
	
	glDeleteTextures(1, &tex);
};

/*void Renderer::SetTexture(uint32 tex, bool hayTextura) {
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(mVTexSampler, 0);
  //glUniform1i(mVHayTextura, hayTextura);

};*/


void Renderer::SetTextures(uint32 baseTexture, uint32 reflectionTexture,
                           uint32 refractionTexture, uint32 normalTexture, bool isBaseTexCube) {
  
  
  glActiveTexture(BASETEX_UNIT);
  glBindTexture(GL_TEXTURE_2D, baseTexture);
  glUniform1i(mVTexSampler, 0);
  
  glActiveTexture(BASECUBETEX_UNIT);
  glBindTexture(GL_TEXTURE_CUBE_MAP, baseTexture);
  glUniform1i(mVBaseCubeSampler, 1);
  
  glActiveTexture(REFLECTTEX_UNIT);
  glBindTexture(GL_TEXTURE_CUBE_MAP, reflectionTexture);
  glUniform1i(mVReflectCubeSampler, 2);
  
  glActiveTexture(REFRACTTEX_UNIT);
  glBindTexture(GL_TEXTURE_CUBE_MAP, refractionTexture);
  glUniform1i(mVRefractCubeSampler, 3);

  glActiveTexture(NORMALTEX_UNIT);
  glBindTexture(GL_TEXTURE_2D, normalTexture);
  glUniform1i(mVNormalTexSampler, 4);
  

  if (baseTexture == 0) {
    glUniform1i(mBaseTexModeLoc, 0);
  }
  else{
    if (isBaseTexCube) {
      glUniform1i(mBaseTexModeLoc, 2);
    }
    else { 
      glUniform1i(mBaseTexModeLoc,1);
    }
  }

  if (reflectionTexture == 0) {
    glUniform1i(mUseReflectTexLoc, 0);
  }
  else {
    glUniform1i(mUseReflectTexLoc, 1);
  }


  if (refractionTexture == 0) {
    glUniform1i(mUseRefractTexLoc, 0);
  }
  else {
    glUniform1i(mUseRefractTexLoc, 1);
  }

  if (normalTexture == 0) {
    glUniform1i(mUseNormalTexLoc, 0);
  }
  else {
    glUniform1i(mUseNormalTexLoc,1);
  }
};

//funciones iluminación practica 3 (needs checks)
void Renderer::SetDiffuse(const glm::vec4& color) {
	glUniform4fv(mVDiffuse, 1, glm::value_ptr(color));
}

void Renderer::SetAmbient(const glm::vec3& color) {
	glUniform3fv(mVAmbient, 1, glm::value_ptr(color));
}

void Renderer::SetShininess(uint8 shininess) {
	glUniform1i(mVShininess, shininess);
}


void Renderer::EnableLighting(bool enable) {
	glUniform1i(mVlightingEnabled, enable);
}


void Renderer::EnableLight(uint32 index, bool enabled) {
	glUniform1i(mVlightEnabled[index], enabled);
};


void Renderer::SetLightData(uint32 index, const glm::vec4& vector, const glm::vec3& color, float attenuation) {
	glUniform4fv(mVlightPos[index],1,glm::value_ptr(vector));
	glUniform3fv(mVlightColor[index], 1, glm::value_ptr(color));
	glUniform1f(mVlightAtt[index],  attenuation);
};

//practica 5
void Renderer::SetSkinned(bool skinned) {
  glUniform1i(mSkinnedLoc,skinned);
};

void Renderer::SetAnimMatrices(const std::vector<glm::mat4>& matrices) {
  
    int numBones = std::min(MAX_BONES, (int)matrices.size());
    glUniformMatrix4fv(mAnimMatricesLoc, numBones, false, glm::value_ptr(matrices[0]));
  
};


//Practica 6

void Renderer::SetBlendMode(BlendMode mode) {
  if (mode == BlendMode::ADDITIVE) {
    glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
  }
  else if (mode == BlendMode::ALPHA) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  else if(mode==BlendMode::MULTIPLICATIVE){
    glBlendFunc(GL_DST_COLOR, GL_ZERO);
  }
};
void Renderer::SetCulling(bool enable) {
  //TODO
};
void Renderer::SetDepthWriting(bool enable) {
  glDepthMask(enable);
};
void Renderer::SetEyePos(const glm::vec3& pos) {
  glUniform3fv(mEyePosLoc, 1, glm::value_ptr(pos));
};
void Renderer::SetRefractCoef(float coef) {
  glUniform1f(mRefractCoefLoc, coef);
};
uint32 Renderer::LoadCubeTexture(const std::string& left, const std::string& right,
                                  const std::string& front, const std::string& back,
                                  const std::string& top, const std::string& bottom,
                                  uint32& width, uint32& height) {
 
  uint32_t glTex;
  // Generamos textura
  glGenTextures(1, &glTex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, glTex);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


  
  //Cargamos textura 1 con stb_image 
  std::shared_ptr<uint8_t> texBuffer = std::shared_ptr<uint8_t>(stbi_load(left.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
  if (texBuffer != nullptr) {
    std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
    for (int line = 0; line < height; ++line) {
      memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer.get());
  }

  //Cargamos textura 2 con stb_image 
  texBuffer = std::shared_ptr<uint8_t>(stbi_load(right.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
  if (texBuffer != nullptr) {
    std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
    for (int line = 0; line < height; ++line) {
      memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer.get());
  }

  //Cargamos textura 3 con stb_image 
  texBuffer = std::shared_ptr<uint8_t>(stbi_load(bottom.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
  if (texBuffer != nullptr) {
    std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
    for (int line = 0; line < height; ++line) {
      memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer.get());
  }

  //Cargamos textura 4 con stb_image 
  texBuffer = std::shared_ptr<uint8_t>(stbi_load(top.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
  if (texBuffer != nullptr) {
    std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
    for (int line = 0; line < height; ++line) {
      memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer.get());
  }

  //Cargamos textura 5 con stb_image 
  texBuffer = std::shared_ptr<uint8_t>(stbi_load(front.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
  if (texBuffer != nullptr) {
    std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
    for (int line = 0; line < height; ++line) {
      memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer.get());
  }

  //Cargamos textura 6 con stb_image 
  texBuffer = std::shared_ptr<uint8_t>(stbi_load(back.c_str(), (int *)&width, (int *)&height, nullptr, 4), stbi_image_free);
  if (texBuffer != nullptr) {
    std::shared_ptr<uint8_t> mirroredTexBuffer = std::shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(width * height * 4)), free);
    for (int line = 0; line < height; ++line) {
      memcpy(mirroredTexBuffer.get() + line*width * 4, texBuffer.get() + (height - line - 1)*width * 4, width * 4);
    }
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBuffer.get());
  }
    //retornamos textura

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return glTex;
};

