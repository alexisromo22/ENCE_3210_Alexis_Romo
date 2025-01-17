//HERE WE ARE ASSIGNING VARIABLE NAMES TO THE PINS THAT WILL LIGHT UP EACH COLORED LED
#define RED_LED    9
#define GREEN_LED  10
#define BLUE_LED   11

//HERE WE HAVE AN ARRAY OF RANDOM LETTERS INCLUDING THE LETTERS R G B
char letters[] = {'A', 'D', 'G', 'B', 'R', 'G', 'R', 'X', 'G', 'Z'};

//HERE WE ARE ASSIGING 0 TO EACH CORRESPONDING LETTER IN ORDER
//TO KEEP COUNT OF HOW MANY TIMES EACH LETTER IS FOUND IN THE ARRAY
int countR = 0;
int countG = 0;
int countB = 0;

void setup() {
  //HERE WE ARE SETTING THE LEDS TO BE OUTPUTS
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  
  // HERE WE HAVE A FOR LOOP THAT WILL GO THRU EVERY LETTER IN THE ARRAY
  //IN THE LOOP IT WILL CHECK WHETHER THE LETTER THE LOOP IS CHECKING IS A 'R' 'G' 'B'
  //IF THIS CASE IS TRUE, THEN 1 will be added to the count of the respective letter.
  for (int i = 0; i < sizeof(letters); i++) {
    if (letters[i] == 'R') {
      countR++;
    }
    if (letters[i] == 'G') {
      countG++;
    }
    if (letters[i] == 'B') {
      countB++;
    }
  }
  
  //HERE THE RESULTS WILL BE PRINTED TO THE SERIAL MONITER 
  Serial.begin(9600);
  Serial.print("Count of R: ");
  Serial.println(countR);
  Serial.print("Count of G: ");
  Serial.println(countG);
  Serial.print("Count of B: ");
  Serial.println(countB);
  
  //HERE EACH CORRESPONDING LED WILL TURN ON BASED ON IF THE THE RESPECTIVE LETTER
  //WAS FOUND ONCE IN THE ARRAY
  if (countR > 0) {
    digitalWrite(RED_LED, HIGH); 
  } else {
    digitalWrite(RED_LED, LOW);   
  }
  
  if (countG > 0) {
    digitalWrite(GREEN_LED, HIGH); 
  } else {
    digitalWrite(GREEN_LED, LOW);   
  }
  
  if (countB > 0) {
    digitalWrite(BLUE_PIN, HIGH);  
  } else {
    digitalWrite(BLUE_PIN, LOW);   
  }
}

void loop() {

}


