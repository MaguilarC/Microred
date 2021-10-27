#config. puerto UART y establecido como objeto 'ser'
ser= serial.Serial('/dev/ttyAMA1',115200,timeout=0.001)

ser.flush()  #se limpia el buffer del puerto Serial

#se establece el tamano de la lista y variables 
x=[0]*30
y=[0]*16
lect=[0]*16
ref=[0]*8
i=0
j=0
n=0

print ('Iniciando')

while True: #Ciclo infinito

    if(ser.in_waiting>0): #si hay algo en el buffer del puerto Serial

        line= ser.readline().decode('ascii').rstrip() #se lee la linea
						      #completa y se deco
                                               #difica en formato ASCII
       #se separan en listas indep. segun el caracter
	x=line.split(";")  
        y[0]=x[0].split("#")
        y[1]=x[1].split("#")

	#se guardan los valores en la variable correspondiente
        if(y[0][0]=='1'):
            lect[0]=int(y[0][1])
            lect[1]=int(y[1][1])
        if(y[0][0]=='2'):
            lect[2]=int(y[0][1])
            lect[3]=int(y[1][1])
        if(y[0][0]=='3'):
            lect[4]=int(y[0][1])
            lect[5]=int(y[1][1])
        if(y[0][0]=='4'):
            lect[6]=int(y[0][1])
            lect[7]=int(y[1][1])
        if(y[0][0]=='5'):
            lect[8]=int(y[0][1])
            lect[9]=int(y[1][1])
        if(y[0][0]=='6'):
            lect[10]=int(y[0][1])
            lect[11]=int(y[1][1])
        if(y[0][0]=='7'):
            lect[12]=int(y[0][1])
            lect[13]=int(y[1][1])
        if(y[0][0]=='8'):
            lect[14]=int(y[0][1])
            lect[15]=int(y[1][1])

	print(lect) #se imprime en consola
    
ser.close()  #se cierra el puerto UART
print ('Fin')
