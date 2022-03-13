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
#include <string.h>

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define CANT_CUERPOS 509

// Gets a random value between min and max
float getRandomFloat(float min, float max)
{
  return min + (max - min) * rand() / (float)RAND_MAX;
}

// Places an asteroid
//
// centerMass: mass of the most massive object in the star system
OrbitalBody placeAsteroid(float centerMass)
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
  OrbitalBody body;
  body.mass = 1E12F;  // Typical asteroid weight: 1 billion tons
  body.radius = 2E3F; // Typical asteroid radius: 2km
  body.color = GRAY;
  body.position = {r * cosf(phi), 0, r * sinf(phi)};
  body.velocity = {-v * sinf(phi), vy, v * cosf(phi)};
  body.aceleracion = Vector3Zero();

  return body;
}

// Make an orbital simulation
OrbitalSim *makeOrbitalSim(float timeStep)
{
  OrbitalBody *cuerpo = (OrbitalBody *)malloc(CANT_CUERPOS * sizeof(OrbitalBody));
  for (int i = 0; i < 9; i++)
  {
    cuerpo[i].mass = solarSystem[i].mass;
    cuerpo[i].radius = solarSystem[i].radius;
    cuerpo[i].color = solarSystem[i].color;
    cuerpo[i].position = solarSystem[i].position;
    cuerpo[i].velocity = solarSystem[i].velocity;
    cuerpo[i].aceleracion = Vector3{0, 0, 0};
  }

  for (int i=9; i<CANT_CUERPOS; i++)
  {
    cuerpo[i] = placeAsteroid(cuerpo[0].mass);
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
      Vector3 diff = Vector3Subtract(sim->cuerpos[i].position, sim->cuerpos[j].position);
      float longitud = Vector3Length(diff);
      longitud = longitud * longitud * longitud;
      diff = Vector3Scale(diff, GRAVITATIONAL_CONSTANT);
      if(i >= 9 && j >= 9)
      {
        Vector3 termino = Vector3Scale(diff, sim->cuerpos[j].mass/longitud);
        sim->cuerpos[i].aceleracion = Vector3Subtract(sim->cuerpos[i].aceleracion, termino);
        sim->cuerpos[j].aceleracion = Vector3Add(sim->cuerpos[j].aceleracion, termino);
      }
      sim->cuerpos[i].aceleracion = Vector3Subtract(sim->cuerpos[i].aceleracion, Vector3Scale(diff, sim->cuerpos[j].mass/longitud));
      sim->cuerpos[j].aceleracion = Vector3Add(sim->cuerpos[j].aceleracion, Vector3Scale(diff, sim->cuerpos[i].mass/longitud));
    }
    sim->cuerpos[i].velocity = Vector3Add(sim->cuerpos[i].velocity, Vector3Scale(sim->cuerpos[i].aceleracion, sim->timestep));
    sim->cuerpos[i].position = Vector3Add(sim->cuerpos[i].position, Vector3Scale(sim->cuerpos[i].velocity, sim->timestep));
    sim->cuerpos[i].aceleracion = Vector3Zero();
  }
  sim->tiempotranscurrido += sim->timestep;
}

void freeOrbitalSim(OrbitalSim *sim)
{
  free(sim->cuerpos);
  free(sim);
}
