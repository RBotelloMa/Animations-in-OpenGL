#define MAX_LIGHTS 8
#define MAX_BONES 75


uniform sampler2D texSampler;
uniform samplerCube baseCubeSampler;
uniform samplerCube reflectCubeSampler;
uniform samplerCube refractCubeSampler;
uniform sampler2D normalTexSampler;
//uniform bool fhayTextura;

uniform vec4 diffuse;
uniform vec3 ambient;
uniform int shininess;

uniform mat4 modelView;
uniform mat4 normalMatrix;

uniform bool lightingEnabled;
uniform bool lightEnabled[MAX_LIGHTS];

uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform float lightAtt[MAX_LIGHTS];


varying vec2 fuv;
varying vec4 fpos4;
varying vec3 fnormal;

//P6
uniform int baseTexMode;
uniform bool useReflectTex;
uniform bool useRefractTex;
uniform bool useNormalTex;

varying vec3 cubeCoords;
varying vec3 reflectCoords;
varying vec3 refractCoords;
varying mat3 tbnMatrix;



void main() {
  vec4 fcolor = vec4(0, 0, 0, 0);
  if (lightingEnabled) {
    vec3 combinedDiffuse = ambient;
    vec3 combinedSpecular = vec3(0, 0, 0);
    int it;
    for (it = 0; it < MAX_LIGHTS; ++it) {
      if (lightEnabled[it]) {

        vec4 N;
        if (useNormalTex) {
          vec4 sampl = texture2D(normalTexSampler, fuv);
          N = vec4(tbnMatrix * (vec3(sampl[0],sampl[1],sampl[2])*2.0 - 1.0) ,1);
        }
        else {
          N = normalMatrix*vec4(fnormal, 1);
        }
        float normalizerN = 1.0 / sqrt((N[0] * N[0] + N[1] * N[1] + N[2] * N[2]));
        N[0] = N[0] * normalizerN;
        N[1] = N[1] * normalizerN;
        N[2] = N[2] * normalizerN;

        vec4 E = modelView*fpos4;
        vec4 L = lightPos[it];
        float att = 1.0;

        //si puntual 
        if (L[3] == 1.0) {
          L = L - E;
          att = 1.0 / pow((1.0 + lightAtt[it] * length(L)), 5.0);
        }

        float normalizerL = 1.0 / sqrt((L[0] * L[0] + L[1] * L[1] + L[2] * L[2]));
        L[0] = L[0] * normalizerL;
        L[1] = L[1] * normalizerL;
        L[2] = L[2] * normalizerL;

        float NdotL = max(N[0] * L[0] + N[1] * L[1] + N[2] * L[2], 0.0);
        combinedDiffuse = combinedDiffuse + vec3(NdotL*lightColor[it][0] * att, NdotL*lightColor[it][1] * att, NdotL*lightColor[it][2] * att);

        if ((shininess > 0) && (NdotL > 0.0)) {
          float normalizerE = 1.0 / sqrt((E[0] * E[0] + E[1] * E[1] + E[2] * E[2]));
          E[0] = E[0] * normalizerE;
          E[1] = E[1] * normalizerE;
          E[2] = E[2] * normalizerE;
          vec4 H = L - E;

          float normalizerH = 1.0 / sqrt((H[0] * H[0] + H[1] * H[1] + H[2] * H[2]));
          H[0] = H[0] * normalizerH;
          H[1] = H[1] * normalizerH;
          H[2] = H[2] * normalizerH;

          float sh = float(shininess);

          float NdotH = max(N[0] * H[0] + N[1] * H[1] + N[2] * H[2], 0.0);
          combinedSpecular += vec3(pow(NdotH, sh)*att, pow(NdotH, sh)*att, pow(NdotH, sh)*att);
        }
      }
    }
    vec4 auxColor = vec4(combinedDiffuse[0], combinedDiffuse[1], combinedDiffuse[2], 1);
    if (useReflectTex) {
      auxColor = 0.5*auxColor + vec4(textureCube(reflectCubeSampler, reflectCoords));
    }
    if (useRefractTex) {
      auxColor = 0.5*auxColor + vec4(textureCube(refractCubeSampler, refractCoords));
    }
    fcolor = vec4(diffuse[0] * auxColor[0], diffuse[1] * auxColor[1], diffuse[2] * auxColor[2], diffuse[3]) + vec4(combinedSpecular, 0.0);
  }
  else {
    fcolor = diffuse;
  }

  if (baseTexMode == 0) {
    gl_FragColor = fcolor;
  }
  else if (baseTexMode == 1) {
    gl_FragColor = texture2D(texSampler, fuv)*fcolor;
  }
  else
  {
    gl_FragColor = textureCube(baseCubeSampler, cubeCoords)*fcolor;
  }
}
