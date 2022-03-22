/**
 * @file 	OrbitalBody.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief Clase para un cuerpo (cuerpo)
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

#ifndef ORBITAL_BODY_CPP
#define ORBITAL_BODY_CPP

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ASTEROIDS_MEAN_RADIO 4E11F
#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define _USE_MATH_DEFINES

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "OrbitalBody.h"
#include <stdlib.h>

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

OrbitalBody::OrbitalBody()
{

}

OrbitalBody::OrbitalBody(float masa, float radio, Color color, Vector3 posicion, Vector3 velocidad, float timeStep)
{
  this->masa = masa;
  this->radio = radio;
  this->color = color;
  this->posicion = posicion;
  this->cteVelocidad = Vector3Scale(velocidad, timeStep);
  this->acumuladorAceleracion = Vector3Zero();
}

OrbitalBody::OrbitalBody(float centermass, float timeStep)
{
  // Logit distribution
  float x = getRandomFloat(0, 1);
  float l = logf(x) - logf(1 - x) + 1;

  // https://mathworld.wolfram.com/DiskPointPicking.html
  float r = ASTEROIDS_MEAN_RADIO * sqrtf(fabs(l));
  float phi = getRandomFloat(0, 2 * M_PI);

  // Surprise!
  phi = 0;

  // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
  float v = sqrtf(GRAVITATIONAL_CONSTANT * centermass / r) * getRandomFloat(0.6F, 1.2F);
  float vy = getRandomFloat(-1E2F, 1E2F);

  this->masa = 1E12F;
  this->radio = 2E3F;
  this->color = GRAY;
  this->posicion = {r * cosf(phi), 0, r * sinf(phi)};
  this->cteVelocidad = Vector3Scale({-v * sinf(phi), vy, v * cosf(phi)}, timeStep);
  this->acumuladorAceleracion = Vector3Zero();
}

void OrbitalBody::incrementarPosicion(Vector3 incremento)
{
  this->posicion = Vector3Add(this->posicion, incremento);
}

float OrbitalBody::getMasa()
{
  return masa;
}
    
float OrbitalBody::getRadio()
{
  return radio;
}

Color OrbitalBody::getColor()
{
  return color;
}

Vector3 OrbitalBody::getPosicion()
{
  return posicion;
}

Vector3 OrbitalBody::getConstanteVelocidad()
{
  return cteVelocidad;
}

Vector3 OrbitalBody::getAcumuladorAceleracion()
{
  return acumuladorAceleracion;
}

void OrbitalBody::incrementarAcumuladorAceleracion(Vector3 incremento)
{
  acumuladorAceleracion = Vector3Add(acumuladorAceleracion, incremento);
}

void OrbitalBody::decrementarAcumuladorAceleracion(Vector3 decremento)
{
  acumuladorAceleracion = Vector3Subtract(acumuladorAceleracion, decremento);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

float OrbitalBody::getRandomFloat(float min, float max)
{
  return min + (max - min) * rand() / (float)RAND_MAX;
}

#endif