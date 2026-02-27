#include "lightArray.h"

struct sensorMap;

void lightArrayInit(uint32_t &binaryControlLineValue)
{
	g_muxControlPinMap = muxControlInit(&g_binaryMappingToControlLinePins, binaryControlLineValue, g_muxPinList, g_controlPinCount, g_sensorCount);
	if (!MatrixIsNull(g_sensorPositionMatrix)) //if matrix is filled already, then we clear first
		MatrixFree(&g_sensorPositionMatrix);
	if (!MatrixIsNull(g_sensorPositionPseudoInverseMatrix)) //if matrix is filled already, then we clear first
		MatrixFree(&g_sensorPositionPseudoInverseMatrix);
	g_sensorPositionMatrix = lightArrayCreateSensorPositionSPHERE(1.0f, g_sensorCount);
	g_sensorPositionPseudoInverseMatrix = lightArraySensorPositionPseudoInverse(g_sensorPositionMatrix);
	return;
}

std::pair<float, float> lightArrayCalculateAoA3D()
{
    return lightArrayAoA3D(g_sensorPositionPseudoInverseMatrix, g_sensorIntensityVector);
}

void lightArraySampleSensors()
{
	uint32_t dummy = 0;
	muxControlReadPins(g_muxControlPinMap, g_binaryMappingToControlLinePins, g_muxADCReadings, dummy, g_sensorCount); //samples all sensors, for 16 bit resolution ADC values range from of 0 to 65535
	uint16_t minValue = 0; //min value can receive from ADC 
	uint16_t maxValue = 4096;//~((uint16_t)0);//since 16 bit resolution, this will hold max value can receive from ADC
	//float temp[] = { 0.866025403784439, 0.991444861373811, 0.965925826289068 ,0.793353340291235, 0.500000000000000,0.130526192220052,0,0,0,0,0,0,0,0,0.258819045102521,0.608761429008721 };

	for (uint32_t i = 0; i < g_sensorCount; i++)
  {
    if (g_muxADCReadings[i] > g_sensorBaseline[i])
		  g_sensorIntensityVector.element[i][0] = lightArrayReMapADC<uint16_t>(minValue, maxValue, g_muxADCReadings[i]-g_sensorBaseline[i]);//temp[i];//  //converts range uint16_t [0, 65535] to float [0, 1]
    else
      g_sensorIntensityVector.element[i][0] = lightArrayReMapADC<uint16_t>(minValue, maxValue, 0);
      //g_sensorIntensityVector.element[i][0] = lightArrayReMapADC<uint16_t>(minValue, maxValue, g_muxADCReadings[i]);
  }
  return;
}

void lightArrayBaseline()
{
	for (uint32_t i = 0; i < g_sensorCount; i++)
		g_sensorBaseline[i] = 0;
	uint32_t dummy = 0;
	for (uint32_t j = 0; j < 10; j++)
  {
    muxControlReadPins(g_muxControlPinMap, g_binaryMappingToControlLinePins, g_muxADCReadings, dummy, g_sensorCount); //take baseline
    for (uint32_t i = 0; i < g_sensorCount; i++)
      g_sensorBaseline[i] += g_muxADCReadings[i]/10;
  }
  return;
}

//testing
void lightArraySelectSensor(const muxControlPinMap &controlPinMap, uint32_t* const &decimalBinaryMapping, const uint32_t selectedSensor)
{
	muxControlWriteControlPins(controlPinMap, decimalBinaryMapping[selectedSensor]);
}

//testing
uint16_t inline lightArraySampleSensor(const muxControlPinMap &controlPinMap, uint32_t* const &decimalBinaryMapping, const uint32_t selectedSensor)
{
	uint16_t minValue = 0; //min value can receive from ADC 
	uint16_t maxValue = 4096;//~((uint16_t)0);//since 16 bit resolution, this will hold max value can receive from ADC
	lightArraySelectSensor(controlPinMap, decimalBinaryMapping, selectedSensor); //change select lines
	//delay for 25 ns since multiple of 2.083 ns
	//we require 24 ns to account for max propagation, there is about 6 cycles per loop = 12.5 ns, so require 2 loops for total of 25 ns
	lightArrayDelayTwentyFiveNano(); //delays for one clock cycle, aka 2.083ns if 480 MHz [asm volatile("nop")] <--In arduino sketches
	
	return lightArrayReMapADC<uint16_t>(minValue, maxValue, muxControlReadPin()); //returns value
}

//testing
uint32_t lightArraySampleSelectedSensors(const muxControlPinMap& controlPinMap, uint32_t* const& decimalBinaryMapping, uint32_t* const &selectedSensors, const uint32_t &sensorCount, float* const& intensityReadings)
{
	uint16_t minValue = 0; //min value can receive from ADC 
	uint16_t maxValue = 4096;//~((uint16_t)0);//since 16 bit resolution, this will hold max value can receive from ADC
	uint32_t i;
	uint16_t* ADCReading = new uint16_t[sensorCount];
	for (i = 0; i < sensorCount; i++)
	{
		lightArraySelectSensor(controlPinMap, decimalBinaryMapping, selectedSensors[i]); //change select lines
		//delay for 25 ns since multiple of 2.083 ns
		//we require 24 ns to account for max propagation, there is about 6 cycles per loop = 12.5 ns, so require 2 loops for total of 25 ns
		delayMicroseconds(100);//delay(200); //delays for one clock cycle, aka 2.083ns if 480 MHz [asm volatile("nop")] <--In arduino sketches
    ADCReading[i] = muxControlReadPin();
	}
	//all selected sensors have been read
	lightArrayReMapArrayValues(intensityReadings, ADCReading, sensorCount); //re map the range from uint16_t [0 65535] to float [0 1]
	delete[] ADCReading;
	return selectedSensors[i-1]; //returns currently selected sensor
}

//testing
void lightArrayReMapArrayValues(float* const & reMappedValues, uint16_t* const & originalValues, const uint32_t &numberToRemap)
{
	uint16_t minValue = 0; //min value can receive from ADC 
	uint16_t maxValue = 4096;//~((uint16_t)0);//since 16 bit resolution, this will hold max value can receive from ADC
	uint32_t i;
	for (i = 0; i < numberToRemap; i++)
		reMappedValues[i] = lightArrayReMapADC<uint16_t>(minValue, maxValue, originalValues[i]);
	return;
}

//SAMPLING TEST
// This test will be used to find out whether the code works to sample a value from the sensor and that we can assert on the control lines.
// This test will initialize the mux and then wait about 1 s before sampling then switch to another sensor before sampling again.
// We will be sampling 25e9x slower than we calculated we required.
// The sensor 0 sub-ckt should be replaced with two 100k Ohm in series. With Vdd of 5V and tapped from between both resistors.
// The sensor 15 sub-ckt should be replaced with two 100k Ohm in series. With Vdd of 2V and tapped from between both resistors.
// We expect voltage at sensor 0 to be 2.5V due to voltage divider configuration.
// We expect voltage at sensor 15 to be 1.0V due to voltage divider configuration.
// We expect ADC reading of 65536*(2.5/5) - 1 = 32767 at sensor 0
// We expect ADC reading of 65536*(1/5) - 1 ~= 13107.2 at sensor 15
// We expect intensity reading of 1*(2.5/5) = 0.5 at sensor 0
// We expect intensity reading of 1*(1/5) = 0.2 at sensor 15

void lightArrayTestOne(float* &intensityReadings)
{
	uint32_t dummy = 0;

	// Dynamically create an array of all sensor indices (0 to g_sensorCount-1)
	uint32_t* selectedSensors = new uint32_t[g_sensorCount];
	for (uint32_t i = 0; i < g_sensorCount; i++)
		selectedSensors[i] = i;

	// Allocate intensity readings if needed
	if (intensityReadings == nullptr)
		intensityReadings = new float[g_sensorCount];

	// Sample all selected sensors
	lightArraySampleSelectedSensors(g_muxControlPinMap, g_binaryMappingToControlLinePins, selectedSensors, g_sensorCount, intensityReadings);

	delete[] selectedSensors; // Clean up memory
}



//MAX SWITCHING FREQUENCY TEST
// This test will be used to finetune the delay required before a value is read from the sensor after it has been switched.
// This test will alternate between sensor 0 and sensor 1, while providing a delay of 25 ns between switching and then sampling.
// We will adjust the delay until the expected values are read.
// The sensor 0 sub-ckt should be replaced with two 100k Ohm in series. With Vdd of 5V and tapped from between both resistors.
// The sensor 1 sub-ckt should be replaced with two 100k Ohm in series. With Vdd of 2V and tapped from between both resistors.
// We expect voltage at sensor 0 to be 2.5V due to voltage divider configuration.
// We expect voltage at sensor 1 to be 1.0V due to voltage divider configuration.
// We expect ADC reading of 65536*(2.5/5) - 1 = 32767 at sensor 0
// We expect ADC reading of 65536*(1/5) - 1 ~= 13107.2 at sensor 1
// We expect intensity reading of 1*(2.5/5) = 0.5 at sensor 0
// We expect intensity reading of 1*(1/5) = 0.2 at sensor 1

void lightArrayTestTwo(float* intensityReadings)
{
	uint32_t dummy = 0;
	uint32_t selectedSensors[] = { 0, 1 };
	int arraySize = sizeof(selectedSensors) / sizeof(uint32_t);
	if (intensityReadings == nullptr)
		intensityReadings = new float[arraySize];
	lightArraySampleSelectedSensors(g_muxControlPinMap, g_binaryMappingToControlLinePins, selectedSensors, arraySize, intensityReadings); //sample sensors 0 and 15, returns to intensityReadings
}

//MAX SWITCHING FREQUENCY TEST
// This test will be used to fine tune the delay required before a value is read from the sensor after it has been switched.
// This test will alternate between sensor 0 and sensor 1.
// The sensor 0 sub-ckt should be replaced with two 100k Ohm in series. With Vdd of 5V and tapped from between both resistors.
// The sensor 1 sub-ckt should be replaced with two 100k Ohm in series. With Vdd of 2V and tapped from between both resistors.
// We expect voltage at sensor 0 to be 2.5V due to voltage divider configuration.
// We expect voltage at sensor 1 to be 1.0V due to voltage divider configuration.
// We expect ADC reading of 65536*(2.5/5) - 1 = 32767 at sensor 0
// We expect ADC reading of 65536*(1/5) - 1 ~= 13107.2 at sensor 1
// We expect intensity reading of 1*(2.5/5) = 0.5 at sensor 0
// We expect intensity reading of 1*(1/5) = 0.2 at sensor 1

void lightArrayTestThree()
{
	uint32_t dummy;
	lightArrayInit(dummy); //init mux control
}


template <typename T> 
float lightArrayReMapADC(T min, T max, T value)
{
	T scale = max - min;
	T shift = value - min;
	float scaledValue = (float)shift / (float)scale;
	return scaledValue;
}

Matrix lightArrayCreateSensorPositionSPHERE(float radius, uint32_t sensorCount)
{
    Matrix sensorPositionMatrix = MatrixRect(sensorCount, 3); // x, y, z

    for (uint32_t i = 0; i < sensorCount; ++i) {
        // Distribute sensors over a sphere using Fibonacci lattice
        float phi = acos(1 - 2 * (i + 0.5f) / sensorCount);   // elevation (0 to π)
        float theta = M_PI * (1 + sqrt(5)) * i;               // golden angle for azimuth

        float x = radius * sin(phi) * cos(theta);
        float y = radius * sin(phi) * sin(theta);
        float z = radius * cos(phi);

        sensorPositionMatrix.element[i][0] = x;
        sensorPositionMatrix.element[i][1] = y;
        sensorPositionMatrix.element[i][2] = z;
    }

    return sensorPositionMatrix;
}


Matrix lightArraySensorPositionPseudoInverse(const Matrix & sensorPositionMatrix)
{
	return MatrixPseudoInverse(sensorPositionMatrix);
}

std::pair<float, float> lightArrayAoA3D(const Matrix & sensorPositionPseudoInverseMatrix, const Matrix & sensorIntensityVector)
{
    Matrix cartesianSolution = MatrixMultiply(sensorPositionPseudoInverseMatrix, sensorIntensityVector);
    Matrix sphericalSolution = cart2SphD(cartesianSolution); // Convert (x, y, z) to (θ, φ, r)

    float theta = sphericalSolution.element[0][0]; // Azimuth (θ)
    float phi = sphericalSolution.element[1][0];   // Elevation (φ)

    MatrixFree(&cartesianSolution);
    MatrixFree(&sphericalSolution);

    return {theta, phi}; // Return both angles
}

//12 cycles for 25 ns, experimentation found it might be 21 cycles?
void inline lightArrayDelayTwentyFiveNano()
{
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
}

//48 cycles for 0.1 us, experimentation found it might be 84 cycles???
void inline lightArrayDelayMicroTenth()
{
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");

	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");

	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
  
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");

  
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");

	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");

	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
	asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop"); asm volatile("nop");
  
}

//480 cycles for one 1 us
void inline lightArrayDelayMicro()
{
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();

	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
	lightArrayDelayMicroTenth();
}

// for 0.01 millisecond
void inline lightArrayDelayMilliHundreth()
{
	lightArrayDelayMicro();
	lightArrayDelayMicro();
	lightArrayDelayMicro();
	lightArrayDelayMicro();
	lightArrayDelayMicro();

	lightArrayDelayMicro();
	lightArrayDelayMicro();
	lightArrayDelayMicro();
	lightArrayDelayMicro();
	lightArrayDelayMicro();
}

void inline lightArrayDelayMilliTenth()
{
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();

	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
	lightArrayDelayMilliHundreth();
}

void inline lightArrayDelayMilli()
{
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();

	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
	lightArrayDelayMilliTenth();
}

void inline lightArrayDelaySecondHundreth()
{
	lightArrayDelayMilli();
	lightArrayDelayMilli();
	lightArrayDelayMilli();
	lightArrayDelayMilli();
	lightArrayDelayMilli();

	lightArrayDelayMilli();
	lightArrayDelayMilli();
	lightArrayDelayMilli();
	lightArrayDelayMilli();
	lightArrayDelayMilli();
}

void inline lightArrayDelaySecondTenth()
{
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();

	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
	lightArrayDelaySecondHundreth();
}

void inline lightArrayDelaySecond()
{
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();

	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
	lightArrayDelaySecondTenth();
}
