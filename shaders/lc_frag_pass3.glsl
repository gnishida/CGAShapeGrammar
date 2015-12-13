#version 420
#extension GL_EXT_gpu_shader4 : enable

in vec2 outUV;

layout(location = 0)out vec4 outputF;

uniform sampler2D tex0;//color
uniform sampler2D tex1;//normals
uniform sampler2D tex2;//orig pos
uniform sampler2D tex3;//AO

uniform vec2 pixelSize;//in texture space
uniform int pass2used;

const int uBlurSize = 4; // use size of noise texture

void main(){

		

		vec2 coord =  outUV.xy;
		vec3 color = texture(tex0, coord).rgb;


		outputF.rgb = color;// vec3(1, 0, 1);// color;
		
		
		////////////////////////////////
		// SSAO
		//float ssaoVal = texture(tex3, coord).r;
		float ssaoVal = 1.0;
		if (pass2used == 1) {
			vec2 texelSize = 1.0 / vec2(textureSize(tex3, 0));
			ssaoVal = 0.0f;
			vec2 hlim = vec2(float(-uBlurSize) * 0.5 + 0.5);
			for (int i = 0; i < uBlurSize; ++i) {
				for (int j = 0; j < uBlurSize; ++j) {
					vec2 offset = (hlim + vec2(float(i), float(j))) * texelSize;
					ssaoVal += texture(tex3, coord + offset).r;
				}
			}
			ssaoVal = ssaoVal / float(uBlurSize * uBlurSize);
		}
		////////////////////////////////
		// LIGHTING
		vec3 normal = texture(tex1, coord).rgb;
		vec3 lightDir = normalize(vec3(-0.235, -0.5773, 0.9773));
		float lighting = 1.0f;
		if (length(normal) > 0.1) {
			lighting = max(0.0, dot(lightDir, normalize(normal)));
		}
		
		
		
		
		outputF.rgb = outputF.rgb*ssaoVal;
		outputF.rgb = vec3(0.2) + vec3(0.8)*lighting*outputF.rgb;
		outputF.a = 1.0;
	
}//

