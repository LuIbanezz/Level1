/**
 * @file 	main.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief 	Archivo principal para la simulación del Sistema Solar
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */


/*
 * El uso de float está justificado pues la precisión para representar las magnitudes es óptima
 * Los errores cometidos en los cálculos no son significativos
 */

/*
 * La complejidad es O(n^2)...
 * 
 * 
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "orbitalSim.h"
#include "orbitalSimView.h"
#include <iostream>

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
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "EDA Orbital Simulation");

  Camera3D camera;
  camera.position = {10.0f, 10.0f, 10.0f};
  camera.target = {0.0f, 0.0f, 0.0f};
  camera.up = {0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  SetCameraMode(camera, CAMERA_FREE);

  SetCameraPanControl(MOUSE_BUTTON_LEFT);

  float fps = 60.0F;
  float timeMultiplier = 10*SECONDS_PER_DAY;
  float timeStep = timeMultiplier / fps;

  OrbitalSim *sim = makeOrbitalSim(timeStep);

  // Game loop
  while (!WindowShouldClose())
  {
    updateOrbitalSim(sim);
    cout << GetTime() << endl;

    UpdateCamera(&camera);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    renderOrbitalSim3D(sim);
    DrawGrid(10, 10.0f);
    EndMode3D();

    renderOrbitalSim2D(sim);
    EndDrawing();
    cout << GetTime() << endl << endl;
  }

  CloseWindow();

  freeOrbitalSim(sim);

  return 0;
}
