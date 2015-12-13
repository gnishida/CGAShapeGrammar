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

uniform int screenWidth;
uniform int screenHeight;

float LinearizeDepth(float z){
		const float zNear = 5.0; // camera z near
		const float zFar = 10000.0; // camera z far

		return (2.0 * zNear) / (zFar + zNear - z * (zFar - zNear));
}//

/*----------------------------------------------------------------------------*/
//	ssao uniforms:
const int MAX_KERNEL_SIZE = 128;
uniform int uKernelSize=16;//16
uniform vec3 uKernelOffsets[MAX_KERNEL_SIZE];
uniform float uRadius = 20.0;//1.5
uniform float uPower = 1.0;//2.0

float linearizeDepth(in float depth, in mat4 projMatrix) {
	return projMatrix[3][2] / (depth - projMatrix[2][2]);
}

float ssao(in mat3 kernelBasis, in vec3 originPos, in float radius) {
	float occlusion = 0.0;
	
	vec4 originPosProj = mvpMatrix * vec4(originPos, 1.0);//for rangecheck
	originPosProj.xyz /= originPosProj.w; 
	originPosProj.xyz = originPosProj.xyz * 0.5 + 0.5; // scale/bias to texcoords
	float originDepth = texture(depthTex, originPosProj.xy).r;
	//originDepth = linearizeDepth(originDepth, pMatrix);

	for (int i = 0; i < uKernelSize; ++i) {
		//	get sample position:
		vec3 samplePos = kernelBasis * uKernelOffsets[i];
		samplePos = samplePos * radius + originPos;
		
		//samplePos = originPos + uKernelOffsets[i];
		//	project sample position:
		vec4 offset;
		//if (false) {
		//	offset = pMatrix * vec4(samplePos, 1.0);
		//} else {
			offset = mvpMatrix * vec4(samplePos, 1.0);
		//}
		//vec4 offset = uProjectionMatrix * vec4(samplePos, 1.0);
		
		offset.xyz /= offset.w; // only need xy
		offset.xyz = offset.xyz * 0.5 + 0.5; // scale/bias to texcoords

		//	get sample depth:
		float sampleDepth = texture(depthTex, offset.xy).r;
		//sampleDepth = linearizeDepth(sampleDepth, pMatrix);

		//float rangeCheck = smoothstep(0.0, 1.0, radius / abs(originPos.z - sampleDepth));
		//occlusion += rangeCheck * step(sampleDepth, samplePos.z);
		//occlusion += (sampleDepth <= samplePos.z ? 1.0 : 0.0);// *rangeCheck;

		float offsetDepth = offset.z;

		float rangeCheck = 1.0f;// abs(offsetDepth - sampleDepth) < radius ? 1.0 : 0.0;
		//float rangeCheck = smoothstep(0.0, 1.0, radius / abs(originPosProj.z - sampleDepth));


		
		//offsetDepth = linearizeDepth(offsetDepth, pMatrix);

		

		occlusion += (sampleDepth <= offsetDepth ? 1.0 : 0.0)*rangeCheck;// *rangeCheck;
	}

	occlusion = 1.0 - (occlusion / float(uKernelSize));
	return pow(occlusion, uPower);
}//

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

			vec3 nn = texture(tex1, vec2(coord.x + float(xx) / screenWidth, coord.y + float(yy) / screenHeight)).xyz;
			float dd = texture(depthTex, vec2(coord.x + float(xx) / screenWidth, coord.y + float(yy) / screenHeight)).x;
			//dd = linearizeDepth(dd, pMatrix);
			vec3 pp = texture(tex2, vec2(coord.x + float(xx) / screenWidth, coord.y + float(yy) / screenHeight)).xyz;

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

