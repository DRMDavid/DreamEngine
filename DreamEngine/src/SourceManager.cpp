#include "SourceManager.h"
#include "ECS/Texture.h"
#include "Prerequisites.h"  // para ERROR()

bool ResourceManager::loadTexture(const std::string& fileName,
  const std::string& extension) {
  // Verificar si la textura esta cargada
  if (m_textures.find(fileName) != m_textures.end()) {
    return true;
  }

  // Crear y cargar la textura
  auto texture = EngineUtilities::MakeShared<Texture>(fileName, extension);
  m_textures[fileName] = texture;
  return true;
}

EngineUtilities::TSharedPointer<Texture>
ResourceManager::getTexture(const std::string& fileName) {
  // Buscar la textura
  auto it = m_textures.find(fileName);
  if (it != m_textures.end()) {
    return it->second;
  }

  // Mensaje de advertencia
  std::cerr << "[ResourceManager] Texture not found: " << fileName
    << ". Using default texture.\n";

  const std::string defaultKey = "Default";

  // Verificar si la textura por defecto ya está cargada
  auto defaultIt = m_textures.find(defaultKey);
  if (defaultIt != m_textures.end()) {
    return defaultIt->second;
  }

  // Cargar textura por defecto
  auto defaultTexture = EngineUtilities::MakeShared<Texture>(defaultKey, "png");
  m_textures[defaultKey] = defaultTexture;
  return defaultTexture;
}