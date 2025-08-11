/**
 * @file ResourceManager.cpp
 * @brief Implementación del administrador de recursos del motor.
 * @details Este módulo se encarga de la carga y recuperación de texturas,
 *          manteniendo un caché para evitar cargas repetidas.
 */

#include "ResourceManager.h"

 /**
	* @brief Carga una textura en memoria si no existe ya en el caché.
	* @param fileName Nombre base del archivo de textura (sin extensión).
	* @param extension Extensión del archivo (por ejemplo: "png", "jpg").
	* @return true si la textura fue cargada o ya existía en el caché.
	* @note Si la textura ya existe, no se vuelve a cargar.
	* @post La textura queda disponible a través de @ref getTexture.
	*/
bool
ResourceManager::loadTexture(const std::string& fileName,
	const std::string& extension)
{
	// Verificar si la textura ya fue cargada
	if (m_textures.find(fileName) != m_textures.end()) {
		return true;
	}

	// Crear y almacenar nueva textura
	auto texture = EngineUtilities::MakeShared<Texture>(fileName, extension);
	m_textures[fileName] = texture;
	return true;
}

/**
 * @brief Obtiene una textura previamente cargada por nombre.
 * @param fileName Nombre base de la textura solicitada.
 * @return Puntero compartido a la textura solicitada.
 * @details Si la textura no existe en el caché:
 *  - Muestra un mensaje de advertencia.
 *  - Retorna la textura por defecto ("Default"), cargándola si es necesario.
 * @warning Si la textura por defecto tampoco existe, se crea en el momento.
 */
EngineUtilities::TSharedPointer<Texture>
ResourceManager::getTexture(const std::string& fileName)
{
	auto it = m_textures.find(fileName);
	if (it != m_textures.end()) {
		return it->second;
	}

	// Advertir al usuario y usar textura por defecto
	std::cerr << "[ResourceManager] Texture not found: "
		<< fileName << ". Using default texture. \n";

	const std::string defaultKey = "Default";

	auto defaultIt = m_textures.find(defaultKey);
	if (defaultIt != m_textures.end()) {
		return defaultIt->second;
	}

	// Crear textura por defecto si no existe
	auto defaultTexture = EngineUtilities::MakeShared<Texture>(defaultKey, "png");
	m_textures[defaultKey] = defaultTexture;
	return defaultTexture;
}
