/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"
#include "raymath.h"

struct OrbitalBody
{
  // const char* name; // Name
  float mass;       // [kg]
  float radius;     // [m]
  Color color;      // raylib color
  Vector3 position; // [m]
  Vector3 velocity; // [m/s]
  Vector3 aceleracion;
};

struct OrbitalSim
{
  float timestep;
  float tiempotranscurrido;
  int cantcuerpos;
  OrbitalBody *cuerpos;
};

OrbitalSim *makeOrbitalSim(float timeStep);
void updateOrbitalSim(OrbitalSim *sim);
void freeOrbitalSim(OrbitalSim *sim);

#endif
