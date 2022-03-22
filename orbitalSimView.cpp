/**
 * @file OrbitalSimView.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief Definiciones relacionadas a datos de la simulación y los cuerpos
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <time.h>
#include "orbitalSimView.h"

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

const char *getISODate(float currentTime)
{
  struct tm epoch_tm = {0, 0, 0, 1, 0, 122};
  time_t epoch = mktime(&epoch_tm);
  time_t local_time = epoch + (time_t)currentTime;
  struct tm *local_tm = localtime(&local_time);
  return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}

void renderizarSimulacion3D(OrbitalSim *sim)
{
  for (int i = 0; i < sim->cantCuerpos; i++)
  {
    DrawSphereWires(Vector3Scale(sim->cuerpos[i].posicion, 1.0e-11f), 0.005f*logf(sim->cuerpos[i].radio), 2, 2, sim->cuerpos[i].color);
    DrawPoint3D(sim->cuerpos[i].posicion, sim->cuerpos[i].color);
  }
}

void renderizarSimulacion2D(OrbitalSim *sim)
{
  DrawFPS(0,0);
  DrawText(getISODate(sim->tiempoTranscurrido),0,25,25,GetColor(0xFFFFF));
}
