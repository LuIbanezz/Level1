/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#define _USE_MATH_DEFINES

#include <math.h>

#include "orbitalSim.h"
#include "ephemerides.h"
#include "raymath.h"
#include <stdlib.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define CANT_CUERPOS 10

// Gets a random value between min and max
float getRandomFloat(float min, float max)
{
  return min + (max - min) * rand() / (float)RAND_MAX;
}

// Places an asteroid
//
// centerMass: mass of the most massive object in the star system
void placeAsteroid(OrbitalBody *body, float centerMass)
{
  // Logit distribution
  float x = getRandomFloat(0, 1);
  float l = logf(x) - logf(1 - x) + 1;

  // https://mathworld.wolfram.com/DiskPointPicking.html
  float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabs(l));
  float phi = getRandomFloat(0, 2 * M_PI);

  // Surprise!
  // phi = 0;

  // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
  float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
  float vy = getRandomFloat(-1E2F, 1E2F);

  // Fill in with your own fields:
  // body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
  // body->radius = 2E3F; // Typical asteroid radius: 2km
  // body->color = GRAY;
  // body->position = {r * cosf(phi), 0, r * sinf(phi)};
  // body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

// Make an orbital simulation
OrbitalSim *makeOrbitalSim(float timeStep)
{
  OrbitalBody *cuerpo = (OrbitalBody *)malloc(CANT_CUERPOS * sizeof(OrbitalBody));
  for (int i = 0; i < CANT_CUERPOS; i++)
  {
    cuerpo[i].mass = solarSystem[i].mass;
    cuerpo[i].radius = solarSystem[i].radius;
    cuerpo[i].color = solarSystem[i].color;
    cuerpo[i].position = solarSystem[i].position;
    cuerpo[i].velocity = solarSystem[i].velocity;
    cuerpo[i].aceleracion = Vector3{0, 0, 0};
  }
  OrbitalSim *simulacion = (OrbitalSim *)malloc(sizeof(OrbitalSim));
  simulacion->cantcuerpos = CANT_CUERPOS;
  simulacion->timestep = timeStep;
  simulacion->tiempotranscurrido = 0.0f;
  simulacion->cuerpos = cuerpo;

  return simulacion;
}

// Simulates a timestep
void updateOrbitalSim(OrbitalSim *sim)
{
  for (int i = 0; i < sim->cantcuerpos; i++)
  {
    for (int j = 0; j < sim->cantcuerpos; j++)
    {
      if (i == j)
        continue;

      Vector3 aux1 = Vector3Subtract(sim->cuerpos[i].position, sim->cuerpos[j].position);
      float aux2 = Vector3Length(aux1);
      aux2 = aux2 * aux2 * aux2;
      aux1 = Vector3Scale(aux1, GRAVITATIONAL_CONSTANT * sim->cuerpos[j].mass / aux2);
      sim->cuerpos[i].aceleracion = Vector3Subtract(sim->cuerpos[i].aceleracion, aux1);
    }
    Vector3 aux3 = Vector3Scale(sim->cuerpos[i].aceleracion, sim->timestep);
    sim->cuerpos[i].velocity = Vector3Add(sim->cuerpos[i].velocity, aux3);

    Vector3 aux4 = Vector3Scale(sim->cuerpos[i].velocity, sim->timestep);
    sim->cuerpos[i].position = Vector3Add(sim->cuerpos[i].position, aux4);
  }
}

void freeOrbitalSim(OrbitalSim *sim)
{
  free(sim->cuerpos);
  free(sim);
}
