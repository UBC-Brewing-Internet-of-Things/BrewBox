import serial
import pandas as pd

# Open the serial port
ser = serial.Serial('COM4', 9600)  # Replace 'COM4' with your Arduino's serial port


#create a csv file to store the data if it doesn't exist


# load the csv file or create a new one as pandas df
df = pd.read_csv('data.csv')
print(df)
# Create a CSV file to store the data



READ_PACKET = False
SEND_MESSAGE = True

# Read and write data
while True:
    # Read data from Arduino
    data = ser.readline().decode().strip()  # Decode the received bytes and remove any leading/trailing whitespace
    
    if data == "Packet End":
        READ_PACKET = False
        # Write the data to the csv file
        df.to_csv('data.csv', index=False)

    if READ_PACKET:
        # Data: Disolved O2,time,Temperature,time,PH,time

        # If data contains Oxygen, next data is value, next data is time
        if "Oxygen" in data:
            df.loc[len(df)] = [data.split(',')[1], data.split(',')[2], None, None, None, None]

        # If data contains RTD
        if "RTD" in data:
            df.loc[len(df)] = [None, None, data.split(',')[1], data.split(',')[2], None, None]

        # If data contains pH
        if "pH" in data:
            df.loc[len(df)] = [None, None, None, None, data.split(',')[1], data.split(',')[2]]

        # Print the received data
        print(df)

    if data == "Packet Start":
        READ_PACKET = True

    print(data)

    if data == "Send Command":
        # Send data to Arduino from Keyboard
        ser.write(input().encode())
        ser.write(b'\n') #send new line to indicate end of command




# Close the serial port and CSV file

ser.close()
csv_file.close()