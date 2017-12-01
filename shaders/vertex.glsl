#define MAX_LIGHTS 8
#define MAX_BONES 75


uniform mat4 MVP;
uniform mat4 modelView;
uniform mat4 normalMatrix;

//P5
uniform bool skinned;
uniform mat4 animMatrices[MAX_BONES];

attribute vec3 vpos;
attribute vec2 vuv;
attribute vec3 vnormal;
//p6
attribute vec3 vtangent;

varying vec2 fuv;
varying vec4 fpos4;
varying vec3 fnormal;
//varying vec3 fcolor;

//P5
attribute vec4 vboneIndices;
attribute vec4 vboneWeights;

//P6
uniform mat4 model;
uniform vec3 eyePos;
uniform float refractCoef;



varying vec3 cubeCoords;
varying vec3 reflectCoords;
varying vec3 refractCoords;
varying mat3 tbnMatrix;


void main() {
  
  vec4 vpos4 = vec4(vpos, 1);
  // Animación por huesos
  if (skinned) {
    mat4 boneTransform = mat4(1);
    for (int i = 0; i < 4; ++i) {
      int index = int(vboneIndices[i]);
      if (index > -1) boneTransform += animMatrices[index] * vboneWeights[i];
    }
    vpos4 = boneTransform * vpos4;
  }
  //cubeCoords
  cubeCoords = vec3(model*vpos4);

  //reflex y refrac coords
  
  vec3 eye = normalize(vec3(model * vec4(vpos, 1)) - eyePos);
  vec3 normal = normalize(vec3(model * vec4(vnormal, 0)));

  reflectCoords = normalize(reflect(eye, normal));
  refractCoords = normalize(refract(eye, normal, refractCoef));

  //tbnMatrix
  vec4 normale4 = normalMatrix*vec4(vnormal, 1);
  vec4 tangente4 = normalMatrix*vec4(vtangent, 1);
  vec3 normale = vec3(normale4[0], normale4[1], normale4[2]);
  vec3 tangente = vec3(tangente4[0], tangente4[1], tangente4[2]);

  vec3 bitangente = cross(normale, tangente);

  tbnMatrix[0] = tangente;//son columnas
  tbnMatrix[1] = bitangente;
  tbnMatrix[2] = normale;


  gl_Position = MVP * vpos4;
	fuv=vuv;
  fpos4 = vpos4;  
  fnormal = vnormal;

}