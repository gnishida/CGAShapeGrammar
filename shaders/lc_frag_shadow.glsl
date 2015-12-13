#version 420
#extension GL_EXT_gpu_shader4 : enable

in vec4 outColor;
in vec2 outUV;
in vec3 origVertex;// L

in vec3 varyingNormal;

// output color
out vec4 outputF;

uniform int mode;
uniform sampler2D tex0;
uniform sampler2DArray tex_3D;

uniform sampler2D shadowMap;

uniform mat4 light_mvpMatrix;
uniform vec3 lightDir;

uniform int textureEnabled;		// 1 -- texture / 0 -- color only
uniform int depthComputation;	// 1 -- depth computation / 0 - otherwise
uniform int useShadow;			// 1 -- use shadow / 0 - no shadow 

uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
//uniform mat4 mvpMatrix;

float shadowCoef(){
	vec4 shadow_coord2 = light_mvpMatrix * vec4(origVertex, 1.0);
	vec3 ProjCoords = shadow_coord2.xyz / shadow_coord2.w;
	vec2 UVCoords;
	UVCoords.x = 0.5 * ProjCoords.x + 0.5;
	UVCoords.y = 0.5 * ProjCoords.y + 0.5;
	float z = 0.5 * ProjCoords.z + 0.5;

	float visibility = 1.0f;
	if (texture2D(shadowMap, UVCoords).z  <  z) {
		visibility = 0;
	}
	return visibility;
}

void main(){
	float opacity = outColor.w;
	outputF = vec4(outColor.xyz, 1);

	/*
	// shadow map computation
	if (depthComputation == 1) return;
	
	if (textureEnabled == 1) { // for texture mode
		outputF = outputF * texture(tex0, fTexCoord.rg);
	}

	// lighting
	vec4 ambient = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0) * max(0.0, dot(-lightDir, varyingNormal));

	float shadow_coef = 1.0;
	if (useShadow == 1) {
		shadow_coef = shadowCoef();
	}
	outputF = (ambient + (shadow_coef * 0.95 + 0.05) * diffuse) * outputF;
	*/
}

