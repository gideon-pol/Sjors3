#pragma once
#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <glm/glm.hpp>
#include "renderer.h"
#include "FBO.h"
#include "shader.h"

#include "texture.h"
#include "Core.h"

class PostProcessLayer {
public:
	virtual void Init() {};
	virtual Ref<Texture> Apply(Ref<Texture> texture) { return nullptr; };
private:
	Ref<Shader> _shader;
};

class Bloom : public PostProcessLayer {
public:
	Bloom();
	~Bloom();

	Ref<Texture> Apply(Ref<Texture> texture);
	int quality = 4;
private:
	Ref<Shader> _shader;
	FBO _thresholdFBO;
	FBO _finalFBO;
	std::vector<FBO> _blurFBOS;
};

class Pixelate : public PostProcessLayer {
public:
	Pixelate();
	~Pixelate();

	Ref<Texture> Apply(Ref<Texture> texture);
	float pixelateIntensity = 3.0f;
private:
	Ref<Shader> _shader;
	FBO _FBO1;
};

class ColorGrading : public PostProcessLayer {
public:
	ColorGrading();
	~ColorGrading();

	Ref<Texture> Apply(Ref<Texture> texture);
	float brightness = 0.0f;
	float contrast = 1.0f;
	float saturation = 1.0f;
	float exposure = 1.0f;
	
private:
	Ref<Shader> _shader;
	FBO _FBO1;
};

class PostProcessing {
public:
	template<typename T>
	static void AddLayer(Ref<T> layer) {
		//layer->Init();
		_layers.push_back(layer);
	}

	template<typename T>
	static void AddLayer(Ref<T> layer, int index) {
		//layer->Init();
		_layers.insert(_layers.begin() + index, 1, layer);
	}

	template<typename T>
	static void RemoveLayer(Ref<T> layer) {
		for (int i = 0; i < _layers.size(); i++) {
			if (_layers[i] == layer) {
				_layers.erase(std::next(_layers.begin(), i));
				return;
			}
		}
	}

	template<typename T>
	static void RemoveLayer(Ref<T> layer, int index) {
		_layers.erase(std::next(_layers.begin(), index));
	}

	static Ref<Texture> ApplyBloom(Ref<Texture> texture);
	static Ref<Texture> Apply(Ref<Texture> texture);
private:
	static std::vector<Ref<PostProcessLayer>> _layers;
};

#endif


