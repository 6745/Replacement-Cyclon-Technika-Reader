import serial
import time
import argparse
print( "by Github.com/6745")
parser = argparse.ArgumentParser()
parser.add_argument("COM", help="Serial port identifier")
parser.add_argument("BAUD", type=int, help="Baud rate")
args = parser.parse_args()

ser = serial.Serial(args.COM, args.BAUD)

while True:
    data = ser.readline().decode('utf-8').strip()
    if data.startswith("Name: "):
        name = data[23:]
        name = name.replace(" ", "").replace("\x00", "") ## removes prints null chars to card from comport this is here for a reason dont remove it
        print(name) #debug 
        with open("CardNum.txt", "w") as file:
            file.write(name)
        time.sleep(5) # waits 5 seconds before nulling the card.
        with open("CardNum.txt", "w") as file:
            file.write("")
