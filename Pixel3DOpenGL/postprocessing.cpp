#include "postprocessing.h"

std::vector<Ref<PostProcessLayer>> PostProcessing::_layers;

Bloom::Bloom() {
	_shader = Ref<Shader>(new Shader("bloom.shader"));

	int resolution = 1;

	_finalFBO.Bind();
	_finalFBO.GenenerateTexture2(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);

	resolution *= 2;

	_thresholdFBO.Bind();
	_thresholdFBO.GenenerateTexture2(Renderer::resolution.x / resolution, Renderer::resolution.y / resolution, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);

	for (int i = 0; i < quality; i++) {
		resolution *= 2;
		FBO FBO1 = FBO();
		FBO1.GenenerateTexture2(Renderer::resolution.x / resolution, Renderer::resolution.y / resolution, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);
		FBO FBO2 = FBO();
		FBO2.GenenerateTexture2(Renderer::resolution.x / resolution, Renderer::resolution.y / resolution, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);
		_blurFBOS.push_back(FBO1);
		_blurFBOS.push_back(FBO2);
	}

	_finalFBO.Unbind();
}

Bloom::~Bloom() {
	_shader->Delete();
	_finalFBO.Delete();
	_thresholdFBO.DeleteTexture(0);
	_thresholdFBO.Delete();

	for (int i = 0; i < quality * 2; i++) {
		_blurFBOS[i].DeleteTexture(0);
		_blurFBOS[i].Delete();
	}
}

Ref<Texture> Bloom::Apply(Ref<Texture> texture) {
	_shader->Activate();
	_shader->SetFloatParameter("threshold", 3);
	_shader->SetIntParameter("tex0", 0);
	_shader->SetIntParameter("tex1", 1);
	_shader->SetIntParameter("mode", 0);

	int resolution = 2;

	glViewport(0, 0, Renderer::resolution.x / resolution, Renderer::resolution.y / resolution);
	_thresholdFBO.Bind();

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	glDisable(GL_DEPTH_TEST);
	Renderer::DrawQuad(_shader);

	bool horizontal = false;
	_shader->SetIntParameter("mode", 1);


	for (int i = 0; i < quality; i++) {
		resolution *= 2;
		for (int j = 0; j < 2; j++) {
			//glViewport(0, 0, _resolutions[i + 1].x, _resolutions[i + 1].y);
			glViewport(0, 0, Renderer::resolution.x / resolution, Renderer::resolution.y / resolution);
			_blurFBOS[i * 2 + j].Bind();
			//_blurFBOS[i * 2 + j].GenenerateTexture2(resolution.x, resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);

			_shader->SetIntParameter("horizontal", horizontal);
			horizontal = !horizontal;

			glActiveTexture(GL_TEXTURE0);
			if (i + j != 0) {
				_blurFBOS[i * 2 + j - 1].BindTexture(0);
			}
			else {
				_thresholdFBO.BindTexture(0);
			}

			Renderer::DrawQuad(_shader);
		}
	}

	_shader->SetIntParameter("mode", 2);

	for (int i = (quality * 2) - 3; i >= 1; i -= 2) {
		resolution /= 2;
		_blurFBOS[i].Bind();

		glViewport(0, 0, Renderer::resolution.x / resolution, Renderer::resolution.y / resolution);
		glActiveTexture(GL_TEXTURE0);
		if (i == (quality * 2) - 3) {
			_blurFBOS[i + 1].BindTexture(0);
		}
		else {
			_blurFBOS[i + 2].BindTexture(0);
		}

		glActiveTexture(GL_TEXTURE1);
		_blurFBOS[i].BindTexture(0);

		Renderer::DrawQuad(_shader);
	}

	resolution /= 2;

	_thresholdFBO.Bind();
	glActiveTexture(GL_TEXTURE0);
	_blurFBOS[1].BindTexture(0);
	glActiveTexture(GL_TEXTURE1);
	_thresholdFBO.BindTexture(0);

	glViewport(0, 0, Renderer::resolution.x / resolution, Renderer::resolution.y / resolution);
	Renderer::DrawQuad(_shader);


	glViewport(0, 0, Renderer::resolution.x, Renderer::resolution.y);
	_finalFBO.Bind();
	//_finalFBO.GenenerateTexture2(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);

	glActiveTexture(GL_TEXTURE0);
	_thresholdFBO.BindTexture(0);
	glActiveTexture(GL_TEXTURE1);
	texture->Bind();

	Renderer::DrawQuad(_shader);

	_finalFBO.Unbind();

	Ref<Texture> tex(new Texture(_finalFBO.TexID[0]));
	return tex;
}

Pixelate::Pixelate() {
	_shader = Ref<Shader>(new Shader("pixelate.shader"));
	_FBO1.Bind();
	_FBO1.GenenerateTexture2(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);
	_FBO1.Unbind();
}

Pixelate::~Pixelate() {
	_shader->Delete();
	_FBO1.Delete();
}

Ref<Texture> Pixelate::Apply(Ref<Texture> texture) {
	_shader->Activate();
	_shader->SetFloatParameter("pixelateIntensity", pixelateIntensity);

	_FBO1.Bind();
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();
	Renderer::DrawQuad(_shader);

	_FBO1.Unbind();

	Ref<Texture> tex(new Texture(_FBO1.TexID[0]));
	return tex;
}

ColorGrading::ColorGrading() {
	_shader = Ref<Shader>(new Shader("colorgrading.shader"));
	_FBO1.Bind();
	_FBO1.GenenerateTexture2(Renderer::resolution.x, Renderer::resolution.y, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_FLOAT, GL_CLAMP_TO_EDGE, 1);
	_FBO1.Unbind();
}

ColorGrading::~ColorGrading() {
	_shader->Delete();
	_FBO1.Delete();
}

Ref<Texture> ColorGrading::Apply(Ref<Texture> texture) {
	_shader->Activate();
	_shader->SetFloatParameter("brightness", brightness);
	_shader->SetFloatParameter("contrast", contrast);
	_shader->SetFloatParameter("saturation", saturation);
	_shader->SetFloatParameter("exposure", exposure);

	_FBO1.Bind();
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();
	Renderer::DrawQuad(_shader);

	_FBO1.Unbind();

	Ref<Texture> tex(new Texture(_FBO1.TexID[0]));
	return tex;
}

Ref<Texture> PostProcessing::Apply(Ref<Texture> texture) {
	Ref<Texture> output;

	if (_layers.size() == 0) return texture;

	for (int i = 0; i < _layers.size(); i++) {
		if (i == 0) {
			output = _layers[i]->Apply(texture);
		}
		else {
			output = _layers[i]->Apply(output);
		}
	}

	return output;
}