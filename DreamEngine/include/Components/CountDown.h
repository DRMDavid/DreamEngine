/**
 * @file
 * @brief Clase para gestionar una cuenta regresiva.
 * @details Permite iniciar, detener y actualizar un temporizador regresivo,
 *  devolviendo cadenas de texto como números o "GO!" al finalizar.
 */

#pragma once
#include <string>
#include <algorithm>

 /**
  * @class Countdown
  * @brief Temporizador de cuenta regresiva.
  * @details Al iniciar, cuenta hacia atrás desde un tiempo determinado.
  *  Durante la cuenta, devuelve los segundos restantes como cadena.
  *  Al finalizar, devuelve "GO!".
  */
class Countdown {
public:
  /**
   * @brief Constructor de la cuenta regresiva.
   * @param seconds Tiempo total de cuenta en segundos (por defecto 3).
   */
  Countdown(float seconds = 3.f)
    : total_(seconds), remaining_(seconds), started_(false), finished_(false) {
  }

  /** @brief Destructor por defecto. */
  ~Countdown() = default;

  /**
   * @brief Inicia la cuenta regresiva.
   * @details Reinicia el tiempo restante y marca el temporizador como activo.
   */
  void start() {
    started_ = true;
    finished_ = false;
    remaining_ = total_;
  }

  /** @brief Detiene la cuenta regresiva sin reiniciarla. */
  void stop() {
    started_ = false;
  }

  /**
   * @brief Indica si la cuenta está en ejecución.
   * @return true si está iniciada, false en caso contrario.
   */
  bool isStarted() const {
    return started_;
  }

  /**
   * @brief Indica si la cuenta ha finalizado.
   * @return true si ha terminado, false en caso contrario.
   */
  bool isFinished() const {
    return finished_;
  }

  /**
   * @brief Actualiza la cuenta regresiva.
   * @param dt Tiempo transcurrido desde la última actualización (segundos).
   * @return Cadena con el número de segundos restantes o "GO!".
   */
  std::string tick(float dt) {
    if (!started_ || finished_) return "";
    remaining_ = std::max(0.f, remaining_ - dt);
    if (remaining_ == 0.f) {
      finished_ = true;
      started_ = false;
    }
    int num = static_cast<int>(remaining_ + 0.999f);
    if (finished_) return "GO!";
    return (num >= 1) ? std::to_string(num) : "GO!";
  }

private:
  float total_;     ///< Tiempo total de cuenta regresiva.
  float remaining_; ///< Tiempo restante actual.
  bool  started_;   ///< Estado de inicio de la cuenta.
  bool  finished_;  ///< Estado de finalización de la cuenta.
};
