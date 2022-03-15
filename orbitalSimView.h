/**
 * @file 	OrbitalSimView.h
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief 	Definiciones relacionadas a datos de la simulación y los cuerpos
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "orbitalSim.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Renderiza la simulación (objetos 3D)
 *
 * @param timeStep
 */
void renderOrbitalSim3D(OrbitalSim *sim);

/**
 * @brief Renderiza la simulación (objetos 2D)
 *
 * @param timeStep
 */
void renderOrbitalSim2D(OrbitalSim *sim);

#endif
