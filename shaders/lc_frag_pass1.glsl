#version 420
#extension GL_EXT_gpu_shader4 : enable

in vec4 outColor;
in vec2 outUV;
in vec3 origVertex;// L

in vec3 varyingNormal;

layout(location = 0)out vec3 def_diffuse;
layout(location = 1)out vec3 def_normal;
layout(location = 2)out vec3 def_originPos;

uniform int mode;
uniform sampler2D tex0;
uniform sampler2DArray tex_3D;

uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
//uniform mat4 mvpMatrix;

uniform int useShadow;
uniform mat4 light_mvpMatrix;
uniform vec3 lightDir;
uniform sampler2D shadowMap;
uniform int textureEnabled;

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
	def_diffuse = outColor.xyz;
	def_normal = varyingNormal;
	def_originPos = origVertex;

	////// DEBUG //////
	/*
	vec4 shadow_coord2 = light_mvpMatrix * vec4(origVertex, 1.0);
	def_diffuse = vec3(shadow_coord2.x * 0.5 + 0.5, shadow_coord2.y * 0.5 + 0.5, 1);
	return;
	*/
	////// DEBUG //////

	if (textureEnabled == 1) { // for texture mode
		def_diffuse = def_diffuse * texture(tex0, outUV.rg).xyz;
	}

	// lighting
	vec3 ambient = vec3(0.6, 0.6, 0.6);
	vec3 diffuse = vec3(0.5, 0.5, 0.5) * max(0.0, dot(-lightDir, varyingNormal));

	float visibility = 1.0;
	if (useShadow == 1) {
		visibility = shadowCoef();
	}

	def_diffuse = (ambient + (visibility * 0.95 + 0.05) * diffuse) * def_diffuse;
}

