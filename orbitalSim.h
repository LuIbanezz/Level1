/**
 * @file 	OrbitalSim.h
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief 	Definiciones relacionadas a datos de la simulación y los cuerpos
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "raylib.h"
#include "raymath.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

struct OrbitalBody
{
  // const char* name; // Name
  float masa;       // [kg]
  float radio;     // [m]
  Color color;      // raylib color
  Vector3 posicion; // [m]
  Vector3 cte_velocidad; // velocidad inicial * timeStep
  Vector3 acumulador_aceleracion;
};

struct OrbitalSim
{
  float timeStep;
  float tiempoTranscurrido;
  int cantCuerpos;
  int cantEstrellas;
  int cantPlanetas;
  int cantAsteroides;
  OrbitalBody *cuerpos;
};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa la simulación
 *
 * @param timeStep
 */
OrbitalSim *iniciarSimulacion(float timeStep);

/**
 * @brief Actualiza la simulación
 *
 * @param sim
 */
void actualizarSimulacion(OrbitalSim *sim);

/**
 * @brief Actualiza la simulación
 *
 * @param sim
 */
void liberarSimulacion(OrbitalSim *sim);

#endif
