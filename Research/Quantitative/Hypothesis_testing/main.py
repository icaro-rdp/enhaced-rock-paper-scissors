import csv
import os 

# Setting the working directory to the current directory
os.chdir('Research/Quantitative/Hypothesis_testing')

with open('games.csv', newline='') as f:
    reader = csv.reader(f)
    # Removing the header from the list
    file_data = list(reader)[1:]
    mode = ''

    for rows in file_data:
        if "test" in rows[0] or "tutorial" in rows[0]:
            mode = 'test'
            continue
        elif "blind" in rows[0] and "not blind" not in rows[0]:
            mode = 'blind'
            continue
        elif "not blind" in rows[0]:
            mode = 'not blind'
            continue
        rows.append(mode)

    # write a new file with the cleaned data
    with open('games_cleaned.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(file_data)


