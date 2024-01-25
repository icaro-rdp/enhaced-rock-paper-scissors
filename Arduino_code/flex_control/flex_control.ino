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
uint16_t analog_input0_pin = 19;
uint16_t analog_input1_pin = 20;
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
int valid_round_played = 0;
int a_wins = 0;
int b_wins = 0;
const int button_pin = 7;
int button_state = 0;
int last_button_state = 1;
int round_total = 5;
const int limit_value = 300;
int hapticPin_a = 2;
int hapticPin_b = 3;
const int flexSensor_reading_time = 3000;
bool players_ready = false;
bool countdown_finished = false;
bool moves_received = false;
bool winner_announced = false;
bool valid_round = true;
bool invalid_announced = false;
int moves[2];

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
  char *all_tokens[2];
  const char delimiters[5] = {START_MARKER, ',', ' ', END_MARKER,'\0'};
  int i = 0;
  all_tokens[i] = strtok(received_message, delimiters);
  
  while (i < 2 && all_tokens[i] != NULL) {
    all_tokens[++i] = strtok(NULL, delimiters);
  }

  char *command = all_tokens[0]; 
  char *value = all_tokens[1];
  
  if (strcmp(command,"players_ready") == 0) {   
    players_ready = true;
  }  

  if (strcmp(command,"count_end") == 0) {
    countdown_finished = true;
  }  

  if (strcmp(command,"mov_rec") == 0) {   
    moves_received = true;
  }

  if (strcmp(command,"win_ann") == 0) {   
    winner_announced = true;
  } 

  if (strcmp(command,"inv_ann") == 0) {   
    invalid_announced = true;
  }
} 

/** Functions for mapping fingers with moves ***********************************************************************
    Data mapping: 0 = "rock"    1 = "paper"   2 = "scissors"    3 = "invalid"                                      */

int check_move(int thumb, int forefinger, int middlefinger, 
                  int ringfinger, int littlefinger){
  if(thumb<limit_value){
    if(forefinger < limit_value & middlefinger < limit_value & 
      ringfinger < limit_value & littlefinger < limit_value){
      return 0;                                                                     // rock
    } else if(forefinger > limit_value & middlefinger > limit_value & 
      ringfinger < limit_value & littlefinger < limit_value){
      return 2;                                                                     // scissors
    } else {
      return 3;                                                                     // invalid
    }
  } else{
    if(forefinger > limit_value & middlefinger > limit_value & 
      ringfinger < limit_value & littlefinger < limit_value){
      return 2;                                                                     // scissors
    } else if(forefinger > limit_value & middlefinger > limit_value & 
      ringfinger > limit_value & littlefinger > limit_value){
      return 1;                                                                     // paper
    } else {
      return 3;                                                                     // invalid
    }
  }
}

void checkFingers(int* moves){
  int thumb_a = analogRead(14);
  int forefinger_a = analogRead(15);
  int middlefinger_a = analogRead(16);
  int ringfinger_a = analogRead(17);
  int littlefinger_a = analogRead(18);
  int thumb_b = analogRead(A5);
  int forefinger_b = analogRead(A6);
  int middlefinger_b = analogRead(A7);
  int ringfinger_b = analogRead(A7);      //to change
  int littlefinger_b = analogRead(A7);    //to change
  
  Serial.print("fingers, ");
  Serial.println(thumb_a);
  //Serial.println("M_a: " + String(middlefinger_a) + ";");
  //Serial.println("L_a: " + String(littlefinger_a) + ";");
  //Serial.println("I_a: " + String(forefinger_a) + ";");
 

  int move_a = check_move(thumb_a, forefinger_a, middlefinger_a, ringfinger_a, littlefinger_a);
  int move_b = check_move(thumb_b, forefinger_b, middlefinger_b, ringfinger_b, littlefinger_b);
  moves[0] = move_a;
  // moves[1] = move_b;

  // moves[0] = 1;
  moves[1] = 1;
}

/** Functions for send IR values to PD ********************************************************************/

void sendIRPd(){
  if (micros() - analog_last_read >= ANALOG_PERIOD_MICROSECS) {
    analog_last_read += ANALOG_PERIOD_MICROSECS;
    
    // Loop for the IR_1 
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
    
    // Loop for the analog sensors
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

/** Function for send feedback about result to PD ****************************************************************/

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
  } else if (win == 1) {                      //playerA wins
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
  } else if (win == 2) {                        //playerB wins
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

/** Function for sending moves to PD ***********************************************************************
    Data mapping: 0 = "rock"    1 = "paper"   2 = "scissors"    3 = "invalid"                                      */

void sendMovesPd(int move_a, int move_b){
  Serial.print("d0, ");
  Serial.println(move_a);

  Serial.print("d1, ");
  Serial.println(move_b);
}

/** Functions to determine the winner of a round and of a match ***************************************************
    Data mapping: 0 = "rock"    1 = "paper"   2 = "scissors"    3 = "invalid"                                     */

int checkWinner(int move_a, int move_b){
  //Serial.println("PlayerA: " + move_a + "; PlayerB: " + move_b);
  if(move_a == 3 || move_b == 3){   //at least one invalid move
    giveFeedback(3);
    return 0;
  } else if(move_a == 1){           
    if(move_b == 2){                // paper - scissors
      b_wins++;
      giveFeedback(2);
      return 2;
    } else if(move_b == 0){        // paper - rock
      a_wins++;
      giveFeedback(1);
      return 1;
    } else {
      giveFeedback(0);              // paper - paper
      return 3;
    }
  } else if (move_a == 2){
    if(move_b == 1){                // scissors - paper
      a_wins++;
      giveFeedback(1);
      return 1;
    } else if(move_b == 0){         // scissors - rock
      b_wins++;
      giveFeedback(2);
      return 2;
    } else {                        // scissors - scissors
      giveFeedback(0);
      return 3;
    }
  } else {
    if(move_b == 1){                // rock - paper
      b_wins++;
      giveFeedback(2);
      return 2;
    } else if(move_b == 2){         // rock - scissors
      a_wins++;
      giveFeedback(1);
      return 1;
    } else {                        // rock - rock
      giveFeedback(0);
      return 3;
    }
  }
}

String checkFinalWinner(){
  if(a_wins > b_wins){
    return "1";
  } else if (a_wins < b_wins){
    return "2";
  } else {
    return "X";
  }
}

/** Loop function ****************************************************************************************/

void loop() {  

  /** Waiting for players ready ****************************************************************************/
   
  while(players_ready == false){
    sendIRPd();
    receive_message(); 
  }
  Serial.print("console, ");
  Serial.println("players_ready");

  
  /** Waiting for countdown finished ****************************************************************************/
  while(countdown_finished == false){
    receive_message(); 
  }
  Serial.print("console, ");
  Serial.println("count_finish");
  

  /** Flex sensors reading with move detection ********************************************************************/
  unsigned long startMillis = millis();
  while (millis() - startMillis < flexSensor_reading_time) { 
    Serial.print("console, ");
    Serial.println("read_flex");
    checkFingers(moves);    
    delay(500);
  }
  Serial.print("console, ");
  Serial.println("read_end");

  

  /** Waiting for moves received by PD ****************************************************************************/
  sendMovesPd(moves[0], moves[1]);
  
  while(moves_received == false){
    receive_message(); 
  }
  Serial.print("console, ");
  Serial.println("moves_rec");

  /** Determination of the winner for each round with partial results **********************************************/
  int winner = checkWinner(moves[0], moves[1]); // if 0, invalid round, if 1, player1 win, if 2, player2 win, if 3, even
  if (winner != 0) {
    round_played ++;
    valid_round_played ++;
    //Serial.println("Round " + String(round_played) + " result: " + winner);
    //Serial.println("Partial result: PlayerA " + String(a_wins) + " - PlayerB " + String(b_wins));
  } else {
    round_played ++;
  }
  Serial.print("w0, ");
  Serial.println(winner);  
  /** Waiting for announciation of winner in PD *********************************************************************/
  while(winner_announced == false){
    receive_message();
  }
  

  /** Round advancement or final winner determination ****************************************************/
  while(true){
    players_ready = false;
    countdown_finished = false;
    moves_received = false;
    winner_announced = false;
    valid_round = true;
    invalid_announced = false;
    if(valid_round_played < round_total){
      String next_round = String(valid_round_played + 1);
      Serial.print("console, ");
      Serial.println("nxt_round");
      break;  
    } else {
      String final_winner = checkFinalWinner();
      Serial.print("console, ");
      Serial.println(final_winner);
      round_played = 0;
      a_wins = 0;
      b_wins = 0;
      break;
    }
  }
}