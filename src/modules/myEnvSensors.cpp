#include "myEnvSensors.h"


#include "my_ms5837.h"
#include "ds12b20.h"
#include "turbidity.h"

#include "settings.h"


#define ___DEBUG_MODE___



envSensorData envSensorData_stamp;

envSensorData get_envData_stamp()
{
  return envSensorData_stamp;
};


//////////////////////////////////////////////////////////////////////////////
///////////////////////   TDS SENSOR   ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//	The MS5837-30BA is a high-resolution pressure and temperature sensor    //
//	from TE Connectivity (TE) with I2C bus interface.						//
//////////////////////////////////////////////////////////////////////////////

#define ADC_MAX_VALUE 1023

const float   VCC    = 5.0f;                      // Напряжение питания Arduino.
const float   Vccm   = 3.3f;                      // Напряжение питания ОУ модуля        (внутрисхемно используется 3,3В).
const float   Ka     = 1000.0f;                   // Множитель степенной функции         (определяется калибровкой модуля). Можно получить функцией getKa().
const float   Kb     = -5.0f;                     // Степень   степенной функции         (определяется калибровкой модуля). Можно получить функцией getKb().
const float   Kt     = 0.02f;                     // Температурный коэффициент           (зависит от состава жидкости).
const float   Kp     = 0.5f;                      // Коэффициент пересчёта               (зависит от состава жидкости).
const float   Kf     = 0.85f;                     // Коэффициент передачи ФВЧ+ФНЧ модуля (зависит от частоты переменного тока используемого для проведения измерений).
const float   T      = 25.0f;                     // Опорная температура в °C.
const float   t      = 25.0f;                     // Температура исследуемой жидкости в °C.


void tds_init() {
  pinMode(TDS_PIN, INPUT);
}

int get_rawTDS() {
  return analogRead(TDS_PIN);
}

// Получаем удельную электропроводность
float get_conductivity(int rawTDS) {
  return float(rawTDS) * VCC / ADC_MAX_VALUE;
}

// Приводим удельную электропроводность в мСм/см
float get_normConductivity(float rawConductivity) {
  return Ka * pow((Vccm - Kf * rawConductivity) / 2, Kb) / 1000;
}

// Получаем минерализацию жидкости в мСм/см
float get_EC(float normConductivity) {
  return normConductivity / (1 + Kt * (t - T));
}

// Получаем загрязнение в мг/л
float get_TDS(float EC) {
  return EC * Kp;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////  PH-METER  /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define PH_PIN 36

void ph_init() {
  pinMode(PH_PIN, INPUT);
}

float get_ph() {
  int phRaw = analogRead(PH_PIN);
  float phValue = (float)phRaw * 3.3 / 4096 / 6; //convert the analog into millivolt
  return 3.5 * phValue; //convert the millivolt into pH value
}



//////////////////////////////////////////////////////////////////////////////
///////////////////   COMMON SENSOR FUNCTIONS   //////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void envSensors_init() {
  tds_init();
  ds12b20_init();
  ms5837_init();
  ph_init();
}

void envSensorsUpdate(envSensorData &data) {
  data.dsTemp = ds12b20_getTemp();

  ms5837_update();
  data.msPressure = get_pressure();
  data.msTemperature = get_temperature();
  data.msDepth = get_depth();
  data.msAltitude = get_altitude();
  int rawTDS = get_rawTDS();
  float Vm = get_conductivity(rawTDS);
  data.PH = get_ph();
  data.Turbidity = get_turbidity();
  data.tdsNormConductivity = get_normConductivity(Vm);
  data.tdsEC = get_EC(data.tdsNormConductivity);
  data.tdsTDS = get_TDS(data.tdsEC);
}

void envSensorData_print(envSensorData &data) {
  Serial.print((String)"Temp C: " + data.dsTemp + " deg C\n" +
               "Pressure: " + data.msPressure + " mbar\n" +
               "Temperature: " + data.msTemperature + " deg C\n" +
               "Depth: " + data.msDepth + " m\n" +
               "Altitude: " + data.msAltitude + " m above mean sea level\n" +
               "S = " + data.tdsNormConductivity + " мСм/см\n" +
               "EC = " + data.tdsEC + " мСм/см\n" +
               "TDS = " + data.tdsTDS + " мг/л\n" +
               "PH = " + data.PH + "\n" +
               "Turbidity = " + data.Turbidity + " мг/л\n");
}

String envSensorData_toString(envSensorData &data) {
  String msg = "";
  msg += (String)"Temp C: " + data.dsTemp + "deg C\n" +
         "Pressure: " + data.msPressure + " mbar\n" +
         "Temperature: " + data.msTemperature + " deg C\n" +
         "Depth: " + data.msDepth + " m\n" +
         "Altitude: " + data.msAltitude + " m above mean sea level\n" +
         "S = " + data.tdsNormConductivity + " мСм/см\n" +
         "EC = " + data.tdsEC + " мСм/см\n" +
         "TDS = " + data.tdsTDS + " мг/л\n" +
         "PH = " + data.PH + "\n" +
         "Turbidity = " + data.Turbidity + " мг/л\n";
  return msg;
}
