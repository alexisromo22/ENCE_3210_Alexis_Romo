//HERE I AM INCLUDING THE LIBRARY THAT WILL HELP ME STORE MY MAX VALUES
#include <limits.h> 

// HERE I AM CREATING MY ARRAY WITH 50 DIFFERENT NUMBERS, I CHANGED ALL NUMBERS
//TO BE IN ORDER TO FURTHER VERIFY THAT IT IS ABLE TO DETECT 50 AS THE MAX NUMBER
// AND 49 AS THE SECOND MAX
int array[50] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
                 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                 41, 42, 43, 44, 45, 46, 47, 48, 49, 50}; 

// HERE I AM CREATING A POINTER THAT WILL POINT TO THE MEMORY JUST AFTER THE ARRAY ENDS
int *secondMaxLocation = &array[50]; 

void setup() {
  Serial.begin(9600); 
  //HERE I AM ASSIGNING A VARIABLE FOR THE LARGEST NUMBER IN MY ARRAY
  int max1 = INT_MIN; 
  //HERE I AM ASSIGNING A VARIABLE FOR THE SECOND LARGEST NUMBER IN MY ARRAY
  int max2 = INT_MIN;
  
  //INT_MIN IS A VARIABLE THAT IS ASSIGNED TO THE SMALLEST POSSIBLE INT IN C

  //HERE I HAVE A FOR LOOP THAT WILL GO THROUGH 0-49 IN ORDER TO ACCESS
  //EVERY NUMBER IN THE ARRAY
  for (int i = 0; i < 50; i++) {

    //HERE IF THE INT BEING INPUTTED IS GREATER THAN MAX1
    //IT WILL UPDATE MAX2 TO THE CURRENT MAX INT,
    //THEN UPDATE MAX1 WITH THE NEW LARGER INT
    if (array[i] > max1) {
      max2 = max1;   
      max1 = array[i]; 

    //HERE IF THE VALUE INPUTTED IS GREATER THAN MAX2 BUT LESS THAN MAX1
    //THEN THE MAX2 VALUE WILL BE UPDATED WITH THE NEW INT   
    } else if (array[i] > max2 && array[i] < max1) {
      max2 = array[i]; // Update second maximum
    }
  }
  //IF NONE OF THESE CONDITIONS ARE MET, THEN THE FOR LOOP WILL JUST
  //CONTINUE TO THE NEXT INT IN THE ARRAY

  //HERE WE ARE STORING THE LOCATION OF MAX2 WITH A POINTER
  *secondMaxLocation = max2;

  //HERE THE RESULTS ARE BEING PRINTED TO THE SERIAL MONITOR
  Serial.print("Maximum Value: ");
  Serial.println(max1);
  Serial.print("Second Maximum Value: ");
  Serial.println(max2);
  Serial.print("Second Maximum Stored at Memory Location: ");
  //HERE THE MEMORY LOCATION OF THE SECOND MAX NUMBER IS BEING PRINTED
  Serial.println((unsigned int)secondMaxLocation);
}

void loop() {
  // Nothing to do in the loop
}
