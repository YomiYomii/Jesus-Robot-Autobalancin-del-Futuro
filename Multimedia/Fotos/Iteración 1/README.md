Acá se encuentran los registro de la primera interación

# Avance 1 — Diseño del Prototipo
 
> Resumen del primer avance del proyecto semestral. Etapa enfocada en definir la geometría, el hardware y validar el control abierto de los motores.
 
## Contexto y objetivo
 
Objetivo central: diseño y fabricación de un mini robot auto-balancín de 2 ruedas, con dos pilares técnicos definidos desde el inicio:
 
- **Control PID**: compensación continua de perturbaciones externas mediante retroalimentación en tiempo real.
- **Estabilidad**: ajuste del centro de masa (CM) para minimizar el momento de inercia y optimizar la respuesta del motor.
## Diseño conceptual
 
Se definió la identidad estética del proyecto bajo el nombre **"Jesucristo SuperStar"**, con un marco general futurista:
 
- Cuerpo de acrílico
- Mini parlantes
- Luces LED
- Soportes internos
## Diseño geométrico
 
**Parámetros seleccionados:**
 
| Parámetro | Valor |
|---|---|
| Largo (L) | 80 mm |
| Altura (H) | 60 mm |
| Ancho (A) | 180 mm |
 
**Cumplimiento de restricciones:**
 
- Condición 1: `H ≤ 2L` → `60 ≤ 180` ✅
- Condición 2: `L = R` → diámetro de rueda aproximado 80 mm ✅
**Justificación geométrica:**
 
Se seleccionó un ancho (A) significativamente mayor que el largo y la altura, con el objetivo de que el robot posea un momento de inercia lateral elevado. Esto previene vuelcos hacia los costados y facilita el control PID, concentrando el eje de inclinación en el sentido longitudinal (problema de control reducido a un solo eje).
 
**Criterios de clasificación adicionales:**
 
- **Criterio A:** `H ≤ 60 mm`, `R ≤ 80 mm` — perfil bajo que permite mantener el CM cerca del eje.
- **Criterio B:** `H ≤ A` — relación que ayuda a la optimización de la estabilidad lateral.
## Selección de componentes
 
| Componente | Función |
|---|---|
| Arduino Uno | Microcontrolador y lógica de control PID |
| Driver L298N | Control de potencia para los motores DC |
| Motores DC + reductora (x2) | Actuadores para el balanceo y movimiento |
| Carcasa + baterías | Suministro de energía independiente |
| Cables + protoboard | Conexiones eléctricas |
| Luces LED + miniparlantes | Elementos de construcción y performance |
 
## Diseño CAD
 
- El CAD contempla los componentes anteriores ensamblados dentro de un armazón de acrílico.
- Altura total en conjunto con motores: **87.80 mm**.
- Corresponde a la presentación del prototipo inicial, previo a las iteraciones estéticas posteriores (cabeza, cruz de LEDs).
## Programación de motores
 
Se desarrolló y probó en TinkerCad un circuito base para controlar los motores vía potenciómetro:
 
1. Lectura analógica del potenciómetro con `analogRead(potPin)`.
2. Mapeo del valor a rango PWM (0–255) con `map(rawValue, 0, 1023, 0, 255)`.
3. Lógica de **3 rangos** `[0,123]`, `[124,132]`, `[133,255]`, con **zona muerta** en el rango central (124–132) para evitar arranques erráticos cerca del punto neutro.
4. Definición de dirección de giro con `digitalWrite(inX, HIGH/LOW)`.
5. Aplicación de velocidad final con `analogWrite(pin, motorSpeed)`.
```
// Definición de pines para el driver
int enA = 10;
int in1 = 7;
int in2 = 6;
int enB = 9;
int in3 = 4;
int in4 = 3;
 
// Pin de lectura del potenciómetro
const int potPin = A0;
 
// Zona muerta (motores en 0 RPM)
const int limiteInferior = 124;
const int limiteSuperior = 132;
```
