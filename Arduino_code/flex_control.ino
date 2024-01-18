#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <EEPROM.h>
#include <IIRFilter.h>
#include <string.h>

#define BAUD_RATE 115200

/* Variables for incoming messages *************************************************************/

const byte MAX_LENGTH_MESSAGE = 64;
char received_message[MAX_LENGTH_MESSAGE];
char START_MARKER = '[';
char END_MARKER = ']';
boolean new_message_received = false;

/* Analog inputs ******************************************************************************************/

#define ANALOG_PERIOD_MICROSECS 1000
static uint32_t analog_last_read = 0;
uint16_t analog_input0_pin = 0;
uint16_t analog_input1_pin = 1;
uint16_t analog_input0 = 0;
uint16_t analog_input1 = 0;
uint16_t analog_input0_lp_filtered = 0;
uint16_t analog_input1_lp_filtered = 0;
uint16_t previous_analog_input0_lp_filtered = 0;
uint16_t previous_analog_input1_lp_filtered = 0;

/* 50 Hz Butterworth low-pass ********************************************************************************/

double a_lp_50Hz[] = {1.000000000000, -3.180638548875, 3.861194348994, -2.112155355111, 0.438265142262};
double b_lp_50Hz[] = {0.000416599204407, 0.001666396817626, 0.002499595226440, 0.001666396817626, 0.000416599204407};
IIRFilter lp_analog_input0(b_lp_50Hz, a_lp_50Hz);
IIRFilter lp_analog_input1(b_lp_50Hz, a_lp_50Hz);

/* Thresholds for IR sensors ***********************************************************************************/

uint16_t analog_input0_threshold = 75;
uint16_t analog_input1_threshold = 75;

/* Const and variables for the script ******************************************************************************/

int round_played = 0;
int a_wins = 0;
int b_wins = 0;
const int button_pin = 7;
int button_state = 0;
int last_button_state = 1;
int round_total = 5;
const int limit_value = 180;
int hapticPin_a = 9;
int hapticPin_b = 10;
const int flexSensor_reading_time = 3000;
bool players_ready = false;

/** Setup function *************************************************************************************/

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(button_pin, INPUT);
  pinMode(hapticPin_a, OUTPUT);
  pinMode(hapticPin_b, OUTPUT);
}

/** Functions for handling received messages ***********************************************************************/

void receive_message() {
    static boolean reception_in_progress = false;
    static byte ndx = 0;
    char rcv_char;

    while (Serial.available() > 0 && new_message_received == false) {
        rcv_char = Serial.read();
        Serial.println(rcv_char);

        if (reception_in_progress == true) {
            if (rcv_char!= END_MARKER) {
                received_message[ndx] = rcv_char;
                ndx++;
                if (ndx >= MAX_LENGTH_MESSAGE) {
                    ndx = MAX_LENGTH_MESSAGE - 1;
                }
            }
            else {
                received_message[ndx] = '\0'; // terminate the string
                reception_in_progress = false;
                ndx = 0;
                new_message_received = true;
            }
        }
        else if (rcv_char == START_MARKER) {
            reception_in_progress = true;
        }
    }

    if (new_message_received) {
      handle_received_message(received_message);
      new_message_received = false;
    }
}


void handle_received_message(char *received_message) {
  //Serial.print("received_message: ");
  //Serial.println(received_message);
  char *all_tokens[2]; //NOTE: the message is composed by 2 tokens: command and value
  const char delimiters[5] = {START_MARKER, ',', ' ', END_MARKER,'\0'};
  int i = 0;
  all_tokens[i] = strtok(received_message, delimiters);
  
  while (i < 2 && all_tokens[i] != NULL) {
    all_tokens[++i] = strtok(NULL, delimiters);
  }

  char *command = all_tokens[0]; 
  char *value = all_tokens[1];
  if (strcmp(command,"players_ready") == 2) {
    /*
    Serial.print("activating message 1: ");
    Serial.print(command);
    Serial.print(" ");
    Serial.print(value);
    Serial.println(" ");
    */
    //analogWrite(digital_output4_pin, atoi(value));    
    players_ready = true;
  }  
} 

/** Functions for check fingers and moves ***********************************************************************/

String check_move(int thumb, int forefinger, int middlefinger, 
                  int ringfinger, int littlefinger){
  if(thumb<limit_value){
    if(forefinger < limit_value & middlefinger < limit_value & 
      ringfinger < limit_value & littlefinger < limit_value){
      return "rock";
    } else if(forefinger > limit_value & middlefinger > limit_value & 
      ringfinger < limit_value & littlefinger < limit_value){
      return "scissors";  
    } else {
      return "invalid";
    }
  } else{
    if(forefinger > limit_value & middlefinger > limit_value & 
      ringfinger < limit_value & littlefinger < limit_value){
      return "scissors";  
    } else if(forefinger > limit_value & middlefinger > limit_value & 
      ringfinger > limit_value & littlefinger > limit_value){
      return "paper";
    } else {
      return "invalid";
    }
  }
}

String* checkFingers(){
  int thumb_a = analogRead(A0);
  int forefinger_a = analogRead(A1);
  int middlefinger_a = analogRead(A2);
  int ringfinger_a = analogRead(A3);
  int littlefinger_a = analogRead(A4);
  int thumb_b = analogRead(A5);
  int forefinger_b = analogRead(A6);
  int middlefinger_b = analogRead(A7);
  int ringfinger_b = analogRead(A7);      //to change
  int littlefinger_b = analogRead(A7);    //to change
  
  //Serial.print("T_a: " + String(thumb_a) + ";");
  //Serial.println("F_a: " + String(forefinger_a));
  //Serial.println("M_a: " + middlefinger_a);
  //Serial.println("R_a: " + ringfinger_a);
  //Serial.println("L_a: " + littlefinger_a);
  //Serial.println("T_a: " + String(thumb_b));
  //Serial.println("F_b: " + forefinger_b);
  //Serial.println("M_b: " + middlefinger_b);
  //Serial.println("R_b: " + ringfinger_b);
  //Serial.println("L_b: " + littlefinger_b);

  String move_a = check_move(thumb_a, forefinger_a, middlefinger_a, ringfinger_a, littlefinger_a);
  String move_b = check_move(thumb_b, forefinger_b, middlefinger_b, ringfinger_b, littlefinger_b);
  static String moves[2];
  moves[0] = move_a;
  moves[1] = move_b;

  return moves; 
}

/** Functions for send IR values to PD ********************************************************************/

void sendIRPd(){
  if (micros() - analog_last_read >= ANALOG_PERIOD_MICROSECS) {
    analog_last_read += ANALOG_PERIOD_MICROSECS;
    
    /* Loop for the IR_1 ******************************************************************************/
    analog_input0 = analogRead(analog_input0_pin);
    analog_input0_lp_filtered =  (uint16_t)lp_analog_input0.filter((double)analog_input0);
    
    // Apply thresholds to the filtered signal
    analog_input0_lp_filtered = (analog_input0_lp_filtered < analog_input0_threshold) ? 0 : analog_input0_lp_filtered;

    // Send the sensor value to the serial port only if it has changed
    if(analog_input0_lp_filtered != previous_analog_input0_lp_filtered){
      Serial.print("a0, ");
      Serial.println(analog_input0_lp_filtered);
      previous_analog_input0_lp_filtered = analog_input0_lp_filtered;
    }
  } // End of the section processing IR_1

  if (micros() - analog_last_read >= ANALOG_PERIOD_MICROSECS) {
    analog_last_read += ANALOG_PERIOD_MICROSECS;
    
    /* Loop for the analog sensors ******************************************************************************/    
    analog_input1 = analogRead(analog_input1_pin);
    analog_input1_lp_filtered =  (uint16_t)lp_analog_input1.filter((double)analog_input1);
    
    // Apply thresholds to the filtered signal
    analog_input1_lp_filtered = (analog_input1_lp_filtered < analog_input1_threshold) ? 0 : analog_input1_lp_filtered;
  
    // Send the sensor value to the serial port only if it has changed
    if(analog_input1_lp_filtered != previous_analog_input1_lp_filtered){
      Serial.print("a1, ");
      Serial.println(analog_input1_lp_filtered);
      previous_analog_input1_lp_filtered = analog_input1_lp_filtered;
    }
  } // End of the section processing IR_2
}

/** Functions for send feedback about result to PD ****************************************************************/

void giveFeedback(int win){
  if(win == 0 or win == 3){                 //draw or null
    digitalWrite(hapticPin_a, HIGH);
    digitalWrite(hapticPin_b, HIGH);
    delay(500);
    digitalWrite(hapticPin_a, LOW);
    digitalWrite(hapticPin_b, LOW);
    delay(200);
    digitalWrite(hapticPin_a, HIGH);
    digitalWrite(hapticPin_b, HIGH);
    delay(500);
    digitalWrite(hapticPin_a, LOW);
    digitalWrite(hapticPin_b, LOW);
  } else if (win == 1) {        //playerA wins
    digitalWrite(hapticPin_a, HIGH);
    digitalWrite(hapticPin_b, HIGH);
    delay(100);
    digitalWrite(hapticPin_a, LOW);
    delay(100);
    digitalWrite(hapticPin_a, HIGH);
    delay(100);
    digitalWrite(hapticPin_a, LOW);
    delay(100);
    digitalWrite(hapticPin_a, HIGH);
    delay(100);
    digitalWrite(hapticPin_a, LOW);
    delay(100);
    digitalWrite(hapticPin_a, HIGH);
    delay(100);
    digitalWrite(hapticPin_a, LOW);
    delay(300);
    digitalWrite(hapticPin_b, LOW);
  } else if (win == 2) {        //playerB wins
    digitalWrite(hapticPin_a, HIGH);
    digitalWrite(hapticPin_b, HIGH);
    delay(100);
    digitalWrite(hapticPin_b, LOW);
    delay(100);
    digitalWrite(hapticPin_b, HIGH);
    delay(100);
    digitalWrite(hapticPin_b, LOW);
    delay(100);
    digitalWrite(hapticPin_b, HIGH);
    delay(100);
    digitalWrite(hapticPin_b, LOW);
    delay(100);
    digitalWrite(hapticPin_b, HIGH);
    delay(100);
    digitalWrite(hapticPin_b, LOW);
    delay(300);
    digitalWrite(hapticPin_a, LOW);
  }
}


//Data mapping: 0 = "rock"    1 = "paper"   2 = "scissors"    3 = "invalid"   
void sendMovesPd(String* moves){
  if(moves[0] == "rock"){
    Serial.print("d0, ");
    Serial.println(0);  
  } else if(moves[0] == "paper"){
    Serial.print("d0, ");
    Serial.println(1);
  } else if(moves[0] == "scissors"){
    Serial.print("d0, ");
    Serial.println(2);
  } else {
    Serial.print("d0, ");
    Serial.println(3);
  }

  if(moves[1] == "rock"){
    Serial.print("d1, ");
    Serial.println(0);  
  } else if(moves[1] == "paper"){
    Serial.print("d1, ");
    Serial.println(1);
  } else if(moves[1] == "scissors"){
    Serial.print("d1, ");
    Serial.println(2);
  } else {
    Serial.print("d1, ");
    Serial.println(3);
  }
}

/** Function to determine the winner of a round and of a match ***************************************************/

String checkWinner(String* moves){
  Serial.println("PlayerA: " + moves[0] + "; PlayerB: " + moves[1]);
  if(moves[0] == "invalid" || moves[1] == "invalid"){
    giveFeedback(3);
    return "N";
  } else if(moves[0] == "paper"){
    if(moves[1] == "scissors"){
      b_wins++;
      giveFeedback(2);
      return "2";
    } else if(moves[1] == "rock"){
      a_wins++;
      giveFeedback(1);
      return "1";
    } else {
      giveFeedback(0);                //HERE TRY WITH 1 FLEX!!
      return "X";
    }
  } else if (moves[0] == "scissors"){
    if(moves[1] == "paper"){
      a_wins++;
      giveFeedback(1);
      return "1";
    } else if(moves[1]=="rock"){
      b_wins++;
      giveFeedback(2);
      return "2";
    } else {
      giveFeedback(0);
      return "X";
    }
  } else {
    if(moves[1]=="paper"){
      b_wins++;
      giveFeedback(2);
      return "2";
    } else if(moves[1]=="scissors"){
      a_wins++;
      giveFeedback(1);
      return "1";
    } else {
      giveFeedback(0);
      return "X";
    }
  }
}

String checkFinalWinner(){
  if(a_wins > b_wins){
    return "PlayerA wins!";
  } else if (a_wins < b_wins){
    return "PlayerB wins!";
  } else {
    return "It's a draw!";
  }
}

/** Loop function ****************************************************************************************/

void loop() {  
  String* moves;
  
  /** Waiting for players ready ****************************************************************************/
  while(players_ready == false){
    sendIRPd();
    receive_message();
    Serial.println("All players ready, exit from IR reading loop");
  }
  
  /** Flex sensors reading with move detection ********************************************************************/
  unsigned long startMillis = millis();
  while (millis() - startMillis < flexSensor_reading_time) {
    moves = checkFingers();
    delay(500);
  }

  /** Determination of the winner for each round with partial results **********************************************/
  String winner = checkWinner(moves);
  sendMovesPd(moves);
  round_played ++;
  Serial.println("Round " + String(round_played) + " result: " + winner);
  Serial.println("Partial result: PlayerA " + String(a_wins) + " - PlayerB " + String(b_wins));

  /** Round advancement or final winner determination ****************************************************/
  while(true){
    if(round_played < round_total){
      Serial.println("Ready for round " + String(round_played + 1));
      //delay(3000);
      players_ready = false;
      break;  
    } else {
      Serial.println(checkFinalWinner());
      round_played = 0;
      a_wins = 0;
      b_wins = 0;
      players_ready = false;
      break;
      /*while(true){
        button_state = digitalRead(button_pin);
        if (button_state != last_button_state) {
          round_played = 0;
          a_wins = 0;
          b_wins = 0;
          players_ready = false;
          last_button_state = button_state;
          break;
        }  
      }*/
    }
  }
}
