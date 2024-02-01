import csv
import os 

# Setting the working directory to the current directory
os.chdir('Research/Quantitative/Hypothesis_testing')

with open('example.csv', newline='') as f:
    reader = csv.reader(f)
    # Removing the header from the list
    file_data = list(reader)
    for rows in file_data[:10]:
        print(rows)


