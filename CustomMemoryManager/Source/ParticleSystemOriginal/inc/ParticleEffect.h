#pragma once

#include "Particle.h"
#include "Interpolator.h"

#define USE_HEAP 1
#define USE_POOL 0


class Camera;
class ParticleEmitter;

/**
 * ParticleEffect class that demonstrates one possible implementation for
 * billboard particles.
 */

class ParticleEffect
{
public:

	// A vertex for the particle
	struct Vertex
	{
		Vertex()
			: m_Pos(0)
			, m_Diffuse(0)
			, m_Tex0(0)
		{}

		glm::vec3   m_Pos;      // Vertex position
		glm::vec4   m_Diffuse;  // Diffuse color
		glm::vec2   m_Tex0;     // Texture coordinate
	};
	typedef std::vector<Vertex> VertexBuffer;
	typedef Interpolator<glm::vec4> ColorInterpolator;

	ParticleEffect(unsigned int numParticles = 0);
	virtual ~ParticleEffect();

	void SetCamera(Camera* pCamera);
	void SetParticleEmitter(ParticleEmitter* pEmitter);
	void SetColorInterplator(const ColorInterpolator& colors);

	// Test method to randomize the particles in an interesting way
	void RandomizeParticles();
	void EmitParticles();

	virtual void Update(float fDeltaTime);
	virtual void Render();

	bool LoadTexture(const std::string& fileName);

	// Resize the particle buffer with numParticles
	void Resize(unsigned int numParticles);
	void CreateParticle();
	void DestroyParticle();
	void DestroyParticle(ParticleBuffer::iterator& particle);
	inline std::size_t GetNumParticles() { return m_Particles.size(); };
	inline float getCreateProbability() { return 100.0f - (float)(m_Particles.size() * 100 / 100000); };

protected:
	void RandomizeParticle(Particle& particle);
	void EmitParticle(Particle& particle);
public:
	// Build the vertex buffer from the particle buffer
	void BuildVertexBuffer();
private:
	Camera * m_pCamera;
	ParticleEmitter*    m_pParticleEmitter;
	ColorInterpolator   m_ColorInterpolator;

	ParticleBuffer      m_Particles;
	VertexBuffer        m_VertexBuffer;
	glm::mat4x4         m_LocalToWorldMatrix;
	GLuint              m_TextureID;

	// Apply this force to every particle in the effect
	glm::vec3           m_Force;
};