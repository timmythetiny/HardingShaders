#include "stdafx.h"

AI_SHADER_NODE_EXPORT_METHODS(DirectionalAoMethods);

enum DirectionalAoParams {
	p_direction,
	p_falloff,
	p_spread,
	p_samples
};


uint32_t hashCString(const char* str) {
	uint32_t result = 0;
	const uint32_t prime = 31;
	while (str != '\0') {
		result = (result * prime) + *(str++);
	}
	return result;
}


node_parameters{
	AiParameterVec("direction", 0.0, 1.0, 0.0)
	AiParameterFlt("falloff", 1.0);
	AiParameterFlt("spread", 1.0);
	AiParameterInt("samples", 4);
}


node_initialize{ 
	AiNodeSetLocalData(node, nullptr);
}


node_update{
	AtSampler* data = static_cast<AtSampler*>(AiNodeGetLocalData(node));
	if (data != nullptr) {
		AiSamplerDestroy(data);
	}
	const int seed = hashCString(AiNodeGetName(node));
	const int samples = AiNodeGetInt(node, "samples");
	const int dimensions = 2;
	AtSampler* sampler = AiSampler(seed, samples, dimensions);
	AiNodeSetLocalData(node, sampler);
}


node_finish{
	AtSampler* sampler = static_cast<AtSampler*>(AiNodeGetLocalData(node));
	if (sampler != nullptr) {
		AiSamplerDestroy(sampler);
	}
}


shader_evaluate{
	const float falloff = AiShaderEvalParamFlt(p_falloff);
	const float spread = AiShaderEvalParamFlt(p_spread);
	const AtVector direction = AiV3Normalize(AiShaderEvalParamVec(p_direction));
	const AtSampler* sampler = static_cast<AtSampler*>(AiNodeGetLocalData(node));
	sg->out.RGB() = AI_RGB_WHITE - AiOcclusion(direction, direction, sg, 0.0, AI_BIG, spread, falloff, sampler, nullptr);
}