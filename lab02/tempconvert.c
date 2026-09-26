# include <stdio.h>

// Converts Fahrenheit to Celsius
float fahrenheit2celsius(float fTempFahrenheit) {

  float fTempCelsius = (fTempFahrenheit - 32) * 5 / 9;

  return fTempCelsius;
}

float celsius2fahrenheit(float fTempCelsius) {
  
  float fTempFahrenheit = (fTempCelsius * 9 / 5) + 32;

  return fTempFahrenheit;
}

int main(void) {

  float fCelsius = 0;
  float fFahrenheit = 0;
  
  printf("Welcome to the Temperature Converter!\n");
  printf("Please select one of the following options:\n");
  printf("1. Convert from Fahrenheit to Celsius.\n");
  printf("2. Convert from Celsius to Fahrenheit.\n");
  printf("3. Exit the Converter.\n");

  int selection = 0;
  scanf("%d", &selection);

  if (selection == 1) {
	float temperature = 0;
	printf("Please provide your temperature in Fahrenheit:");
	scanf("%f", &temperature);
  	fCelsius = fahrenheit2celsius(temperature);

  	printf("%.2f degrees Celsius\n", fCelsius);
  }
  else if (selection == 2) {
	float temperature = 0;
	printf("Please provide your temperature in Celsius:");
	scanf("%f", &temperature);
	fFahrenheit = celsius2fahrenheit(temperature);

	printf("%.2f degrees Fahrenheit\n", fFahrenheit);
  }
  else {
	printf("Goodbye!\n");
	return 0;
}
  return 0;
}
