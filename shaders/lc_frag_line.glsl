#version 420
#extension GL_EXT_gpu_shader4 : enable

in vec2 outUV;

//layout(location = 0)out vec4 def_AO;
layout(location = 0)out vec4 outputF;

uniform sampler2D tex0;//color
uniform sampler2D tex1;//normals
uniform sampler2D tex2;//orig pos

uniform sampler2D depthTex;

uniform vec2 pixelSize;//in texture space

//uniform mat4 uProjectionMatrix; // current projection matrix, for linearized depth
//uniform mat4 uInvProjectionMatrix;
uniform mat4 pMatrix;
uniform mat4 mvpMatrix;

float LinearizeDepth(float z){
		const float zNear = 5.0; // camera z near
		const float zFar = 10000.0; // camera z far

		return (2.0 * zNear) / (zFar + zNear - z * (zFar - zNear));
}//

float linearizeDepth(in float depth, in mat4 projMatrix) {
	return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

void main(){
	float normalSensitivity = 50.0;
	float depthSensitivity = 400000.0;

	vec2 coord = outUV.xy;
	
	vec3 normal = texture(tex1, coord).rgb;
	normal = normalize(normal);

	float depth = texture(depthTex, coord).r;
	//depth = linearizeDepth(depth, pMatrix);

	vec3 originPos = texture(tex2, coord).rgb;

	// check the normal and depth in the surrounding pixels

	int range = 1;
	float normal_diff = 0;
	float depth_diff = 0;

	for (int xx = -range; xx <= range; ++xx) {
		for (int yy = -range; yy <= range; ++yy) {
			if (xx == 0 && yy == 0) continue;

			vec3 nn = texture(tex1, vec2(coord.x + xx * pixelSize.x, coord.y + yy * pixelSize.y)).xyz;
			float dd = texture(depthTex, vec2(coord.x + xx * pixelSize.x, coord.y + yy * pixelSize.y)).x;
			//dd = linearizeDepth(dd, pMatrix);
			vec3 pp = texture(tex2, vec2(coord.x + xx * pixelSize.x, coord.y + yy * pixelSize.y)).xyz;

			if (abs(dot(normalize(pp - originPos), normal)) < 0.4) continue;

			normal_diff = max(normal_diff, length(normal - nn));
			depth_diff = max(depth_diff, length(depth - dd));
		}
	}

	float diff = min(1, max(depth_diff * depthSensitivity, normal_diff * normalSensitivity));
	if (diff > 0.3) diff = 1;
	else diff = 0;
	outputF = vec4(1 - diff, 1 - diff, 1 - diff, 1);


}

