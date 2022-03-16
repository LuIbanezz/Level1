/**
 * @file 	OrbitalSim.h
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief  Clase para controlar la simulación (encabezado)
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

#ifndef ORBITAL_SIM_H
#define ORBITAL_SIM_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "OrbitalBody.h"

/*******************************************************************************
 * CLASSES
 ******************************************************************************/
  
class OrbitalSim
{
  public:
    OrbitalSim(float timeStep);
    ~OrbitalSim();
    bool actualizarSimulacion();
    void renderizarSimulacion3D();
    void renderizarSimulacion2D();

  private:
    float timeStep;
    float tiempoTranscurrido;
    int cantPlanetas;
    int cantAsteroides;
    int cantCuerpos;
    OrbitalBody *cuerpos;
    float getRandomFloat(float min, float max);
    const char *getISODate(float currentTime);
};

#endif
