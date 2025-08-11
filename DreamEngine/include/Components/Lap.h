/**
 * @file
 * @brief Estructuras y tipos para cronometraje de vueltas.
 * @details Contiene la estructura `Lap`, utilizada para registrar el número
 *  de vuelta, tiempos de vuelta y progreso sobre el circuito.
 */

#pragma once
#include "Prerequisites.h"

 /**
	* @brief Datos de una vuelta en un circuito.
	* @details Acumula el tiempo actual de la vuelta, el mejor tiempo histórico y
	*  el tiempo total. También guarda el último checkpoint alcanzado y el
	*  progreso dentro del segmento actual del trazado.
	*/
struct
	Lap {
	/** @brief Índice de la vuelta actual (0-based o 1-based según el uso). */
	int   lap = 0;

	/** @brief Tiempo transcurrido de la vuelta en curso (segundos). */
	float currentLapTime = 0.f;

	/** @brief Mejor tiempo histórico de vuelta (segundos). */
	float bestLapTime = std::numeric_limits<float>::infinity();

	/** @brief Tiempo total acumulado (segundos). */
	float totalTime = 0.f;

	/** @brief Último checkpoint alcanzado en el circuito. */
	size_t checkpoint = 0;

	/**
	 * @brief Progreso dentro del segmento actual [0.0, 1.0].
	 * @details 0 representa el inicio del segmento y 1 el final.
	 */
	float  progressAlongSegment = 0.f;
};
