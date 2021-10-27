
#libreria para uso del bluetooth
from bluetooth import *
                               
#-------- MAC bluetooth de los ESP32 -----------

addr_intermediario = "AC:67:B2:3B:C8:EE"
addr1= "AC:67:B2:3B:DE:F6"
addr2= "AC:67:B2:3C:02:32"
addr3= "AC:67:B2:3B:C5:02"
addr4= "AC:67:B2:3B:E3:82"
addr5= "AC:67:B2:3B:FD:02"
addr6= "AC:67:B2:3C:21:2E"
addr7= "AC:67:B2:3B:FF:52"
#addr8= "AC:67:B2:3B:E0:1E"

#-------- Fin MAC bluetooth de los ESP32 -----------


#----config. adicional deshabilitada-----------

#uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"
#service_matches = find_service( uuid = uuid, address = addr )

#-------------------------------------------------

#----busqueda de dispositivos con la MAC otorgada
 
mod1_con = find_service( address = addr1 )
mod2_con = find_service( address = addr2 )
mod3_con = find_service( address = addr3 )
mod4_con = find_service( address = addr4 )
mod5_con = find_service( address = addr5 )
mod6_con = find_service( address = addr6 )
mod7_con = find_service( address = addr7 )
#mod8_con = find_service( address = addr8 )

#-----------------------------------------------


buf_size = 1024 #designacion tamaño buffer bluetooth

#informacion adicional resultados de busqueda
#---------------------------------------------------------
"""
if len(service_matches) == 0:
    print("couldn't find the SampleServer service =(")
    sys.exit(0)

for s in range(len(service_matches)):
    print("\nservice_matches: [" + str(s) + "]:")
    print(service_matches[s])"""
#---------------------------------------------------------


#------------Adquisicion de info. de los ESP32 para conexion bluetooth----------

#-----info mod 1
mod1_inf = mod1_con[0];port_mod1 = mod1_inf["port"];name_mod1 = mod1_inf["name"]
host_mod1 = mod1_inf["host"]
#-----info mod 2
mod2_inf = mod2_con[0];port_mod2 = mod2_inf["port"];name_mod2 = mod2_inf["name"]
host_mod2 = mod2_inf["host"]
#-----info mod 3
mod3_inf = mod3_con[0];port_mod3 = mod3_inf["port"];name_mod3 = mod3_inf["name"]
host_mod3 = mod3_inf["host"]
#-----info mod 4
mod4_inf = mod4_con[0];port_mod4 = mod4_inf["port"];name_mod4 = mod4_inf["name"]
host_mod4 = mod4_inf["host"]
#-----info mod 5
mod5_inf = mod5_con[0];port_mod5 = mod5_inf["port"];name_mod5 = mod5_inf["name"]
host_mod5 = mod5_inf["host"]
#-----info mod 6
mod6_inf = mod6_con[0];port_mod6 = mod6_inf["port"];name_mod6 = mod6_inf["name"]
host_mod6 = mod6_inf["host"]
#-----info mod 7
mod7_inf = mod7_con[0];port_mod7 = mod7_inf["port"];name_mod7 = mod7_inf["name"]
host_mod7 = mod7_inf["host"]
'''
#-----info mod 8
mod8_inf = mod8_con[0];port_mod8 = mod8_inf["port"];name_mod8 = mod8_inf["name"]
host_mod8 = mod8_inf["host"]
'''
#----------Fin adquisicion de info. de los ESP32 para conexion bluetooth--------


# imprimir por consola todos los dispositivos que se quiere conectar
print("conectando a \"%s\" en %s, port %s" % (name_mod1, host_mod1, port_mod1))
print("conectando a \"%s\" en %s, port %s" % (name_mod2, host_mod2, port_mod2))
print("conectando a \"%s\" en %s, port %s" % (name_mod3, host_mod3, port_mod3))
print("conectando a \"%s\" en %s, port %s" % (name_mod4, host_mod4, port_mod4))
print("conectando a \"%s\" en %s, port %s" % (name_mod5, host_mod5, port_mod5))
print("conectando a \"%s\" en %s, port %s" % (name_mod6, host_mod6, port_mod6))
print("conectando a \"%s\" en %s, port %s" % (name_mod7, host_mod7, port_mod7))
#print("conectando a \"%s\" en %s, port %s" % (name_mod8, host_mod8, port_mod8))

#--------------Creacion socket del cliente y conexion---------------------------
sock_mod1=BluetoothSocket(RFCOMM);sock_mod1.connect((host_mod1, port_mod1))
sock_mod2=BluetoothSocket(RFCOMM);sock_mod2.connect((host_mod2, port_mod2))
sock_mod3=BluetoothSocket(RFCOMM);sock_mod3.connect((host_mod3, port_mod3))
sock_mod4=BluetoothSocket(RFCOMM);sock_mod4.connect((host_mod4, port_mod4))
sock_mod5=BluetoothSocket(RFCOMM);sock_mod5.connect((host_mod5, port_mod5))
sock_mod6=BluetoothSocket(RFCOMM);sock_mod6.connect((host_mod6, port_mod6))
sock_mod7=BluetoothSocket(RFCOMM);sock_mod7.connect((host_mod7, port_mod7))
#sock_mod8=BluetoothSocket(RFCOMM);sock_mod8.connect((host_mod8, port_mod8))
#--------------Fin creacion socket del cliente y conexion-----------------------

print("connected")

#declaracion variables globales
count=0
ref=[0]*8
lect=[0]*16
x=[0]*2

while True:  #ciclo infinito
    
    count=count+1 #contador de modulo ESP32
    if count>7:
        count=1
    

    if count==1:
        #//se copia la ref. a un formato de Trama Virtual
        buf1 = "A%dB" % (ref[0])
        #vector que se manda por bluetooth
        sock_mod1.send(buf1.encode('ascii'))
        #Trama Virtual recivida por bluetooth
        data1 = sock_mod1.recv(buf_size).decode('ascii')
        #Si se ha enviado el carac. 'F' o el tamaño es mayor al correspondido
        if("F" in data1)or(len(data1)>9):
            data1=[None]*10 #se borra el contenido
        #si e mensaje es correcto    
        else:
            x=data1.split(";") #se separa segun el carac. ';'
            lect[0]=int(x[0]);lect[1]=int(x[1]) #se convierte a numero
            #se establece la ref. segun el valor de la lectura
            if (lect[0]>3500)or(lect[1]>3500): 
                ref[0]=1
            else:
                ref[0]=0
    
    if count==2:
        buf2 = "A%dB" % (ref[1])
        sock_mod2.send(buf2.encode('ascii'))
        data2 = sock_mod2.recv(buf_size).decode('ascii')
        if("F" in data2)or(len(data2)>9):
            data2=[None]*10
        else:
            x=data2.split(";")
            lect[2]=int(x[0]);lect[3]=int(x[1])
            if (lect[2]>3500)or(lect[3]>3500):
                ref[1]=1
            else:
                ref[1]=0
    
    if count==3:
        buf3 = "A%dB" % (ref[2])
        sock_mod3.send(buf3.encode('ascii'))
        data3 = sock_mod3.recv(buf_size).decode('ascii')
        if("F" in data3)or(len(data3)>9):
            data3=[None]*10
        else:
            x=data3.split(";")
            lect[4]=int(x[0]);lect[5]=int(x[1])
            if (lect[4]>3500)or(lect[5]>3500):
                ref[2]=1
            else:
                ref[2]=0
    
    if count==4:
        buf4 = "A%dB" % (ref[3])
        sock_mod4.send(buf4.encode('ascii'))
        data4 = sock_mod4.recv(buf_size).decode('ascii')
        if("F" in data4)or(len(data4)>9):
            data4=[None]*10
        else:
            x=data4.split(";")
            lect[6]=int(x[0]);lect[7]=int(x[1])
            if (lect[6]>3500)or(lect[7]>3500):
                ref[3]=1
            else:
                ref[3]=0
    
    if count==5:
        buf5 = "A%dB" % (ref[4])
        sock_mod5.send(buf5.encode('ascii'))
        data5 = sock_mod5.recv(buf_size).decode('ascii')
        if("F" in data5)or(len(data5)>9):
            data5=[None]*10
        else:
            x=data5.split(";")
            lect[8]=int(x[0]);lect[9]=int(x[1])
            if (lect[8]>3500)or(lect[9]>3500):
                ref[4]=1
            else:
                ref[4]=0
    
    if count==6:
        buf6 = "A%dB" % (ref[5])
        sock_mod6.send(buf6.encode('ascii'))
        data6 = sock_mod6.recv(buf_size).decode('ascii')
        if("F" in data6)or(len(data6)>9):
            data6=[None]*10
        else:
            x=data6.split(";")
            lect[10]=int(x[0]);lect[11]=int(x[1])
            if (lect[10]>3500)or(lect[11]>3500):
                ref[5]=1
            else:
                ref[5]=0
    
    if count==7:
        
        buf7 = "A%dB" % (ref[6])
        sock_mod7.send(buf7.encode('ascii'))
        data7 = sock_mod7.recv(buf_size).decode('ascii')
        if("F" in data7)or(len(data7)>9):
            data7=[None]*10
        else:
            x=data7.split(";")
            lect[12]=int(x[0]);lect[13]=int(x[1])
            if (lect[12]>3500)or(lect[13]>3500):
                ref[6]=1
            else:
                ref[6]=0
                
        
    '''   
    if count==8:
        
        buf8 = "A%dB" % (ref[7])
        sock_mod8.send(buf8.encode('ascii'))
        data8 = sock_mod8.recv(buf_size).decode('ascii')
        if("F" in data8)or(len(data8)>9):
            data8=[None]*10
        else:
            x=data8.split(";")
            lect[14]=int(x[0]);lect[15]=int(x[1])
            if (lect[14]>3500)or(lect[15]>3500):
                ref[7]=1
            else:
                ref[7]=0
        
    '''    
        print(ref) #se imprime por consola la ref.
        print(lect) #se imprime por consola todas las lecturas
    
