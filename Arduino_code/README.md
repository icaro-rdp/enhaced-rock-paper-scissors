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
- `limit_value`: The limit value for the flex sensor.
- `hapticPin_a`: The pin number where first haptic motor is connected.
- `hapticPin_b`: The pin number where second haptic motor is connected.

## Setup

The setup routine runs once when you press reset. It initializes serial communication at 9600 bits per second and sets the button pin as an input and haptic pins as output.

## Functions

### check_move

This function checks what move a user is currently performing and classifies it as "rock," "paper," "scissors," or "invalid." The classification is done based on the flex sensor value obtained from each individual finger.
It is performed in parallel for both users.
The return value is of type String.

### checkFingers

This function reads the sensor value of each finger continuously and then calls the check_move function to classify the user's move.
The return value is an array of String two values, containing the moves performed by the user at that time. 

### giveFeedback

This function returns haptic feedback attrvaerso small haptic motors attached to the user's wrist.
The feedbacks, calssified by "win," "lose," "draw," or "null," are all distinct from each other, except for "draw" and "null" which are classified the same way.

### checkWinner

This function, once it obtains the moves made at the end of the round by the two users, compares them and determines the winner of that round, increasing the variable related to the number of rounds won by the user.
It calls, for each result obtained, the giveFeedback function, passing the value of the result.
The return value is a character representing either user's win or tie.

### checkFinalWinner

This function is called at the end of the last round. It compares the numbers of rounds won by the two users and returns the final result, ruling a match winner or a tie.
