/**
 * @file OrbitalSim.cpp
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
#define ASTEROIDS_MEAN_RADIO 4E11F
#define CANT_ASTEROIDES 1000

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
  float r = ASTEROIDS_MEAN_RADIO * sqrtf(fabs(l));
  float phi = getRandomFloat(0, 2 * M_PI);

  // Este es el Easter Egg: Los asteriodes aparecerán alineados
  // phi = 0;

  // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
  float v = sqrtf(GRAVITATIONAL_CONSTANT * centermass / r) * getRandomFloat(0.6F, 1.2F);
  float vy = getRandomFloat(-1E2F, 1E2F);

  OrbitalBody body;
  body.masa = 1E12F; // Typical asteroid weight: 1 billion tons
  body.radio = 2E3F; // Typical asteroid radio: 2km
  body.color = GRAY;
  body.posicion = {r * cosf(phi), 0, r * sinf(phi)};
  body.cte_velocidad = Vector3Scale({-v * sinf(phi), vy, v * cosf(phi)}, timeStep);
  body.acumulador_aceleracion = Vector3Zero();

  return body;
}

OrbitalSim *iniciarSimulacion(float timeStep)
{
  OrbitalSim *simulacion = (OrbitalSim *)malloc(sizeof(OrbitalSim));
  simulacion->timeStep = timeStep;
  simulacion->tiempoTranscurrido = 0.0f;
  simulacion->cantEstrellas = 0; 
  //Descomentar la siguiente linea (44) para introducir las esterllas alpha Centauri. 
  //La posición de las mismas era tan cercana a los planetas que comenzaban a "absorberlos"
  //simulacion->cantEstrellas = sizeof(alphaCentauriSystem)/sizeof(EphemeridesBody); 
  simulacion->cantPlanetas = sizeof(solarSystem)/sizeof(EphemeridesBody);
  simulacion->cantAsteroides = CANT_ASTEROIDES;
  simulacion->cantCuerpos = simulacion->cantEstrellas + simulacion->cantPlanetas + 
                            simulacion->cantAsteroides;

  float masaSol = solarSystem[0].mass;
  OrbitalBody *cuerpo = (OrbitalBody *)malloc(simulacion->cantCuerpos * sizeof(OrbitalBody));

  for (int i = 0; i < simulacion->cantEstrellas; i++)
  {
    cuerpo[i].masa = alphaCentauriSystem[i].mass;
    cuerpo[i].radio = alphaCentauriSystem[i].radius;
    cuerpo[i].color = alphaCentauriSystem[i].color;
    cuerpo[i].posicion = alphaCentauriSystem[i].position;
    cuerpo[i].cte_velocidad = Vector3Scale(alphaCentauriSystem[i].velocity, 
                                                                       timeStep);
    cuerpo[i].acumulador_aceleracion = Vector3Zero();
  }
  
  for (int i = 0; i < simulacion->cantPlanetas; i++)
  {
    cuerpo[i + simulacion->cantEstrellas].masa = solarSystem[i].mass;
    cuerpo[i + simulacion->cantEstrellas].radio = solarSystem[i].radius;
    cuerpo[i + simulacion->cantEstrellas].color = solarSystem[i].color;
    cuerpo[i + simulacion->cantEstrellas].posicion = solarSystem[i].position;
    cuerpo[i + simulacion->cantEstrellas].cte_velocidad = Vector3Scale(solarSystem[i].velocity, 
                                                                       timeStep);
    cuerpo[i + simulacion->cantEstrellas].acumulador_aceleracion = Vector3Zero();
  }

  for (int i = simulacion->cantEstrellas + simulacion->cantPlanetas; i < simulacion->cantCuerpos; i++)
  {
    cuerpo[i] = placeAsteroid(cuerpo[0].masa, timeStep);
  }

  simulacion->cuerpos = cuerpo;

  return simulacion;
}

void actualizarSimulacion(OrbitalSim *sim)
{
  for (int i = 0; i < sim->cantCuerpos; i++)
  {
    //Sólo se considera la fuerza gravitacional de las estrellas, que empíricamente se ha comprobado
    // que es la que predomina
    for (int j = 0 ; j < sim->cantEstrellas + 1 ; j++)
    {
      if(i == j)
        continue;
      Vector3 diff = Vector3Subtract(sim->cuerpos[i].posicion, sim->cuerpos[j].posicion);
      float longitud = Vector3Length(diff);
      longitud = longitud * longitud * longitud;
      diff = Vector3Scale(diff, GRAVITATIONAL_CONSTANT);
      sim->cuerpos[i].acumulador_aceleracion = Vector3Subtract(sim->cuerpos[i].acumulador_aceleracion,
                                               Vector3Scale(diff, sim->cuerpos[j].masa / longitud));
      //De no respetar el orden anterior y hacerlo como en las sigueintes lineas, se incurrirá en 
      //underflow

      //diff = Vector3Scale(diff, GRAVITATIONAL_CONSTANT/longitud);
      //sim->cuerpos[i].acumulador_aceleracion = Vector3Subtract(sim->cuerpos[i].acumulador_aceleracion, Vector3Scale(diff, sim->cuerpos[j].masa));
    }
    sim->cuerpos[i].posicion = Vector3Add(sim->cuerpos[i].posicion, sim->cuerpos[i].cte_velocidad);
    sim->cuerpos[i].posicion = Vector3Add(sim->cuerpos[i].posicion, 
                                          Vector3Scale(sim->cuerpos[i].acumulador_aceleracion, 
                                                       sim->timeStep * sim->timeStep));
  }
  sim->tiempoTranscurrido += sim->timeStep;
}

void liberarSimulacion(OrbitalSim *sim)
{
  free(sim->cuerpos);
  free(sim);
}
