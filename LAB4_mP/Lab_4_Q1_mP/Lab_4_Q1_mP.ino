//here we are defining the bit size
#define bitsize 14

//here we are setting the reference voltage
double Vref = 3.0;

//here we are creating the array on inputs voltages that will be tested
double test_Vin[] = {0.42, 0.83, 1.65, 2.752};

//here we are creating the array that will store the binary representation
//of the quantized voltages
int bits[bitsize];

//here we are creating a variable
//named count to count the iterations in the loop
int count;



void SAR_ADC(float Vin) {
  //here we are starting with half of the reference voltage
    double thresh = Vref / 2.0; 

    //here we are defining the step size
    double step = Vref / 2.0;

    //here we are initializing this variable to then
    //store the quantized value
    double quantized = 0;


  //here we are looping through all 14 bits
  for(count=0; count<bitsize; count++){
    //here we are halving the step size for each bit
    step /= 2;

    //here if the Vin is greater than or equal to thresh 
    //then the current bit will be sit to 1
    if (Vin >= thresh){
      bits[count] = 1;
      //then the thresh will move up by the current step size
      thresh += step;}

    else {
      //otherwise the current bit will be set to 0
      //and move the thresh down by the current size
        bits[count] = 0;
        thresh -= step;}


    //here the the quantized value of the current bit will be added to the total
    quantized += bits[count]*step*2.0;}

    //here we are printing the results to the serial monitor
    Serial.print("Vin: ");
    Serial.print(Vin, 5);
    Serial.print("V -> Quantized: ");
    Serial.print(quantized, 5);
    Serial.print("V -> Binary: ");
    
    //here we are creating a loop to print each bit of the binary representation
    for (int i = 0; i < bitsize; i++) {
        Serial.print(bits[i]);
    }
    Serial.println();

}


void setup() {
  // put your setup code here, to run once:
  //here we are starting the serial communication
  Serial.begin(9600);  
    delay(1000);  

    //here we have another loop to test the input voltages values
    //and perform SAR ACD to each input
    for (int i = 0; i < 4; i++) {
        SAR_ADC(test_Vin[i]);
    }

}

void loop() {
  // put your main code here, to run repeatedly:

}
