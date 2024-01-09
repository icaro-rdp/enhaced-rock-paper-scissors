int round_played = 0;
int a_wins = 0;
int b_wins = 0;
const int button_pin = 2;
int button_state = 0;
int last_button_state = 1;
int round_total = 5;
const int limit_value = 180;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(button_pin, INPUT);
}

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
  
  Serial.println("T_a: " + String(thumb_a));
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

  //Serial.println("Move_a: " + move_a);
  //Serial.println("Move_b: " + move_b);
  static String moves[2];
  moves[0] = move_a;
  moves[1] = move_b;

  return moves;
  
}

String checkWinner(String* moves){
  Serial.println("PlayerA: " + moves[0] + "; PlayerB: " + moves[1]);
  if(moves[0] == "invalid" || moves[1] == "invalid"){
    return "N";
  } else if(moves[0] == "paper"){
    if(moves[1] == "scissors"){
      b_wins++;
      return "2";
    } else if(moves[1] == "rock"){
      a_wins++;
      return "1";
    } else {
      return "X";
    }
  } else if (moves[0] == "scissors"){
    if(moves[1] == "paper"){
      a_wins++;
      return "1";
    } else if(moves[1]=="rock"){
      b_wins++;
      return "2";
    } else {
      return "X";
    }
  } else {
    if(moves[1]=="paper"){
      b_wins++;
      return "2";
    } else if(moves[1]=="scissors"){
      a_wins++;
      return "1";
    } else {
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


void loop() {  
  String* moves;

  unsigned long startMillis = millis();
  while (millis() - startMillis < 3000) {
    moves = checkFingers();
    delay(500);
  }

  String winner = checkWinner(moves);
  round_played ++;
  Serial.println("Round " + String(round_played) + " result: " + winner);
  Serial.println("Partial result: PlayerA " + String(a_wins) + " - PlayerB " + String(b_wins));
  
  while(true){
    if(round_played < round_total){
      Serial.println("Ready for round " + String(round_played + 1));
      delay(3000);
      break;  
    } else {
      Serial.println(checkFinalWinner());
      while(true){
        button_state = digitalRead(button_pin);
        if (button_state != last_button_state) {
          round_played = 0;
          a_wins = 0;
          b_wins = 0;
          last_button_state = button_state;
          break;
        }
      }
    }
    
  }
}
