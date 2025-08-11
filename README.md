# DreamEngine

Motor de videojuegos 2D en C++ para prototipos de **carreras** y **simulaciones**.  
Arquitectura **ECS**, render con **SFML** y UI con **ImGui-SFML**.

---

## 🔎 Visión rápida
- Entidades ligeras · componentes de datos · sistemas especializados.
- Demo con HUD/menús, cronómetro, vueltas y meta.
- Extensible: añade **actores**, **sistemas** y **pistas** sin tocar el núcleo.

---

## 📚 Mapa de carpetas — módulo `DreamEngine/`

| Carpeta               | Rol principal                                                                 | Contiene / ejemplos                                                                 |
|-----------------------|-------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|
| `include/`            | Cabeceras públicas del motor                                                  | Interfaces de ECS, utilidades, tipos comunes, helpers                              |
| `src/`                | Implementación del runtime                                                    | Lógica del engine y de la demo, inicialización, glue de sistemas                   |
| `ECS/`                | Núcleo de la arquitectura ECS                                                 | Definición de entidad/ID, registro de componentes, mensajería/eventos              |
| `Components/`         | Datos puros que describen a las entidades                                     | `Transform`, `CShape`, `Lap`, físicos, input, tags, etc.                  |
| `Systems/`            | Lógica operativa que procesa conjuntos de componentes                         | `PlayerInputSystem`, `WaypointFollowSystem`, `SteeringSystem`, `RaceSystem`, `GUI` |
| `Circuits/`           | Datos de pistas y rutas                                                       | Waypoints, checkpoints, meta, configuraciones de trazado                            |
| `Resources/` *(opc.)* | Abstracciones para acceso a recursos                                          | Carga/gestión de texturas, fuentes, cachés                                          |

---

## ✨ Módulos clave
- **ECS** – gestión de entidades, componentes y ciclo del juego.  
- **Gameplay** – vueltas, posiciones, *countdown* y meta.  
- **AI** – *waypoints* + *steering* (Seek / Arrive / Pursuit).  
- **UI** – menús, HUD y paneles de depuración con ImGui-SFML.  
- **Assets** – carga centralizada de texturas/fuentes.

---

## 🚀 Arranque rápido (Windows)
1) Abre `DreamEngine.sln` en **Visual Studio 2019/2022**.  
2) Selecciona **x64** y **Release**.  
3) Si no usas *vendoring*:
   - **C/C++ → Additional Include Dirs**: `...\SFML\include; ...\imgui-sfml\include`
   - **Linker → Additional Library Dirs**: `...\SFML\lib`
   - **Linker → Input**: `sfml-graphics.lib; sfml-window.lib; sfml-system.lib;` (+ lo que uses)
4) Compila (`Ctrl+Shift+B`) y ejecuta (`F5`).  
5) Asegura que `assets/` esté **junto al `.exe`**.

---

## 🎮 Controles (demo)
WASD = mover · Ratón = cámara · F1 = HUD · Esc = salir

---

## 🧩 Extender
- **Actor nuevo** → hereda de `Actor` y agrega componentes (p. ej. `Transform`, `CShape`).  
- **Sistema nuevo** → clase que itere entidades con los componentes requeridos y regístrala en el *update loop*.  
- **Pista nueva** → define *waypoints* en `Circuits/` y selecciónala desde la demo.

---

## 🧰 Requisitos
- **C++17** (MSVC 2019/2022, x64)  
- **SFML** `3.0.0`  
- **ImGui-SFML** compatible

---

## 🧪 Tips
- Si no aparece la UI: revisa init y ciclo `new frame → draw → render` de ImGui-SFML.  
- Si faltan DLLs: coloca las de SFML junto al ejecutable.  
- Usa rutas relativas para *assets* (carpeta `assets/`).

---

## 📜 Licencia
**MIT** — ver `LICENSE`.
