#include "ParticleSystemPCH.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "Random.h"
#include "ParticleEffect.h"
#include "Allocator.h"
#include "PoolAllocator.h"
#include "Util.h"
#include "StopWatch.h"

ParticleEffect::ParticleEffect(unsigned int numParticles /* = 0 */)
	: m_pCamera(NULL)
	, m_pParticleEmitter(NULL)
	, m_ColorInterpolator(glm::vec4(1))
	, m_LocalToWorldMatrix(1)
	, m_TextureID(0)
	, m_Force(0, -9.81f, 0)
{
	Resize(numParticles);
	srand(time(NULL));
}

ParticleEffect::~ParticleEffect()
{
	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
	}
}

void ParticleEffect::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

void ParticleEffect::SetParticleEmitter(ParticleEmitter* pEmitter)
{
	m_pParticleEmitter = pEmitter;
}

void ParticleEffect::SetColorInterplator(const ColorInterpolator& colors)
{
	m_ColorInterpolator = colors;
}

bool ParticleEffect::LoadTexture(const std::string& fileName)
{
	if (m_TextureID != 0)
	{
		glDeleteTextures(1, &m_TextureID);
	}

	m_TextureID = SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	return (m_TextureID != 0);
}

void ParticleEffect::RandomizeParticle(Particle& particle)
{
	particle.m_fAge = 0.0f;
	particle.m_fLifeTime = RandRange(3, 5);

	glm::vec3 unitVec = RandUnitVec();

	particle.m_Position = unitVec * 1.0f;
	particle.m_Velocity = unitVec * RandRange(10, 20);
}

void ParticleEffect::RandomizeParticles()
{
	for (unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		RandomizeParticle(*(m_Particles[i]));
	}
}

void ParticleEffect::EmitParticle(Particle& particle)
{
	assert(m_pParticleEmitter != NULL);
	m_pParticleEmitter->EmitParticle(particle);
}

void ParticleEffect::EmitParticles()
{
	//if (m_pParticleEmitter == NULL)
	//{
	//	RandomizeParticles();
	//}
	//else
	//{
	for (unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		EmitParticle(*(m_Particles[i]));
	}
	//}
}

void ParticleEffect::BuildVertexBuffer()
{
	const glm::vec3 X(0.5, 0, 0);
	const glm::vec3 Y(0, 0.5, 0);
	const glm::vec3 Z(0, 0, 1.0);

	glm::quat cameraRotation;

	if (m_pCamera != NULL)
	{
		cameraRotation = glm::quat(glm::radians(m_pCamera->GetRotation()));
	}

	// Make sure the vertex buffer has enough vertices to render the effect
	// If the vertex buffer is already the correct size, no change is made.
	m_VertexBuffer.resize(m_Particles.size() * 4, Vertex());

	for (unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		Particle& particle = *(m_Particles[i]);

		glm::quat rotation = glm::angleAxis(particle.m_fRotate, Z);

		unsigned int vertexIndex = i * 4;
		Vertex& v0 = m_VertexBuffer[vertexIndex + 0];   // Bottom-left
		Vertex& v1 = m_VertexBuffer[vertexIndex + 1];   // Bottom-right
		Vertex& v2 = m_VertexBuffer[vertexIndex + 2];   // Top-right
		Vertex& v3 = m_VertexBuffer[vertexIndex + 3];   // Top-left

		// Bottom-left
		v0.m_Pos = particle.m_Position + (rotation * (-X - Y) * particle.m_fSize) * cameraRotation;
		v0.m_Tex0 = glm::vec2(0, 1);
		v0.m_Diffuse = particle.m_Color;

		// Bottom-right
		v1.m_Pos = particle.m_Position + (rotation * (X - Y) * particle.m_fSize) * cameraRotation;
		v1.m_Tex0 = glm::vec2(1, 1);
		v1.m_Diffuse = particle.m_Color;

		// Top-right
		v2.m_Pos = particle.m_Position + (rotation * (X + Y) * particle.m_fSize) * cameraRotation;
		v2.m_Tex0 = glm::vec2(1, 0);
		v2.m_Diffuse = particle.m_Color;

		// Top-left
		v3.m_Pos = particle.m_Position + (rotation * (-X + Y) * particle.m_fSize) * cameraRotation;
		v3.m_Tex0 = glm::vec2(0, 0);
		v3.m_Diffuse = particle.m_Color;
	}

}

void ParticleEffect::Update(float fDeltaTime, bool allocate, bool deallocate, int amount)
{
	//int x = 0;
	for (unsigned int i = 0; i < m_Particles.size(); ++i)
	{
		Particle& particle = *(m_Particles[i]);

		particle.m_fAge += fDeltaTime;
		//		if (particle.m_fAge > particle.m_fLifeTime  && x < 100)
		//		{
		//			if (m_pParticleEmitter != NULL)  //EmitParticle(particle);
		//			{
		//				ParticleBuffer::iterator it = std::find(m_Particles.begin(), m_Particles.end(), &particle);
		//				if (it != m_Particles.end() && (getRangedRandom(0, 100000) < getCreateProbability()))
		//				{
		//				std::cout << getCreateProbability() << std::endl;
		//					Particle* p = &particle;
		//					m_Particles.erase(it);
		//#ifdef memory
		//					//POOL_DEALLOC(p, "test", Particle);
		//					HEAP_DEALLOC3(p, "test", Particle);
		//					m_VertexBuffer.resize(m_Particles.size() * 4, Vertex());
		//					++x;
		//#else
		//					delete p;
		//#endif // memory
		//				}
		//			}
		//			//else RandomizeParticle(particle);
		//		}

		float lifeRatio = particle.m_fAge;// glm::saturate(particle.m_fAge / particle.m_fLifeTime);
		if (lifeRatio == 0.0f) lifeRatio = 0.9f;
		m_Force = glm::core::type::vec3(0, -1, 0);
		particle.m_Velocity += (m_Force * fDeltaTime);
		particle.m_Position += (particle.m_Velocity * fDeltaTime);
		particle.m_Color = m_ColorInterpolator.GetValue(lifeRatio);
		particle.m_fRotate = glm::lerp<float>(0.0f, 720.0f, lifeRatio);
		particle.m_fSize = glm::lerp<float>(5.0f, 0.0f, 0.0f);// lifeRatio);
	}

	//if (m_Particles.empty())
	//{
	//	GLOBAL_MEMORY_MANAGER.OutputFileAverages();
	//}

	for (int i = 0; i < amount; ++i)
	{
		bool createParticles = false;
		bool deleteParticles = false;
		if (allocate && deallocate)
		{
			if (GetNumParticles() <= 0)
			{
				createParticles = true;
			}
			else if (GetNumParticles() >= 100000)
			{
				createParticles = false;
				deleteParticles = true;
			}
			else
			{
				float p = getCreateProbability();
				if (getRangedRandom(0, 100) < getCreateProbability())
				{
					createParticles = true;
				}
				else
				{
					deleteParticles = true;
				}
			}
			if (createParticles)
			{
				CreateParticle();
			}
			if (deleteParticles)
			{
				DestroyParticle();
			}
		}
		else if (allocate)
		{
			CreateParticle();
		}
		else if (deallocate)
		{
			DestroyParticle();
		}
	}

	BuildVertexBuffer();
}

void ParticleEffect::Render()
{
	glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
	glEnable(GL_BLEND);                 // Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
	glEnable(GL_TEXTURE_2D);            // Enable textures

	glPushMatrix();
	glMultMatrixf(glm::value_ptr(m_LocalToWorldMatrix));

	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Pos));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Tex0));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Diffuse));

	glDrawArrays(GL_QUADS, 0, m_VertexBuffer.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);

#if _DEBUG
	if (m_pParticleEmitter != NULL)
	{
		m_pParticleEmitter->DebugRender();
	}
#endif

	glPopMatrix();

}

void ParticleEffect::Resize(unsigned int numParticles)
{
	//m_Particles.resize( numParticles,  new Particle() );
	

	m_Particles.clear();
	float totalMs = 0.0f;
	std::vector<float> allms;
	for (unsigned int i = 0U; i < numParticles; ++i)
	{
		Library::StopWatch sw;
#if MEMORY == 1
#if USE_HEAP_FIRST == 1
		sw.Start();
		m_Particles.push_back(HEAP_ALLOC_FIRSTFIT("test") Particle());
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#elif USE_HEAP_BEST == 1
		sw.Start();
		m_Particles.push_back(HEAP_ALLOC_BESTFIT("test") Particle());
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#elif USE_HEAP_LAST == 1
		sw.Start();
		m_Particles.push_back(HEAP_ALLOC_LASTFIT("test") Particle());
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#elif USE_POOL == 1
		sw.Start();
		m_Particles.push_back(POOL_ALLOC("test") Particle());
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#endif
#else
		sw.Start();
		m_Particles.push_back(new Particle());
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#endif // MEMORY

	}

	//std::string directory("DataData.txt");
	//FILE* outfile;
	//fopen_s(&outfile, directory.c_str(), "w+");
	//int index = 0;
	//if (outfile != nullptr)
	//{
	//	for (const auto& data : allms)
	//	{
	//		//fprintf_s(outfile, "%d, %.30lf\n", index, data);
	//		fprintf_s(outfile,"%.30lf\n", data);
	//		++index;
	//	}
	//}
	//fclose(outfile);

	//float ms = totalMs / numParticles;
	//std::cout << ms << std::endl;
	//
	//CustomMemoryManager::Allocators::IAllocator* allocator = GLOBAL_MEMORY_MANAGER.Get("test", CustomMemoryManager::AllocType::HEAP);
	//if (allocator != nullptr)
	//{
	//	CustomMemoryManager::Allocators::HeapAllocator* pool = static_cast<CustomMemoryManager::Allocators::HeapAllocator*>(allocator);
	//	if (pool != nullptr)
	//	{
	//		GLOBAL_MEMORY_MANAGER.OutputFileAverages();
	//	}
	//}
	m_VertexBuffer.resize(numParticles * 4, Vertex());
}

void ParticleEffect::CreateParticle()
{
#if MEMORY == 1
#if USE_HEAP_FIRST == 1
	Particle* p = HEAP_ALLOC_FIRSTFIT("test") Particle();
#elif USE_HEAP_BEST == 1
	Particle* p = HEAP_ALLOC_BESTFIT("test") Particle();
#elif USE_HEAP_LAST == 1
	Particle* p = HEAP_ALLOC_LASTFIT("test") Particle();
#elif USE_POOL == 1
	Particle* p = POOL_ALLOC("test") Particle();
#endif
#else
	Particle* p = new Particle();
#endif
	if (p != nullptr)
	{
		m_Particles.push_back(p);
		m_VertexBuffer.resize(m_Particles.size() * 4, Vertex());
		EmitParticle(*p);
	}
}

void ParticleEffect::DestroyParticle()
{
	Particle* p = nullptr;
	if (m_Particles.size() == 0U)
		return;
	if (m_Particles.size() == 1U)
		p = m_Particles.front();

	int particleNum = getRangedRandom(0, m_Particles.size() - 1);

	int particleCount = 0;
	ParticleBuffer::iterator it = m_Particles.begin();
	for (; it != m_Particles.end(); ++it)
	{
		if (particleCount == particleNum)
		{
			p = *it;
			p->m_Color = m_ColorInterpolator.GetValue(1.0f);
			break;
		}
		++particleCount;
	}

	if (p != nullptr)
	{
		m_Particles.erase(it);
#if MEMORY == 1
#if USE_HEAP_FIRST == 1 || USE_HEAP_BEST == 1 || USE_HEAP_LAST == 1
		HEAP_DEALLOC3(p, "test", Particle);
#elif USE_POOL == 1
		POOL_DEALLOC(p, "test", Particle);
#endif // USE_HEAP
#else
		delete p;
#endif
		m_VertexBuffer.resize(m_Particles.size() * 4, Vertex());
	}
}

void ParticleEffect::DestroyParticle(ParticleBuffer::iterator& it)
{
	Particle* p = *it;
	m_Particles.erase(it);
	//Heap
}

void ParticleEffect::DestroyAllParticles()
{
	float totalMs = 0.0f;
	std::vector<float> allms;
	for (auto& p : m_Particles)
	{
		Library::StopWatch sw;
#if MEMORY == 1
#if USE_HEAP_FIRST == 1 || USE_HEAP_BEST == 1 || USE_HEAP_LAST == 1
		sw.Start();
		HEAP_DEALLOC3(p, "test", Particle);
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#elif USE_POOL == 1
		sw.Start();
		POOL_DEALLOC(p, "test", Particle);
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#endif // USE_HEAP
#else
		sw.Start();
		delete p;
		sw.Stop();
		allms.push_back(sw.Elapsed().count());
#endif
	}
	std::string directory("DataDataDealloc.txt");
	FILE* outfile;
	fopen_s(&outfile, directory.c_str(), "w+");
	int index = 0;
	if (outfile != nullptr)
	{
		for (const auto& data : allms)
		{
			//fprintf_s(outfile, "%d, %.30lf\n", index, data);
			fprintf_s(outfile,"%.30lf\n", data);
			++index;
		}
	}
	fclose(outfile);
}
