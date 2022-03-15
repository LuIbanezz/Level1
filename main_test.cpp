/**
 * @file 	main_test.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief 	Archivo principal #2
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <iostream>
#include "orbitalSim.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SECONDS_PER_DAY 86400.0F

/*******************************************************************************
 *******************************************************************************
                                    MAIN
 *******************************************************************************
 ******************************************************************************/

using namespace std;
int main()
{
  float fps = 60.0F;                            // frames per second
  float timeMultiplier = 100 * SECONDS_PER_DAY; // Simulation speed: 100 days per real second
  float timeStep = timeMultiplier / fps;

  OrbitalSim *sim = makeOrbitalSim(timeStep);

  updateOrbitalSim(sim);

  return 0;
}
