import csv
import os 

# Setting the working directory to the current directory
os.chdir('Research/Quantitative/Hypothesis_testing')

with open('games.csv', newline='') as f:
    reader = csv.reader(f)
    file_data = list(reader)

print(file_data)
