// ================================================================
// ===        Jesus Robot Autobalancín del Futuro               ===
// ================================================================

// Código INO del proyecto de mecatrónica jesus el robot autobalancin 
// del futuro.

//  Integrantes:
// Vicente Leiva
// Millaray González
// Valentina Gómez
// Esteban Ortiz


// ================================================================
// ===                    Librerias Usadas                      ===
// ================================================================


#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h> // Inclusión forzada para evitar errores de tipo

MPU6050 mpu;

// ================================================================
// ===         DEFINIMOS VARIABLES GLOBALES Y PINES             ===
// ================================================================

// Pines L298N
const int PinL1 = 7;    
const int PinL2 = 8;    
const int PinR1 = 5;    
const int PinR2 = 6;    
const int PwmL  = 10;   
const int PwmR  = 9;    

// CONSTANTES PID AJUSTADAS 
float Kp = 80.0;
float Kd = 1;  
float Ki = 0.05; 

float setpoint = 0.0;   // Se calculará automáticamente en el setup dinámico
float angulo_actual = 0;    //Valor asociado al ángulo que detecta en el momento el sensor 
float error = 0, error_anterior = 0;
float P = 0, I = 0, D = 0;  //Valores asociados a PID
float derivada_filtrada = 0; // Filtro para robustez ante perturbaciones
float u = 0;     //Velocidad motor        

unsigned long tiempo_anterior = 0; //Usado para definir la diferencia entre angulo y tiempo

// Variables del MPU6050
bool dmpReady = false;
uint8_t mpuIntStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

volatile bool mpuInterrupt = false;
void dmpDataReady() { mpuInterrupt = true; }

// Variables para Auto-Setpoint (Calibración Dinámica del Centro de Masa)
bool setpoint_configurado = false;
float suma_angulos = 0;
int contador_lecturas = 0;

// ================================================================
// ===                         SETUP DEL ROBOT                  ===
// ================================================================

void setup() {
    pinMode(PinR1, OUTPUT); pinMode(PinR2, OUTPUT); pinMode(PwmR, OUTPUT);
    pinMode(PinL1, OUTPUT); pinMode(PinL2, OUTPUT); pinMode(PwmL, OUTPUT);

    // Inicialización I2C Segura con Salvavidas Anti-Congelamiento
    Wire.begin();
    Wire.setClock(400000); 
    Wire.setWireTimeout(3000, true); // Mitigación de I2C Lockup por ruido electromagnético

    Serial.begin(115200); 
    while (!Serial);

    mpu.initialize();
    uint8_t devStatus = mpu.dmpInitialize();

    // OFFSETS DE CALIBRACIÓN ESTÁTICA (Valores fijos de la estructura, definidos usando IMUZero)
    mpu.setXAccelOffset(605);
    mpu.setYAccelOffset(-4743);
    mpu.setZAccelOffset(1481);
    mpu.setXGyroOffset(-11);
    mpu.setYGyroOffset(30);
    mpu.setZGyroOffset(17);

    if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        attachInterrupt(digitalPinToInterrupt(2), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        Serial.println(F("Error inicializando DMP"));
    }
}

// ================================================================
// ===                      LOOP PRINCIPAL                      ===
// ================================================================

void loop() {
    if (!dmpReady) return;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        leerSensores();
        
        // --- RUTINA DE AUTO-SETPOINT (Calibración Adaptativa al Centro de Masa) ---
        if (!setpoint_configurado) {
            unsigned long tiempo_actual = millis();
            
            if (tiempo_actual < 3000) {
                // Seg 0 a 3: Estabilización del filtro inercial del MPU
                Serial.println("Estabilizando filtro inercial...");
            } 
            else if (tiempo_actual >= 3000 && tiempo_actual < 6000) {
                // Seg 3 a 6: Muestreo y acumulación para cálculo del punto de equilibrio real
                suma_angulos += angulo_actual;
                contador_lecturas++;
                Serial.println("CALIBRANDO... ¡MANTEN EL ROBOT EN EQUILIBRIO FÍSICO!");
            } 
            else {
                // Cálculo del valor esperado del Setpoint mecánico
                setpoint = suma_angulos / contador_lecturas;
                setpoint_configurado = true;
                tiempo_anterior = millis(); // Inicialización limpia del delta de tiempo para el PID
                Serial.print("=== SETPOINT DINÁMICO FIJADO EN: ");
                Serial.print(setpoint);
                Serial.println(" GRADOS ===");
            }
            return; // Bloqueo de seguridad: No energizar motores durante la calibración
        }
        // --------------------------------------------------------------------------

        // Manejo de Seguridad Estructural (Corte por Ángulo de Caída)
        if (abs(error) < 45.0) { 
            calcularPID();
            controlarMotores(u);
        } else {
            controlarMotores(0); // Parada de emergencia de los actuadores
            I = 0; 
            u = 0;
        }

        imprimirDatosPlotter();
    }
}

// ================================================================
// ===                   MÓDULO 1: SENSORES                     ===
// ================================================================

void leerSensores() {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    
    // Extracción limpia del Pitch en grados usando el DMP interno
    angulo_actual = ypr[1] * 180.0 / PI; 
}

// ================================================================
// ===                     MÓDULO 2: PID                        ===
// ================================================================

void calcularPID() {
    unsigned long tiempo_actual = millis();
    float dt = (tiempo_actual - tiempo_anterior) / 1000.0;
    if (dt <= 0.0) dt = 0.01; 
    tiempo_anterior = tiempo_actual;

    // Cálculo del error dinámico respecto al setpoint autocalibrado
    error = setpoint - angulo_actual;

    // Acción Proporcional
    P = Kp * error;

    // Acción Integral con Mitigación de Saturación (Anti-Windup)
    I += Ki * error * dt;
    if (error * error_anterior < 0) { I = 0; } // Reseteo al cruzar por el cero
    if (I > 200) I = 200;                      // Saturación de seguridad
    if (I < -200) I = -200;

    // Acción Derivativa con Filtro Pasa-Bajas para Atenuación de Ruido (Robustez)
    float derivada_cruda = (error - error_anterior) / dt;
    derivada_filtrada = (0.7 * derivada_cruda) + (0.3 * derivada_filtrada); // Suaviza picos por perturbación
    D = Kd * derivada_filtrada;

    // Señal de control u(t)
    u = P + I + D;
    error_anterior = error;
}

// ================================================================
// ===                   MÓDULO 3: MOTORES                      ===
// ================================================================

void controlarMotores(float senal_control) {
    int pwm_salida = abs(senal_control);
    
    if (pwm_salida > 255) pwm_salida = 255;
    
    // Compensación de la zona muerta por fricción estática
    if (pwm_salida > 0 && pwm_salida < 15) pwm_salida = 15; 

    if (senal_control > 0) {
        digitalWrite(PinL1, HIGH); digitalWrite(PinL2, LOW);
        digitalWrite(PinR1, HIGH); digitalWrite(PinR2, LOW);
        analogWrite(PwmL, pwm_salida);
        analogWrite(PwmR, pwm_salida);
    } 
    else if (senal_control < 0) {
        digitalWrite(PinL1, LOW); digitalWrite(PinL2, HIGH);
        digitalWrite(PinR1, LOW); digitalWrite(PinR2, HIGH);
        analogWrite(PwmL, pwm_salida);
        analogWrite(PwmR, pwm_salida);
    } 
    else {
        analogWrite(PwmL, 0);
        analogWrite(PwmR, 0);
    }
}

// ================================================================
// ===                   SERIAL PLOTTER                         ===
// ================================================================

void imprimirDatosPlotter() {
    // Entrega las métricas cuantitativas exactas exigidas por la rúbrica final
    Serial.print("Setpoint:"); Serial.print(setpoint); Serial.print(",");
    Serial.print("Angulo:"); Serial.print(angulo_actual); Serial.print(",");
    Serial.print("Error:"); Serial.print(error); Serial.print(","); 
    Serial.print("PWM_u:"); Serial.println(u);
}