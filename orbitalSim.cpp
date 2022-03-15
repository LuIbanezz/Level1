/**
 * @file 	OrbitalSim.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief 	Definiciones relacionadas a datos de la simulación y los cuerpos
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "orbitalSim.h"
#include "ephemerides.h"
#include "raymath.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_radio 4E11F
#define CANT_CUERPOS 509

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

float getRandomFloat(float min, float max)
{
  return min + (max - min) * rand() / (float)RAND_MAX;
}

OrbitalBody placeAsteroid(float centermass, float timeStep)
{
  // Logit distribution
  float x = getRandomFloat(0, 1);
  float l = logf(x) - logf(1 - x) + 1;

  // https://mathworld.wolfram.com/DiskPointPicking.html
  float r = ASTEROIDS_MEAN_radio * sqrtf(fabs(l));
  float phi = getRandomFloat(0, 2 * M_PI);

  // Surprise!
  // phi = 0;

  // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
  float v = sqrtf(GRAVITATIONAL_CONSTANT * centermass / r) * getRandomFloat(0.6F, 1.2F);
  float vy = getRandomFloat(-1E2F, 1E2F);

    OrbitalBody body;
  body.masa = 1E12F;  // Typical asteroid weight: 1 billion tons
  body.radio = 2E3F; // Typical asteroid radio: 2km
  body.color = GRAY;
  body.posicion = {r * cosf(phi), 0, r * sinf(phi)};
  body.cte_velocidad = Vector3Scale({-v * sinf(phi), vy, v * cosf(phi)}, timeStep);
  body.acumulador_aceleracion = Vector3Zero();

  return body;
}

OrbitalSim *makeOrbitalSim(float timeStep)
{
  OrbitalBody *cuerpo = (OrbitalBody *)malloc(CANT_CUERPOS * sizeof(OrbitalBody));
  for (int i = 0; i < 9; i++)
  {
    cuerpo[i].masa = solarSystem[i].mass;
    cuerpo[i].radio = solarSystem[i].radius;
    cuerpo[i].color = solarSystem[i].color;
    cuerpo[i].posicion = solarSystem[i].position;
    cuerpo[i].cte_velocidad = Vector3Scale(solarSystem[i].velocity, timeStep);
    cuerpo[i].acumulador_aceleracion = Vector3Zero();
  }

  for (int i=9; i<CANT_CUERPOS; i++)
  {
    cuerpo[i] = placeAsteroid(cuerpo[0].masa, timeStep);
  }

  OrbitalSim *simulacion = (OrbitalSim *)malloc(sizeof(OrbitalSim));
  simulacion->cantcuerpos = CANT_CUERPOS;
  simulacion->timestep = timeStep;
  simulacion->tiempotranscurrido = 0.0f;
  simulacion->cuerpos = cuerpo;

  return simulacion;
}

void updateOrbitalSim(OrbitalSim *sim)
{
  for (int i = 0; i < sim->cantcuerpos; i++)
  {
    for (int j = i+1; j < sim->cantcuerpos; j++)
    {
      Vector3 diff = Vector3Subtract(sim->cuerpos[i].posicion, sim->cuerpos[j].posicion);
      float longitud = Vector3Length(diff);
      longitud = longitud * longitud * longitud;
      diff = Vector3Scale(diff, sim->timestep * sim->timestep * GRAVITATIONAL_CONSTANT);
      if(i >= 9 && j >= 9)
      {
        Vector3 termino = Vector3Scale(diff, sim->cuerpos[j].masa/longitud);
        sim->cuerpos[i].acumulador_aceleracion = Vector3Subtract(sim->cuerpos[i].acumulador_aceleracion, termino);
        sim->cuerpos[j].acumulador_aceleracion = Vector3Add(sim->cuerpos[j].acumulador_aceleracion, termino);
      }
      sim->cuerpos[i].acumulador_aceleracion = Vector3Subtract(sim->cuerpos[i].acumulador_aceleracion, Vector3Scale(diff, sim->cuerpos[j].masa/longitud));
      sim->cuerpos[j].acumulador_aceleracion = Vector3Add(sim->cuerpos[j].acumulador_aceleracion, Vector3Scale(diff, sim->cuerpos[i].masa/longitud));
    }
    sim->cuerpos[i].posicion = Vector3Add(sim->cuerpos[i].posicion, sim->cuerpos[i].cte_velocidad);
    sim->cuerpos[i].posicion = Vector3Add(sim->cuerpos[i].posicion, sim->cuerpos[i].acumulador_aceleracion);
  }
  sim->tiempotranscurrido += sim->timestep;
}

void freeOrbitalSim(OrbitalSim *sim)
{
  free(sim->cuerpos);
  free(sim);
}
