/**
 * @file 	OrbitalSim.h
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief Clase para un cuerpo (encabezado)
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

#ifndef ORBITAL_BODY_H
#define ORBITAL_BODY_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "raylib.h"
#include "raymath.h"

/*******************************************************************************
 * CLASSES
 ******************************************************************************/

class OrbitalBody
{
  public:
    OrbitalBody();
    OrbitalBody(float centermass, float timeStep); //constructor para el asteroide
    OrbitalBody(float masa, float radio, Color color, Vector3 posicion, Vector3 velocidad, float timeStep);
    void incrementarPosicion(Vector3 incremento);
    float getMasa();
    float getRadio();
    Color getColor();
    Vector3 getPosicion();
    Vector3 getConstanteVelocidad();
    Vector3 getAcumuladorAceleracion();
    void incrementarAcumuladorAceleracion(Vector3 incremento);
    void decrementarAcumuladorAceleracion(Vector3 incremento);

  private:
    float masa;       // [kg]
    float radio;     // [m]
    Color color;      // raylib color
    Vector3 posicion; // [m]
    Vector3 cte_velocidad; // velocidad inicial * timestep
    Vector3 acumulador_aceleracion;
    float getRandomFloat(float min, float max);
};

#endif