//#include "Particle.h"
//#include "Graphics.h"
//#include "SOIL.h"
//
//
//TE::Particle::Particle() : Component(ComponentType::CT_PARTICLE)
//{
//	GRAPHICS->ParticleList.push_back(this);
//}
//
//TE::Particle::~Particle()
//{
//	GRAPHICS->ParticleList.erase(std::find(GRAPHICS->ParticleList.begin(), GRAPHICS->ParticleList.end(), this));
//}
//
//void TE::Particle::Initialize()
//{
//}
//
//GLuint & TE::Particle::texutre_load(std::string filepath)
//{
//	char * Userinfo;
//	size_t len = filepath.size();
//	_dupenv_s(&Userinfo, &len, "USERPROFILE");
//
//	std::string saveLevel = filepath;
//#ifdef _DEBUG
//	filepath = ".\\texture.\\" + filepath;
//#else
//	filepath = Userinfo;
//	filepath += "/Documents/RoteRote/texture/" + saveLevel;
//#endif
//	free(Userinfo);
//
//	m_textureID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
//	
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, m_textureID);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	m_TexutureDir = saveLevel;
//	return m_textureID;
//}
//
//int TE::Particle::findFreeParticle()
//{
//	return 0;
//}
