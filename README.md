# 🤖 Jesucristo, el Robot Auto-Balancín del Futuro
 
<p align="center">
  <img src="docs/images/prototipo_final.jpg" alt="Prototipo final del robot" width="400"/>
</p>
<p align="center">
  <i>Robot auto-balancín de 2 ruedas con control PID, inspirado en "Jesucristo, el robot del futuro" de Zorman.</i>
</p>

## Índice
 
- [Descripción](#-descripción)
- [Galería del proyecto](#-galería-del-proyecto)
- [Estructura del repositorio](#-estructura-del-repositorio)
- [Integrantes](#-integrantes)
- [Curso y agradecimientos](#-curso-y-agradecimientos)
- [Referencias](#-referencias)

## Descripción
 
Este proyecto consiste en el diseño, manufactura y programación de un **mini robot auto-balancín** de 2 ruedas, inspirado estéticamente en *Jesucristo, el robot del futuro*, personaje de la canción homónima de Zorman.
 
El robot utiliza un **sensor/giroscopio MPU6050** para medir su ángulo de inclinación y un **controlador PID** (implementado en Arduino IDE) para calcular la señal de corrección enviada a dos motores DC mediante un driver L298N, logrando mantener el equilibrio de forma autónoma.
 
El desarrollo pasó por múltiples iteraciones de diseño CAD y prototipado físico (impresión 3D FDM y corte láser en acrílico), hasta llegar a una versión final que integra mejoras estéticas (cruz de LEDs, brazos de alambre, cabeza) y estructurales (mejor manejo de cableado y sujeción de componentes).
 
Este repositorio reemplaza el informe final del proyecto, documentando de forma abierta y reproducible todo el proceso de diseño, fabricación, electrónica y control.

## Galería del proyecto

| Prototipo final | Diagrama de control | Constantes PID |
|:---:|:---:|:---:|
| ![Prototipo](/Multimedia/Fotos/prototipo_final.jpg) | ![Diagrama de bloques](/Multimedia/Fotos/diagrama_bloques.png) | ![PID](/Multimedia/Fotos/constantes_pid.png) |


| Iteración 1 | Iteración 2 |
|:---:|:---:|
| <img src="/Multimedia/Fotos/Iteración%201/primer_diseño_cad_1.jpeg" alt="Iteración 1" width="400"/> | <img src="/Multimedia/Fotos/iteraciones_2_3_final.jpg" alt="Iteración 2" width="200"/> |

| Iteración 3 | Prototipo final |
|:---:|:---:|
| <img src="/Multimedia/Fotos/Ruta_de_tu_Iteracion_3.jpeg" alt="Iteración 3" width="200"/> | <img src="/Multimedia/Fotos/prototipo_final.jpg" alt="Prototipo final" width="200"/> |


> 📌 Más imágenes y videos del proceso y resultados experimentales en [`Multimedia/`](/Multimedia/).
 
---
 
## Estructura del repositorio
 
```
├── README.md                     # Este archivo
├── CAD/                          # Modelos y planos de fabricación
│   ├── Fusion360/                # Archivos nativos .f3d / .f3z
│   ├── STEP/                     # Exportaciones .step
│   └── planos_corte_laser/       # Planos 2D para corte láser (.dxf/.pdf)
├── componentes_y_materiales/     # Lista de materiales (BOM)
│   └── BOM.md (o .xlsx)
├── codigo/                       # Firmware del robot
│   ├── src/                      # Código Arduino IDE comentado
│   └── diagrama_logica.png       # Diagrama de flujo del programa
├── diagramas_electronicos_y_control/
│   ├── esquematico_electrico.png
│   └── diagrama_control_PID.png
└── registros_audiovisuales/      # Fotos y videos de avances/resultados
```
 
---
 
## Integrantes
 
- Esteban Ortiz
- Millaray González
- Valentina Gómez
- Vicente Leiva
---
 
## Curso y agradecimientos
 
Proyecto desarrollado para el curso **ME4250-2 — Mecatrónica**, semestre 2026-1, Departamento de Ingeniería Mecánica, Facultad de Ciencias Físicas y Matemáticas, Universidad de Chile.
 
Agradecemos al **Laboratorio de Mecatrónica y Robótica (LEMUR)** por facilitar el espacio, herramientas de fabricación (impresión 3D y corte láser) y equipos utilizados durante el desarrollo del proyecto.

Agradecemos también al **FabLab** por el corte láser e impresión 3D de las piezas del robot.
 
---
 
## Referencias
 
- Zorman. *Jesucristo, el robot del futuro* (canción, 2013). Inspiración estética del proyecto.
