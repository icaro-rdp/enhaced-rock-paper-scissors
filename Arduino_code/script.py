import serial
from serial import Serial
import csv
import time

#time.sleep(10)

ser = serial.Serial('COM5', 9600)  # Sostituisci 'COM3' con la porta seriale corretta
#with open('output.csv', 'w', newline='') as file:
#    writer = csv.writer(file)
#    while True:
#        data = ser.readline().decode('utf-8').rstrip()
#        writer.writerow([data])


# Create a CSV file for saving the data
csv_file = open('data.csv', 'w', newline='')
csv_writer = csv.writer(csv_file)
#csv_writer.writerow(['Timestamp', 'Weight'])

try:
    while True:
        # Read a line of serial data from Arduino
        serial_data = ser.readline().decode('utf-8').strip()
        
        # Split the CSV values and save them
        #timestamp, weight = serial_data.split(',')
        csv_writer.writerow([serial_data])
        csv_file.flush()  # Ensure immediate writing to file
        
except KeyboardInterrupt:
    pass

# Close the CSV file and serial connection
csv_file.close()
ser.close()