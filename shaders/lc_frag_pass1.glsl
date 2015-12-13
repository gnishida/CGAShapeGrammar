#version 330
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

void main(){
	def_diffuse = outColor.xyz;
	//if(false){//mv space
	//	def_normal = normalMatrix*varyingNormal;
	//	def_originPos = (mvMatrix*vec4(origVertex, 1.0)).xyz;
	//} else {//object space
		def_normal = varyingNormal;
		def_originPos = origVertex;
	//}
	//def_originPos = (mvpMatrix*vec4(origVertex, 1.0)).xyz;// origVertex;

	// TEXTURE
	if((mode&0xFF)==2||(mode&0xFF)==4||(mode&0xFF)==6){// tex / water / model texture
		def_diffuse = texture( tex0, outUV.rg ).rgb;
	}
	
	//////////////
	// TERRAIN
	if((mode&0xFF)==3){

		vec4 terrainColor=vec4(0,0,0,1.0);
		float factor;
		float height=(origVertex.z)/255.0;//0-1
		height=min(height,1.0);
		height=max(height,0);

		if(height<=0.33){
			factor=height/0.33;//0-1
			terrainColor+=(1-factor)*texture( tex_3D, vec3(outUV.rg,0.0));
			terrainColor+=factor*texture( tex_3D, vec3(outUV.rg+vec2(0.1,0.1),1.0));
		}else{
			if(height<=0.66){
				factor=(height-0.33)/0.33f;//0-1
				terrainColor+=(1-factor)*texture( tex_3D, vec3(outUV.rg+vec2(0.1,0.1),1.0));
				terrainColor+=factor*texture( tex_3D, vec3(outUV.rg+vec2(0.45,0.45),2.0));
			}else{
				factor=(height-0.66)/0.33f;//0-1
				terrainColor+=(1-factor)*texture( tex_3D, vec3(outUV.rg+vec2(0.45,0.45),2.0));
				terrainColor+=factor*texture( tex_3D, vec3(outUV.rg+vec2(0.3,0.3),3.0));
			}
		}
		def_diffuse =terrainColor.rgb;
	}
}//

