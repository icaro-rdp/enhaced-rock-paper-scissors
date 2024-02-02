import csv
import os 
import matplotlib.pyplot as plt

# Setting the working directory to the current directory
os.chdir('Research/Quantitative/Hypothesis_testing')

with open('games.csv', newline='') as f:
    reader = csv.reader(f)
    # Removing the header from the list
    header = next(reader)
    header.insert(0, "id")
    header.append("mode")
    file_data = list(reader)[0:]
    mode = ''
    
    count = 0
    gameId = 0
    clean_data = []
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
        
        rows[0] = f"{gameId}R{count}"
        clean_data.append(rows)
        rows.append(mode)
        count += 1

    final_data = [rows for rows in clean_data if rows[-1] != 'test']
    final_data.insert(0, header)
    # write a new file with the cleaned data
    with open('games_cleaned.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(final_data)
    
#transforming the data into a dictionary
    
with open('games_cleaned.csv', newline='') as f:
   csv_reader = csv.DictReader(f)
   data = [row for row in csv_reader]


def count_moves(move, dict):
    count = 0
    for row in dict:
        if row['move1'] == move:
            count += 1
        if row['move2'] == move:
            count += 1
    return f"The move {move} was played {count} times"

def count_moves_by_mode(move, mode, dict):
    count = 0
    for row in dict:
        if row['move1'] == move and row['mode'] == mode:
            count += 1
        if row['move2'] == move and row['mode'] == mode:
            count += 1
    return f"The move {move} was played {count} times in {mode} mode"

print(count_moves('rock', data))
print(count_moves('paper', data))
print(count_moves('scissors', data))
print("")
print(count_moves_by_mode('rock', 'blind', data))
print(count_moves_by_mode('paper', 'blind', data))
print(count_moves_by_mode('scissors', 'blind', data))
print("")
print(count_moves_by_mode('rock', 'not blind', data))
print(count_moves_by_mode('paper', 'not blind', data))
print(count_moves_by_mode('scissors', 'not blind', data))


