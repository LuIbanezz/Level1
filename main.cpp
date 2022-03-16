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
 * El uso de float está justificado pues:
 * La precisión para representar las magnitudes es óptima y
 * Los errores cometidos en los cálculos no son significativos
 * Utilizar un sistema de punto fijo requieriría demasiada memoria puesto que las magnitudes alcanzan valores muy grandes
 * Si se ejecutan las operaciones en el orden correcto, no se cometarán desbordes
 */

/*
 * Para optimizar el código se han hecho dos cosas:
 * 
 * 1. Despreciar la fuerza que ejercen los asteroides sobre los demás cuerpos:
 *  Puesto que la masa de los asteroides es pequeña, en cada cuerpo sólo se sumarán
 *  las fuerzas ejercidas por los planetas. Siendo n la cantidad total de cuerpos,
 *  la complejidad del algoritmo que actualiza la posición previa a esta modificación
 *  era O(n^2) por tener dos bucles anidados, mientras que ahora la complejidad es
 *  O(n), puesto que el bucle anidado sólo hace 9 iteraciones, que es lo mismo que la
 *  cantidad de planetas.
 * 
 * 2. Cambiar la función que dibuja las esferas
 *  
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "OrbitalSim.h"
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

  OrbitalSim sim = OrbitalSim(timeStep);

  // Game loop
  while (!WindowShouldClose())
  {
    sim.actualizarSimulacion();

    UpdateCamera(&camera);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    sim.renderizarSimulacion3D();
    DrawGrid(10, 10.0f);
    EndMode3D();

    sim.renderizarSimulacion2D();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
