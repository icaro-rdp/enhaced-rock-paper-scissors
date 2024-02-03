import csv
import os 
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Setting the working directory to the current directory
os.chdir('Research/Quantitative/Hypothesis_testing')

# Reading the data from the csv file and manipulating it
# The output is a new csv file with the cleaned data
with open('games.csv', newline='') as f:
    reader = csv.reader(f)
    
    # Removing the header from the list and adding the id and mode columns
    header = next(reader)
    header.insert(0, "id")
    header.append("mode")
    file_data = list(reader)[0:]

    # Initializing the variables to store the game mode, the game count and the game id
    mode = ''
    count = 0
    gameId = 0
    clean_data = []

    # Looping through the data and cleaning it
    for rows in file_data: 
        rows.insert(0, "")
        
        if rows[1].startswith("C"):
            gameId = rows[1].split("-")[0].replace("C", "G").strip()
            count = 1
        if "test" in rows[1] or "tutorial" in rows[1]:
            mode = 'test'
            continue
        if "blind" in rows[1] and "not blind" not in rows[1]:
            mode = 'blind'
            continue
        if "not blind" in rows[1]:
            mode = 'not blind'
            continue
        
        rows[0] = f"{gameId}R{count}{'B' if mode == 'blind' else 'NB'}"
        clean_data.append(rows)
        rows.append(mode)
        count += 1

    final_data = [rows for rows in clean_data if rows[-1] != 'test']
    final_data.insert(0, header)
    with open('games_cleaned.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(final_data)
    
# Reading the cleaned data and converting it to a pandas dataframe    
games_df = pd.read_csv('games_cleaned.csv')

# If needed statistics with the invalid moves do it above this line

# Removing the rows with invalid moves
games_df = games_df[(games_df['winner'] != 'Invalid move' ) & (games_df['winner'] != 'Draw')]

# Listing the unique IDs in the dataframe 
list_unique_ids = games_df['id'].str.split('R', expand=True)[0].unique()

# Plotting the win percentage for each mode just to visualize the data (not part of the hypothesis testing)
for id in list_unique_ids:
    game_df = games_df[games_df['id'].str.startswith(id)]

    # Calculating the win percentage for each mode
    win_percentage_blind =game_df[game_df["mode"] == "blind"]["winner"].value_counts(ascending=True, normalize=True)*100;
    win_percentage_not_blind = game_df[game_df["mode"] == "not blind"].winner.value_counts(ascending=True, normalize=True)*100;

    print(f'Win percentage for {id} in blind mode:\n {win_percentage_blind}')
    print(f'Win percentage for {id} in not blind mode:\n {win_percentage_not_blind}')

    labels = ['Blind', 'Not Blind']
    players = win_percentage_blind.index

    p1_values = [win_percentage_blind.get('p1', 0), win_percentage_not_blind.get('p1', 0)]
    p2_values = [win_percentage_blind.get('p2', 0), win_percentage_not_blind.get('p2', 0)]

    x = np.arange(len(labels))  # the label locations
    width = 0.35  # the width of the bars
    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width/2, p1_values, width, label='P1')
    rects2 = ax.bar(x + width/2, p2_values, width, label='P2')
    ax.set_xlabel('Mode')
    ax.set_ylabel('Win Percentage')
    #set to 100 to show the percentage
    ax.set_yticks(np.arange(0, 101, 10))
    ax.set_yticklabels([f'{int(x)}%' for x in np.arange(0, 101, 10)])
    ax.set_title(f'Win Percentage for {id}')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    # create a folder to save the plots
    if not os.path.exists('plots'):
        os.makedirs('plots')
    plt.savefig(f'plots/winning_rate_{id}.png')
