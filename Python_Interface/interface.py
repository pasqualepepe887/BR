import tkinter as tk
from tkinter import *
from tkinter import ttk
import tkinter.font as tkFont
import os
import time
import tkinter.messagebox
import customtkinter
from pynput.keyboard import Key, Controller,Listener

global ip_device
global acc_plus,acc_negative

global key_x_plus
global key_x_negative

global key_y_plus
global key_y_negative

global key_z_plus
global key_z_negative

global x_line,y_line,z_line

global cord_select

global key_posi,key_neg
global line_t_neg,line_t_p

global key_click


cord_select="none"


window = tk.Tk()
window.geometry("1200x650")
window.title("BR")
canvas = Canvas(window, width=1200, height=650,background="gray",bd=0)
a = canvas.create_rectangle(20, 20, 1180, 560, fill='black') # rect generale
aa= canvas.create_rectangle(0, 480, 1200, 500, fill='gray') # rect generale barra
b= canvas.create_rectangle(0, 500, 1200, 650, fill='black') #rect barra


imgsett = tk.PhotoImage(file="./img/sett.png")


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



def change():
    global ip_device
    global acc_negative,acc_plus
    
    global key_x_plus
    global key_x_negative
    
    global key_y_plus
    global key_y_negative
    
    global key_z_plus
    global key_z_negative
    global textbox

    with open('./config/conf.txt', 'w') as f:
            line1 = ip_device +"\n"

            line2= acc_plus +"/"+acc_negative +"\n"
            line3=key_x_plus +"/"+key_x_negative +"\n"
            line4= key_y_plus +"/"+  key_y_negative+"\n"
            line5= key_z_plus +"/"+  key_z_negative+"\n"

            f.writelines([line1,line2,line3,line4,line5])
            f.close()





def slider_eventneg(value):
    global acc_negative,acc_plus
    value= round(value,2)
    acc_negative= str(value)
    valuelabel= "Acc.- trigger: "+acc_negative +" g"
    labelsliderneg.config(text=valuelabel)
    window.after(1000,change)
   # print(value)


def slider_eventplus(value):
    global acc_negative,acc_plus
    value= round(value,2)
    acc_plus= str(value)
    valuelabel= "Acc.+ trigger: "+acc_plus +" g"
    labelsliderplus.config(text=valuelabel)
    window.after(1000,change)
   # print(value)

def on_press(key):
    global key_click
    if key != Key.enter: 
        key_click = key
    #print('{0} pressed'.format(key))
    if key != NONE:
        # Stop listener
        return False



def text_click(textbox):
     global key_click
     with Listener(
          on_press=on_press
          ) as listener:
          listener.join()
     #print(key_click)
     textbox.delete('1.0',END)
     textbox.insert("0.0", str(key_click))


     
 
    




def x_butt_action():
     global key_x_plus
     global key_x_negative
     global cord_select
     global key_neg,key_posi
     global line_t_neg,line_t_p
     global key_click
     #key_x_plus = "CIAO +"
     #key_x_negative = "CIAO -"

     cord_select = "x"

   #  print("X")
     key_posi.destroy()
     key_neg.destroy()
     

     canvas.coords(x_line,580,230,485,230)
     canvas.coords(y_line,580,230,675,230)
     canvas.itemconfig(y_line,fill="red")
     canvas.itemconfig(z_line,fill="black")
     canvas.itemconfig(x_line,fill="red")

     x_butt.config(foreground="red")
     y_butt.config(foreground="white")
     z_butt.config(foreground="white")


     key_posi= Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))
     key_posi.insert("0.0", key_x_plus)  # X+_TEXTBOX
     key_posi.place(x=785, y=215)
     key_posi.bind("<Button-1>",lambda e:window.after(500,text_click(key_posi)))

     #line_t_p=canvas.create_line(785,230,730,230,fill="white") #LINE +

     key_neg= Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))
     key_neg.insert("0.0", key_x_negative)  # X-_TEXTBOX
     key_neg.place(x=200, y=215)
     key_neg.bind("<Button-1>",lambda e:window.after(500,text_click(key_neg)))

     #line_t_neg= canvas.create_line(300,230,430,230,fill="white") #LINE -
     canvas.coords(line_t_p,785,230,730,230)
     canvas.coords(line_t_neg,300,230,430,230) 



def y_butt_action():
     global key_y_plus
     global key_y_negative
     global cord_select
     global key_neg,key_posi
     global line_t_neg,line_t_p

     #print("Y")
     key_posi.destroy()
     key_neg.destroy()
     cord_select = "y"

     canvas.itemconfig(x_line,fill="dark green")
     canvas.itemconfig(y_line,fill="dark green")
     canvas.itemconfig(z_line,fill="black")
     canvas.coords(x_line,580,230,493.5,173.5)
     canvas.coords(y_line,580,230,675,285)  

     x_butt.config(foreground="white")
     y_butt.config(foreground="dark green")
     z_butt.config(foreground="white")

     key_posi= Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))
     key_posi.insert("0.0", key_y_plus)  # Y+_TEXTBOX
     key_posi.place(x=785, y=285)
     key_posi.bind("<Button-1>",lambda e:window.after(500,text_click(key_posi)))
     #canvas.create_line(785,330,730,330,fill="white") #LINE +

     key_neg= Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))
     key_neg.insert("0.0", key_y_negative)  # Y-_TEXTBOX
     key_neg.place(x=200, y=155)
     key_neg.bind("<Button-1>",lambda e:window.after(500,text_click(key_neg)))
     #canvas.create_line(300,130,430,130,fill="white") #LINE -
     canvas.coords(line_t_p,785,300,715,300)
     canvas.coords(line_t_neg,320,170,440,170)  



def z_butt_action():
     global key_z_plus
     global key_z_negative
     global cord_select
     global key_neg,key_posi
     
    # print("Z")
     cord_select = "z"
     key_posi.destroy()
     key_neg.destroy()

     canvas.itemconfig(x_line,fill="blue")
     canvas.itemconfig(y_line,fill="black")
     canvas.itemconfig(z_line,fill="blue")    
     canvas.coords(z_line,580,230,580,120)
     canvas.coords(x_line,580,230,580,330)  

     x_butt.config(foreground="white")
     y_butt.config(foreground="white")
     z_butt.config(foreground="blue")

     key_posi= Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))
     key_posi.insert("0.0", key_z_plus)  # Z+_TEXTBOX
     key_posi.place(x=785, y=105)
     key_posi.bind("<Button-1>",lambda e:window.after(500,text_click(key_posi)))
     #canvas.create_line(785,330,730,330,fill="white") #LINE +

     key_neg= Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))
     key_neg.insert("0.0", key_z_negative)  # Z-_TEXTBOX
     key_neg.place(x=200, y=315)
     key_neg.bind("<Button-1>",lambda e:window.after(500,text_click(key_neg)))
     #canvas.create_line(300,130,430,130,fill="white") #LINE -
     canvas.coords(line_t_p,785,120,680,120)
     canvas.coords(line_t_neg,320,330,470,330)  



#SLIDER-BAR ACC -
sliderneg = customtkinter.CTkSlider(window, from_=0, to=-5, command=slider_eventneg,width=525,height=50,button_color="white",button_hover_color="gray",bd=0)
sliderneg.place(x=20, y=540)
labelsliderneg= tk.Label(window, font=("Arial",15),  background = "black", foreground = "white",   borderwidth = 0,relief="sunken",text=("Acc.- trigger: "+acc_negative +" g"))
labelsliderneg.place(x=180, y=605)


canvas.create_line(580,500,580,650,width=15,fill="gray")#separatore slide-bar

sliderplus = customtkinter.CTkSlider(window, from_=0, to=5, command=slider_eventplus,width=525,height=50,button_color="white",button_hover_color="gray",bd=0)
sliderplus.place(x=620, y=540)
labelsliderplus= tk.Label(window, font=("Arial",15),  background = "black", foreground = "white",   borderwidth = 0,relief="sunken",text=("Acc.+ trigger: "+acc_plus +" g"))
labelsliderplus.place(x=770, y=605)

sliderneg.set(float(acc_negative))
sliderplus.set(float(acc_plus))


canvas.create_oval(430,80,730,380, width = 3,outline="white")#CERCHIO PER ASSI
z_line= canvas.create_line(580,230,580,120,width=3,fill="blue")#Asse z
x_line= canvas.create_line(580,230,485,285,width=3,fill="red")#Asse x
y_line= canvas.create_line(580,230,675,285,width=3,fill="dark green")#Asse y




canvas.create_oval(1050,50,1150,150, width = 3,outline="white")#CERCHIO PER ASSI X
canvas.create_oval(1070,70,1130,130, width = 3,outline="white")#CERCHIO PER ASSI X 2

x_butt= tk.Label(window,text = "X",  background = "black", foreground = "white", font= ("Arial",25), borderwidth = 2)
x_butt.place(x=1087,y =78)

x_butt.bind("<Button-1>",lambda e:window.after(100,x_butt_action()))

canvas.create_oval(1050,200,1150,300, width = 3,outline="white")#CERCHIO PER ASSI Y 
canvas.create_oval(1070,220,1130,280, width = 3,outline="white")#CERCHIO PER ASSI Y 2

y_butt= tk.Label(window,text = "Y",  background = "black", foreground = "white", font= ("Arial",25), borderwidth = 2)
y_butt.place(x=1087,y =228)

y_butt.bind("<Button-1>",lambda e:y_butt_action())

canvas.create_oval(1050,350,1150,450, width = 3,outline="white")#CERCHIO PER ASSI Z
canvas.create_oval(1070,370,1130,430, width = 3,outline="white")#CERCHIO PER ASSI Z 2

z_butt= tk.Label(window,text = "Z",  background = "black", foreground = "white", font= ("Arial",25), borderwidth = 2)
z_butt.place(x=1087,y =378)

z_butt.bind("<Button-1>",lambda e:z_butt_action())


ip_label = tk.Label(window,text = "IP:",  background = "black", foreground = "white", font= ("Arial",15), borderwidth = 5)
ip_label.place(x=30,y =47)


global textbox
#textbox = customtkinter.CTkTextbox(window,width=250, corner_radius=5,height=50,text_color="white",fg_color="black",border_color="white",border_width=2,font=("Arial",15))
textbox = Text(window,height=1,width=15,background="black",foreground="white",font=("Arial",15))

textbox.insert("0.0", ip_device)  # IP_TEXTBOX
textbox.place(x=70, y=50)

key_neg=Text(window)
key_posi=Text(window)
line_t_p=canvas.create_line(0,0,0,0,fill="white")
line_t_neg=canvas.create_line(0,0,0,0,fill="white")

canvas.pack()

#window.iconphoto(False, tk.PhotoImage(file='./img/icona.png'))
window.resizable(False, False)
window.config(background="black")
combostyle = ttk.Style()





def lettura():
    global ip_device
    global acc_plus,acc_negative
    
    global key_x_plus
    global key_x_negative
    
    global key_y_plus
    global key_y_negative
    
    global key_z_plus
    global key_z_negative
    global textbox

    global cord_select
    global key_posi,key_neg

   

    text = textbox.get("1.0", "end")
    text = str(text)
    text=text.strip()
   # print(text)
    #print(ip_device)
    if ip_device != text:
             ip_device = text
             window.after(1000,change)


    if cord_select == "x":
        #print("X SELECT")
        textplus = key_posi.get("1.0", "end")
        textplus = str(textplus)
        textplus=textplus.strip()

        textneg = key_neg.get("1.0", "end")
        textneg = str(textneg)
        textneg=textneg.strip()

        if key_x_plus != textplus:
             key_x_plus = textplus
             window.after(1000,change)

        elif key_x_negative != textneg:
                key_x_negative = textneg
                window.after(1000,change)    

    if cord_select == "y":
        #print("X SELECT")
        textplus = key_posi.get("1.0", "end")
        textplus = str(textplus)
        textplus=textplus.strip()

        textneg = key_neg.get("1.0", "end")
        textneg = str(textneg)
        textneg=textneg.strip()

        if key_y_plus != textplus:
             key_y_plus = textplus
             window.after(1000,change)

        elif key_y_negative != textneg:
                key_y_negative = textneg
                window.after(1000,change) 

    if cord_select == "z":
        #print("X SELECT")
        textplus = key_posi.get("1.0", "end")
        textplus = str(textplus)
        textplus=textplus.strip()

        textneg = key_neg.get("1.0", "end")
        textneg = str(textneg)
        textneg=textneg.strip()

        if key_z_plus != textplus:
             key_z_plus = textplus
             window.after(1000,change)

        elif key_z_negative != textneg:
                key_z_negative = textneg
                window.after(1000,change) 



    window.after(2500,lettura)


window.after(100,lettura)




if __name__ == "__main__":
    window.mainloop()
    print("CHIUSA")
