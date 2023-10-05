from urllib.request import urlopen
from pynput.keyboard import Key, Controller,KeyCode
import time

#x/y/z

global ip_device
global acc_plus,acc_negative

global key_x_plus
global key_x_negative

global key_y_plus
global key_y_negative

global key_z_plus
global key_z_negative


#url = "http://192.168.1.55"
keyboard = Controller()

timer = 0.6

def lett_conf():
    global ip_device
    global acc_plus,acc_negative
    
    global key_x_plus
    global key_x_negative
    
    global key_y_plus
    global key_y_negative
    
    global key_z_plus
    global key_z_negative

    config = open("./config/conf.txt","r") 

    ip_device= config.readline()
    ip_device= str(ip_device) #IP
    ip_device = ip_device.strip()
  
    acc_date =config.readline()#ACC DATE
    acc_date = str(acc_date)
    acc_date= acc_date.strip()

    sed = acc_date.find("/") #ACC_+
    acc_plus= acc_date[0:sed]
    #print(acc_plus)

    acc_negative = acc_date[sed+1:] #ACC_-
    #print(acc_negative)



    x_date =config.readline() #X_DATE
    x_date = str(x_date)
    x_date= x_date.strip()
    

    sed = x_date.find("/") #KEY_X_+
    key_x_plus= x_date[0:sed]
   # print(key_x_plus)

    key_x_negative = x_date[sed+1:] #KEY_X-
   # print(key_x_negative)

   # x_date = x_date[sed+1:]

    y_date =config.readline() #Y_DATE
    y_date = str(y_date)
    y_date= y_date.strip()
    

    sed = y_date.find("/") #KEY_y_+
    key_y_plus= y_date[0:sed]
   # print(key_y_plus)

    key_y_negative = y_date[sed+1:] #KEY_y-
    #print(key_y_negative)


    z_date =config.readline() #Z_DATE
    z_date = str(z_date)
    z_date= z_date.strip()
    

    sed = z_date.find("/") #KEY_z_+
    key_z_plus= z_date[0:sed]
    #print(key_z_plus)

    key_z_negative = z_date[sed+1:] #KEY_z-
  #  print(key_z_negative)

    config.close()

lett_conf()
#print(ip_device,acc_plus,acc_negative,key_x_negative,key_x_plus,key_y_negative,key_y_plus,key_z_negative,key_z_plus)


ip_device = "http://"+ip_device+"/date"

while True:

    page = urlopen(ip_device)
    html = page.read().decode("utf-8")
    print(html)
    
    var = html.find("/")
    xacc= html[0:var]
    #print(xacc)
    html = html[var+1:]
    xacc = float(xacc)

    var = html.find("/")
    yacc= html[0:var]
    #print(yacc)
    html = html[var+1:]
    yacc=float(yacc)
    


    var = html.find("/")
    zacc= html[0:var]
    #print(zacc)
    zacc = float(zacc)

    acc_plus = float(acc_plus)
    acc_negative= float(acc_negative)

    #print(acc_plus)
    if xacc>= acc_plus :
        if key_x_plus != "none":
          print("DESTRA")
          keyboard.press(eval(key_x_plus))
          keyboard.release(eval(key_x_plus))
          time.sleep(timer)

    elif xacc<=acc_negative:
        if key_x_negative != "none":
           print("SINISTRA")
           keyboard.press(eval(key_x_negative))
           keyboard.release(eval(key_x_negative))
           time.sleep(timer)


    if yacc>= acc_plus :
        if key_y_plus != "none":       
          # print("DESTRA")
           keyboard.press(eval(key_y_plus))
           keyboard.release(eval(key_y_plus))
           time.sleep(timer)

    elif yacc<=acc_negative:
        if key_y_negative != "none":
         #  print("SINISTRA")
           keyboard.press(eval(key_y_negative))
           keyboard.release(eval(key_y_negative))
           time.sleep(timer)


    if zacc>= acc_plus :
        if key_z_plus != "none":       
           print("s")
           keyboard.press(eval(key_z_plus))
           keyboard.release(eval(key_z_plus))
           time.sleep(timer)

    elif zacc<=acc_negative:
        if key_z_negative != "none":
           print("g")
           keyboard.press(eval(key_z_negative))
           keyboard.release(eval(key_z_negative))
           time.sleep(timer)
