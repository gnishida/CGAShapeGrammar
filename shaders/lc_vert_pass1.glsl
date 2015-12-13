#version 330

layout(location = 0)in vec3 vertex;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec4 color;
layout(location = 3)in vec2 uv;

out vec4 outColor;
out vec2 outUV;
out vec3 origVertex;// L

out vec3 varyingNormal;

// UNIFORM
uniform int mode;

// MODE 1--> color
// MODE 2--> texture

//0x0100 --> adapt terrain
//0x0200 --> lighting

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;

void main(){
	outColor=color;
	outUV=uv;
	origVertex=vertex;

	// LIGHTING
	if((mode&0x0200)==0x0200){
		varyingNormal=normal;
	}else{
		varyingNormal=vec3(0);//no lighting (check when lighting)
	}

	gl_Position = mvpMatrix * vec4(origVertex,1.0);

}