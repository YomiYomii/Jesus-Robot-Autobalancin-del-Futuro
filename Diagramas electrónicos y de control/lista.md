# Diagramas electrónicos y de control
 
Esta carpeta contiene la documentación gráfica del sistema electrónico y de control del robot auto-balancín.
 
## Índice
 
- [Diagrama esquemático electrónico](#-diagrama-esquemático-electrónico)
- [Diagrama del sistema de control](#-diagrama-del-sistema-de-control)

## Diagrama esquemático electrónico
 
**Archivo:** `esquematico_electrico.png` (o `.pdf` / `.fzz` si se exporta desde Fritzing/KiCad)
 
Muestra las conexiones físicas entre todos los componentes electrónicos del robot:
 
- **Arduino** ↔ **MPU6050**: comunicación I2C (pines SDA/SCL), alimentación 3.3V/5V y GND
- **Arduino** ↔ **Driver L298N**: señales de control PWM (ENA/ENB) y dirección de giro (IN1-IN4)
- **Driver L298N** ↔ **Motores DC**: salidas de potencia hacia cada motor
- **Portapilas (2x12V)** ↔ **Driver L298N**: alimentación de potencia para los motores
- **Arduino** ↔ **LEDs amarillos**: salida digital/PWM para la cruz de luces
- Conexiones de **GND común** entre todas las etapas (Arduino, driver y alimentación de motores)

 
**Componentes representados:**
 
| Componente | Conexión principal |
|---|---|
| Arduino (Uno/Nano) | Controlador central |
| MPU6050 | I2C (SDA/SCL) |
| L298N | PWM + dirección (4 pines de control) |
| Motores DC (x2) | Salida de potencia del driver |
| Portapilas 2x12V | Alimentación de motores |
| LEDs amarillos | Salida digital Arduino |
 
---
 
## Diagrama del sistema de control
 
**Archivo:** `diagrama_control_PID.png`
 
Representa el **diagrama de bloques en lazo cerrado** que describe cómo el robot mantiene el equilibrio:
 
```
Ángulo de     ┌───┐    Error   ┌─────────────┐   Señal PWM   ┌──────────┐   Voltaje   ┌─────────┐  Ángulo real
referencia ──►│ Σ │──────────► │  Controlador│──────────────►│  Driver  │────────────►│ Motores │────────────► (Robot)
   (0°)       └───┘            │     PID     │               │  L298N   │              │   DC    │
                 ▲             └─────────────┘               └──────────┘              └─────────┘      │
                 │                                                                                      │
                 │                      Retroalimentación (ángulo medido)                               │
                 └─────────────────────────── Sensor MPU6050 ◄──────────────────────────────────────────┘
```
 
**Descripción del lazo de control:**
 
1. **Punto de referencia**: ángulo objetivo (0°, robot vertical)
2. **Comparador (Σ)**: calcula el error entre el ángulo deseado y el ángulo medido
3. **Controlador PID**: procesa el error y calcula la acción correctiva, usando las constantes:
   - `Kp = 6.4`
   - `Kd = 0.8`
   - `Ki = 0.05`
4. **Driver L298N**: convierte la señal de control en voltaje/PWM aplicado a los motores
5. **Motores DC**: generan el movimiento correctivo de las ruedas
6. **Planta (robot)**: responde físicamente, cambiando su ángulo de inclinación
7. **Sensor MPU6050**: mide el nuevo ángulo y retroalimenta el sistema, cerrando el lazo
Las constantes PID se obtuvieron mediante el método de **Ziegler-Nichols** como punto de partida, y se ajustaron posteriormente mediante **prueba y error** hasta lograr un balance estable.
