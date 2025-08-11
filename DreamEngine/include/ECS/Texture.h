/**
 * @file
 * @brief Componente para manejar texturas en el motor.
 * @details Permite cargar texturas desde archivo y acceder a ellas para
 *  renderizado. Hereda de `Component` y se clasifica como `TEXTURE`.
 */

#pragma once
#include "../Prerequisites.h"
#include "Component.h"

 /**
  * @class Texture
  * @brief Componente que gestiona una textura de SFML.
  * @details Facilita la carga, acceso y uso de texturas dentro del motor.
  *  La textura se carga desde un archivo en el constructor y queda disponible
  *  para otros componentes o sistemas.
  */
class Texture : public Component {
public:
  /** @brief Constructor por defecto. */
  Texture() = default;

  /**
   * @brief Constructor que carga una textura desde archivo.
   * @param textureName Nombre base del archivo (sin extensión).
   * @param extension Extensión del archivo (por defecto "png").
   * @details Si la carga falla, se muestra un mensaje de error en consola.
   */
  Texture(const std::string& textureName, const std::string& extension = "png") :
    m_textureName(textureName), m_extension(extension), Component(TEXTURE) {
    if (!m_texture.loadFromFile(m_textureName + "." + m_extension)) {
      std::cout << "Error de carga de textura: "
        << m_textureName << "." << m_extension << std::endl;
    }
  }

  /** @brief Destructor virtual por defecto. */
  virtual ~Texture() = default;

  /** @brief Inicializa el componente (no implementado para este tipo). */
  void start() override {};

  /**
   * @brief Actualiza el componente (no implementado para este tipo).
   * @param deltaTime Tiempo transcurrido desde el último frame (segundos).
   */
  void update(float deltaTime) override {};

  /**
   * @brief Renderiza el componente (no implementado para este tipo).
   * @param window Puntero compartido a la ventana de renderizado.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override {};

  /** @brief Libera recursos del componente (no implementado para este tipo). */
  void destroy() override {};

  /**
   * @brief Obtiene la textura gestionada por el componente.
   * @return Referencia a la textura de SFML.
   */
  sf::Texture& getTexture() {
    return m_texture;
  }

private:
  sf::Texture m_texture;     ///< Textura de SFML cargada en memoria.
  std::string m_textureName; ///< Nombre base del archivo de textura.
  std::string m_extension;   ///< Extensión del archivo de textura.
};
