
# Import libraries for the Serial, Time and Flask interaction
import mysql.connector
import serial
import time
from flask import Flask, render_template

app = Flask(__name__)
    # Define a Dictionary of pins with the anme of the pin and On/Off state
pins = {
        2: {'name' : 'FAN 1', 'state' : 0},
        3: {'name' : 'FAN 2', 'state' : 0},
        4: {'name' : 'BULB', 'state' : 0},
        5: {'name' : 'DOOR LOCK', 'state' : 0}
    }
    # Activated when the Webpage is loaded/refreshed via URL <WebpageIPAddress>:8080
        
@app.route("/")
def index():
    # Read the status of pins from Dictionary and put it into the template
    templateData = {
        'pins' : pins
    }
    # Update the Webpage with the template's contents
    return render_template('index.html',**templateData)
    # Activated when the URL <WebpageIPAddress>:8080/action<something> is passed
@app.route("/<action>")
def action(action):
    # See if the action matches. If yes, write a serial message to the Microcontroller.
    # The Microcontroller needs to receive and interpret the message.
    # Don't forget to update our Dictionary values too, so the refresh works properly.
    if action == 'action1':
        ser.write(b"1")
        pins[2]['state'] = 1
    if action == 'action2':
        ser.write(b"2")
        pins[2]['state'] = 0
    if action == 'action3':
        ser.write(b"3")
        pins[3]['state'] = 1
    if action == 'action4':
        ser.write(b"4")
        pins[3]['state'] = 0
    if action == 'action5':
        ser.write(b"5")
        pins[4]['state'] = 1
    if action == 'action6':
        ser.write(b"6")
        pins[4]['state'] = 0
    if action == 'action7':
        ser.write(b"7")
        pins[5]['state'] = 1
    if action == 'action8':
        ser.write(b"8")
        pins[5]['state'] = 0
    # Like before, read the contents of the Dictionary, update the template,
    # then update the Webpage based on the template.
    templateData = {
        'pins' : pins
    }
    return render_template('index.html',**templateData)
# Activated when the URL <WebpageIPAddress>:8080/<device number>/<on/off> is passed.
@app.route("/<changePin>/<toggle>")
def toggle_function(changePin,toggle):
    # Originally, changePin value is in text. Need to convert it into Integer.
    changePin = int(changePin)
    # Get the real Device Name from the Dictionary. Need this as a label later.
    deviceName = pins[changePin]['name']
    # Check to see if we want to toggle the Device on or off.
    # Then send the appropriate Serial Message to the Microcontroller.
    if toggle == "on":
        if changePin == 2:
            ser.write(b"1")
            pins[changePin]['state'] = 1
        if changePin == 3:
            ser.write(b"3")
            pins[changePin]['state'] = 1
        if changePin == 4:
            ser.write(b"5")
            pins[changePin]['state'] = 1
        if changePin == 5:
            ser.write(b"7")
            pins[changePin]['state'] = 1
            # Then write a label message saying the Device is ON.
            message = "Turned " + deviceName + "on."
    if toggle == "off":
        if changePin == 2:
            ser.write(b"2")
            pins[changePin]['state'] = 0
        if changePin == 3:
            ser.write(b"4")
            pins[changePin]['state'] = 0
        if changePin == 4:
            ser.write(b"6")
            pins[changePin]['state'] = 0
        if changePin == 5:
            ser.write(b"8")
            pins[changePin]['state'] = 0
            # Then write a label message saying the Device is ON.
            message = "Turned " + deviceName + "off."
            # Like before, read the contents of the Dictionary, update the template,
            # then update the Webpage based on the template.
    templateData = {
        'pins' : pins
    }
    return render_template('index.html',**templateData)

def insert_data():
    
    while 1:
        mydb = mysql.connector.connect( host = "localhost",user = "ZashasPI", password = "password", database = "home_automation")
    
    print(mydb)
    
    while(arduino.in_waiting == 0):
        pass
    
    line = arduino.readline()
    sunlight = int(line[0:])
    print(sunlight)
    
    with mydb:
        mycursor = mydb.cursor()
        mycursor.execute("INSERT INTO fantable (`id`, `timestamp`, `fan_status`) VALUES (NULL,DEFAULT,{0})".format(lampstatus))
        
        mycursor.execute("INSERT INTO monitoring_data (`id`,`timestamp`,`humidity`,`temperature`) VALUES (NULL,DEFAULT,{0},{1})".format(sunlight, temperature))
        
# Finally, start the Flask micro-web-framework server,
# and establish the Serial Messaging connection to the Microcontroller.
if __name__ == "__main__":
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.flush()
    app.run(host='0.0.0.0',port=8080,debug=False)
