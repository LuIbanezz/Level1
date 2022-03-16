/**
 * @file 	OrbitalSim.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief 	Clase para controlar la simulación (cuerpo)
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

#ifndef ORBITAL_SIM_CPP
#define ORBITAL_SIM_CPP

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "OrbitalSim.h"
#include "ephemerides.h"
#include "raymath.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdlib.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define CANT_CUERPOS 509
#define CANT_PLANETAS 9
#define CANT_ASTERIODES 500

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

OrbitalSim::OrbitalSim(float timeStep)
{
  OrbitalBody *cuerpo = new OrbitalBody[CANT_CUERPOS];
  for (int i = 0; i < CANT_PLANETAS ; i++)
  {
    cuerpo[i] = OrbitalBody(solarSystem[i].mass, solarSystem[i].radius, solarSystem[i].color, solarSystem[i].position, solarSystem[i].velocity, timeStep);
  }
  float masa_sol = solarSystem[0].mass;
  for(int i = CANT_PLANETAS; i < CANT_CUERPOS ; i++)
  {
    cuerpo[i] = OrbitalBody(masa_sol, timeStep);
  }
  
  this->cantcuerpos = CANT_CUERPOS;
  this->timestep = timeStep;
  this->tiempotranscurrido = 0.0f;
  cuerpos = cuerpo;
}

bool OrbitalSim::actualizarSimulacion()
{
  for (int i = 0; i<cantcuerpos ; i++)
  {
    for (int j = i+1; j < cantcuerpos; j++)
    {
      Vector3 diff = Vector3Subtract(cuerpos[i].getPosicion(), cuerpos[j].getPosicion());
      float longitud = Vector3Length(diff);
      longitud = longitud * longitud * longitud;
      diff = Vector3Scale(diff, timestep * timestep * GRAVITATIONAL_CONSTANT);
      if(i >= 9 && j >= 9)
      {
        Vector3 termino = Vector3Scale(diff,  cuerpos[j].getMasa()/longitud);
        cuerpos[i].decrementarAcumuladorAceleracion(termino);
        cuerpos[j].incrementarAcumuladorAceleracion(termino);
      }
      cuerpos[i].decrementarAcumuladorAceleracion(Vector3Scale(diff, (cuerpos[j].getMasa())/longitud));
      cuerpos[j].incrementarAcumuladorAceleracion(Vector3Scale(diff, cuerpos[i].getMasa()/longitud));
    }
    cuerpos[i].incrementarPosicion(cuerpos[i].getConstanteVelocidad());
    cuerpos[i].incrementarPosicion(cuerpos[i].getAcumuladorAceleracion());
    
  }
  tiempotranscurrido += timestep;
  return true;
}

void OrbitalSim::renderizarSimulacion3D()
{
  for (int i = 0; i < cantcuerpos; i++)
  {
    DrawSphereWires(Vector3Scale(cuerpos[i].getPosicion(), 1.0e-11f), 0.005f*logf(cuerpos[i].getRadio()), 2, 2, cuerpos[i].getColor());
    DrawPoint3D(cuerpos[i].getPosicion(), cuerpos[i].getColor());
  }
}

void OrbitalSim::renderizarSimulacion2D()
{
  DrawFPS(0,0);
  DrawText(getISODate(tiempotranscurrido), 0, 25, 25, GetColor(0xFFFFF));
}

OrbitalSim::~OrbitalSim()
{
  delete[] cuerpos;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

const char * OrbitalSim::getISODate(float currentTime)
{
  struct tm epoch_tm = {0, 0, 0, 1, 0, 122};
  time_t epoch = mktime(&epoch_tm);
  time_t local_time = epoch + (time_t)currentTime;
  struct tm *local_tm = localtime(&local_time);
  return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}

#endif