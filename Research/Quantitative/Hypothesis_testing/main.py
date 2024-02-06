import csv
import os 
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import scipy.stats as stats

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

# Plotting the win percentage for each mode just to visualize the data (not part of the hypothesis testing) -> Uncomment the code below to run it 

# for id in list_unique_ids:
#     game_df = games_df[games_df['id'].str.startswith(id)]

#     # Calculating the win percentage for each mode
#     win_percentage_blind =game_df[game_df["mode"] == "blind"]["winner"].value_counts(ascending=True, normalize=True)*100;
#     win_percentage_not_blind = game_df[game_df["mode"] == "not blind"].winner.value_counts(ascending=True, normalize=True)*100;

#     print(f'Win percentage for {id} in blind mode:\n {win_percentage_blind}')
#     print(f'Win percentage for {id} in not blind mode:\n {win_percentage_not_blind}')

#     labels = ['Blind', 'Not Blind']
#     players = win_percentage_blind.index

#     p1_values = [win_percentage_blind.get('p1', 0), win_percentage_not_blind.get('p1', 0)]
#     p2_values = [win_percentage_blind.get('p2', 0), win_percentage_not_blind.get('p2', 0)]

#     x = np.arange(len(labels))  # the label locations
#     width = 0.35  # the width of the bars
#     fig, ax = plt.subplots()
#     rects1 = ax.bar(x - width/2, p1_values, width, label='P1')
#     rects2 = ax.bar(x + width/2, p2_values, width, label='P2')
#     ax.set_xlabel('Mode')
#     ax.set_ylabel('Win Percentage')
#     #set to 100 to show the percentage
#     ax.set_yticks(np.arange(0, 101, 10))
#     ax.set_yticklabels([f'{int(x)}%' for x in np.arange(0, 101, 10)])
#     ax.set_title(f'Win Percentage for {id}')
#     ax.set_xticks(x)
#     ax.set_xticklabels(labels)
#     ax.legend()

#     # create a folder to save the plots
#     if not os.path.exists('plots'):
#         os.makedirs('plots')
#     plt.savefig(f'plots/winning_rate_{id}.png')

# Hypothesis testing
# The h0 is that the win rate for the blind mode is the same as the win rate for the not blind mode whit a significance level of 0.05
# The h1 is that the win rate for the blind mode is different from the win rate for the not blind mode

non_blind_percentages = []
blind_percentages = []
for ids in list_unique_ids:
    game_df = games_df[games_df['id'].str.startswith(ids)]
    win_percentage_blind_p1 = game_df[game_df["mode"] == "blind"]["winner"].value_counts(normalize=True).get('p1', 0)
    win_percentage_not_blind_p1 = game_df[game_df["mode"] == "not blind"]["winner"].value_counts(normalize=True).get('p1', 0)
    
    win_percentage_blind_p2 = game_df[game_df["mode"] == "blind"]["winner"].value_counts(normalize=True).get('p2', 0)
    win_percentage_not_blind_p2 = game_df[game_df["mode"] == "not blind"]["winner"].value_counts(normalize=True).get('p2', 0)

    non_blind_percentages.append(win_percentage_not_blind_p1)
    # non_blind_percentages.append(win_percentage_not_blind_p2)

    blind_percentages.append(win_percentage_blind_p1)
    # blind_percentages.append(win_percentage_blind_p2)

# Test the distribution of the differences between the win rates

# plot an histogram of the differences
differences = np.array(blind_percentages) - np.array(non_blind_percentages)
print(differences)
plt.hist(differences, bins=20)
plt.title('Differences between the win rates')
plt.xlabel('Difference')
plt.ylabel('Frequency')
plt.savefig('plots/differences.png')

# Test the normality of the differences
# H0: The differences are normally distributed
# H1: The differences are not normally distributed

# Perform the Shapiro-Wilk test for normality
shapiro_test = stats.shapiro(differences)

# Generate a Q-Q plot
fig, ax = plt.subplots(figsize=(6, 4))
stats.probplot(differences, dist="norm", plot=ax)
ax.set_title('Q-Q Plot of Differences')
plt.savefig('plots/qq_plot.png')

# Perform the paired samples t-test
t_stat, p_value = stats.ttest_rel(blind_percentages, non_blind_percentages)

# Print the results
print(f'Shapiro-Wilk test for normality: {shapiro_test}')
print(f'Paired samples t-test: t-statistic = {t_stat}, p-value = {p_value}')
print(f'Are the differences normally distributed? {shapiro_test[1] > 0.05}')
print(f'Are the win rates different? {p_value < 0.05}')

# Plot the win rates for each mode
fig, ax = plt.subplots()
ax.hist(blind_percentages, bins=20, alpha=0.5, label='Blind')
ax.hist(non_blind_percentages, bins=20, alpha=0.5, label='Not Blind')
ax.set_title('Win Rates for Each Mode')
ax.set_xlabel('Win Rate')
ax.set_ylabel('Frequency')
ax.legend()
plt.savefig('plots/win_rates.png')




