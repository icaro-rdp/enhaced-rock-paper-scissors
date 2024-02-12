# enhaced-rock-paper-scissors

# Flex Control

This is a project that uses a flex sensor to control a game. The game is played in rounds, and the number of rounds won by each player is tracked.

## Variables

- `round_played`: The number of rounds that have been played.
- `a_wins`: The number of rounds won by player A.
- `b_wins`: The number of rounds won by player B.
- `button_pin`: The pin number where the button is connected.
- `button_state`: The current state of the button.
- `last_button_state`: The last state of the button.
- `round_total`: The total number of rounds to be played.
- `limit_value`: The limit value for the flex sensor. We created different threshold values given the different default values returned by the sensors.
- `hapticPin_a`: The pin number where first haptic motors are connected.
- `hapticPin_b`: The pin number where second haptic motors are connected.
- `flexSensor_reading_time`: Milliseconds of reading each user's move.
- `players_ready`: Boolean variable indicating both players ready for the round.
- `countdown_finished`: Boolean variable indicating acoustic countdown performed.
- `moves_received`: Boolean variable indicating receipt of moves by Pure Data.
- `winner_announced`: Boolean variable indicating audio announcement of the winner made.
- `valid_round`: Boolean variable indicating round validity or not. 
- `moves[2]`: Vector containing two integers representing the moves made by users.

## Setup

The setup routine runs once when you press reset. It initializes serial communication at 115200 bits per second and sets the various haptic pins as output.

## Functions

### receive_message

Standard function that identifies receipt of new messages from Pure Data. This calls the handle_received_message function to handle them.

### handle_receive_message

Control function for various messages from Pure Data. Checks that the command and value arriving from Pure Data match the expected values before continuing to the next step.

### check_move

This function checks what move a user is currently performing and classifies it as "0" (rock), "1" (paper), "2" (scissors) or "3" (invalid). The classification is done based on the flex sensor value obtained from each individual finger.
We created twin functions for the two gloves because of the extremely different default values that the various flex sensors returned.
The return value is of type int.

### checkFingers

This function reads the sensor value of each finger continuously and then calls the check_move functions to classify the users' moves.
Assigns the values corresponding to the moves made to the moves array.

### sendIRPd

Function that reads values from IR sensors and communicates those values to Pure Data via the serial port.

### giveFeedback

This function returns haptic feedback through small haptic motors attached to the user's wrist.
The feedbacks, calssified by "win," "lose," "draw," or "null," are all distinct from each other, except for "draw" and "null" which are classified the same way.

### sendMovesPd

Function that communicates the moves made by users to Pure Data via the serial port.

### checkWinner

This function, once it obtains the moves made at the end of the round by the two users, compares them and determines the winner of that round, increasing the variable related to the number of rounds won by the user.
It calls, for each result obtained, the giveFeedback function, passing the value of the result.
The return value is of type String and corresponds to "1" in case the first player won, "2" in case the second player won, "X" in case of a tie, and "N" in case of a null round.

### checkFinalWinner

This function is called at the end of the last round. It compares the numbers of rounds won by the two users and returns the final result, ruling a match winner or a tie.
The return value is of type String and corresponds to "1" in case the first player won, "2" in case the second player won, "X" in case of a tie.

## Loop function

First, the function sends the values recorded by the IR sensors to pure data and waits until it receives the true value for the variable "players_ready".
After receiving the true value for the variable "countdown_finished" as well, it starts a timer during which it calls the checkFingers function, and evaluates the moves executed by the users.
Once recorded, these moves are sent to Pure Data via serial communication, and the program awaits confirmation of receipt from Pure Data.
Once received, the checkWinner function is executed and the winning player in the round is evaluated, increasing the number of executed rounds and, possibly, the number of valid rounds as well.
After returning all global variables to their default values, the loop ends and returns to the initial phase, waiting for the true value to the "players_ready" variable.
