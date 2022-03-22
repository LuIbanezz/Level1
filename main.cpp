/**
 * @file main.cpp
 * @authors	Tomás Castro, Lucía Ibañez
 *
 * @brief Archivo principal para la simulación del Sistema Solar
 * 
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */


/*
 * El uso de float (formato IEEE754 coma flotante 32 bits) está justificado pues:
 *
 * - La precisión para representar las magnitudes es óptima y los errores cometidos en los cálculos 
 * no son significativos
 * - Si se ejecutan las operaciones en el orden correcto, no se cometarán desbordes (ver 
 * actualizarSimulacion() en OrbitalSim.cpp) y por lo tanto no es necesario usar variables flotantes
 *  de 64 bits (double)
 * - Utilizar un sistema de coma fija requieriría demasiada memoria puesto que las magnitudes 
 * alcanzan valores de órdenes muy grandes o pequeños, por lo que se necesitarían muchísimos bits 
 * para lograr cubrir los rangos de magnitud (en float se guarda la información de signo exponente y
 * mantisa por separado pudiendo cubrir grandes rangos sin que la variable ocupe demasiada memoria)
 */

/*
 * Para optimizar el código se han hecho dos cosas:
 * 
 * 1. Despreciar la fuerza gravitacional que ejercen los asteroides y planetas:
 *  Sólo se tiene en cuenta la fuerza que ejercen las estrellas, de esta forma se evitan cómputos
 *  que no son significativos sobre la fuerza resultante. Siendo n la cantidad total de cuerpos, la 
 *  complejidad del algoritmo que actualizaba la posición previa a esta modificación era O(n^2) por 
 *  tener dos bucles anidados que iteraban hasta lograr considerar las interacciones entre todos los
 *  cuerpos, mientras que ahora la complejidad es O(n), puesto que las iteraciones que realiza el 
 *  bucle anidado depende de la cantidad de estrellas (máximo 3 si se incluyen las de Alpha Centauri)
 *  y no de la cantidad de planetas
 * 
 * 2. Cambiar el modo en que se realizan las asignaciones de velocidad y posición:
 *  El objetivo final de llamar a actualizarSimulacion() es obtener la posición de los planetas para
 *  el nuevo instante de tiempo. Ahora bien, viendo las fórmulas para el cómputo de v(n+1) y x(n+1),
 *  si se sustituye la expresión de v(n+1) en la de x(n+1), se llega a que 
 *  x(n+1) = x(n) + v(0)*delta_t + [a*(n-1) + a(n)]*(delta_t)^2 ; donde a* es la suma de todas las 
 *  aceleraciones anteriores. Es decir, no sólo se puede ahorrar el cálculo para la velocidad, sino 
 *  que además no es necesario poner en cero la aceleración de cada cuerpo antes de volver a 
 *  computarla en el n-ésimo instante, sino que basta sumar sobre la misma variable la nueva 
 *  aceleración resultante para obtener a*(n-1) + a(n)
 * 
 * 3. Cambiar la función que dibuja las esferas:
 *  Realizando testeos sobre el tiempo de ejecución de cada función hemos descubierto que la rutina
 *  para renderizar las esferas es relativamente lenta comparada a las demás rutinas relacionadas a
 *  la interfaz gráfica. Se ha utilizado en su reemplazo a DrawSphereWires, la cual dibuja esferas en
 *  forma incompleta, pero ahorrando tiempo y aumentando los cuadros por segundo.
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

  OrbitalSim *sim = iniciarSimulacion(timeStep);

  // Game loop
  while (!WindowShouldClose())
  {
    actualizarSimulacion(sim);
    cout << GetTime() << endl;

    UpdateCamera(&camera);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    renderizarSimulacion3D(sim);
    DrawGrid(10, 10.0f);
    EndMode3D();

    renderizarSimulacion2D(sim);
    EndDrawing();
    cout << GetTime() << endl << endl;
  }

  CloseWindow();

  liberarSimulacion(sim);

  return 0;
}
