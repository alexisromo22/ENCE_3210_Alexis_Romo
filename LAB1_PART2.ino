//HERE WE ARE INCLUDING THE MATH LIBRARY TO USE IN OUR CODE
#include <math.h>

//HERE WE ARE CREATING THE ARRAY IN WHICH ALL VALUES WILL BE STORED INTO
int val[360];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  //HERE WE ARE CREATING A FOR LOOP THAT WILL GO THRU EACH VALUE 0 TO 360
  for (int degree = 0; degree <= 360; degree++) {
    //THE DEGREE WILL THEN BE CONVERTED TO RADIANS
    //THEN IT WILL BE CONVERTED TO THE COSINE OF THE ANGLE
    //THE 1000 IS USED TO SCALE THE NUMBER IN ORDER TO BETTER READ THE VALUES
    //THAT OUR BEING OUTPUTTED ONTO THE SERIAL MONITER
    val[degree] = cos(radians(degree)) * 1000; // Calculate and scale
  }


  //THE PURPOSE OF THIS FOR LOOP IS TO PRINT THE VALUES ONTO THE 
  //SERIAL MONITOR IN ORDER FOR US TO READ THE RESULTS
  for (int degree = 0; degree <= 360; degree++) {
    Serial.print("Degree: ");
    Serial.print(degree);
    Serial.print(", Cosine: ");
    Serial.println(val[degree]);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
}
