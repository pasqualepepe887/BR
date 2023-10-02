# BR
BR - Un dispositivo Open-Source per controllare il Mondo

# DEMO VIDEO
https://github.com/pasqualepepe887/BR/assets/65864547/d3e2f18b-b16f-4497-b7ca-766497c48a9a


Il BR è un dispositivo Open-Source basato su ESP8266 01. Grazie alla connettività Wi-Fi 2.4Ghz il BR può controllare da remoto dispositivi anche diversi km lontani. IL device è dotato di un ADXL345 che permette di rilevare l'accelerazione del polso lungo i tre assi X-Y-Z. Inoltre grazie ad una Batteria da 3.7 V e 600 mAh il BR presentra un elevata autonomia adatta a qualasiasi utilizzo.

Fondamentale per il BR è la personalizzazione. Grazie ad un Interfaccia in Python, è possibile utilizzare il BR per controllarte il proprio PC (Volume, simulazione premuta tasti ecc). 

# PCB
![pcb_BR](https://github.com/pasqualepepe887/BR/assets/65864547/63855d0c-3c9e-40c3-97da-4dee1dee0a10)

Il BR si compone di una PCB personalizzata che rende semplice la connessione dei diversi elementi. L'ESP 01 è affiancata da un AMS1117-3.3 SOT-223 per regolare il voltaggio della batteria. Inoltre sulla PCB è presente unconnettore per un puilsante (TOUCH) per connettere dispositivi esterni come pulsanti, switch ecc.


# SOFTWARE
Il codice in C caricato nell'ESP 01 permette di creare un Wi-Fi temporaneo chiamato Configura Wi-fi la cui password è 'password'. Una volta connessi basta recarsi all'indirizzo 192.168.4.1/ per accedere all'iterfaccia e salvare le credenziali del proprio Wi-Fi sulla memoria EEMPROM. L'ESP una volta connesso crea un Web-Server a cui si può connettere qualiasi dispositivi(IP 192.168.1.45/) e richiedere i dati rilevati dall'ADXL345 tramite l'apposita interfaccia a connettendosi all'indirizzo 192.168.1.45/date.

# INTERFACCIA WEB-SERVER
![Web_Interface](https://github.com/pasqualepepe887/BR/assets/65864547/ab401cd4-4277-4f6c-a0f5-38a1811d877a)

In questo archivio troverai ilcodice relativo al BR  e ad un interfaccia realizzata in Python che permette di comprendere le potenzialità del dispositivo. (L'interfaccia è un esempio di personalizzazione dell'utilizzo del dispositivo.n L'unica barriera è l'immaginazione).

# INTERFACCIA PYTHON
![Interface_Python1](https://github.com/pasqualepepe887/BR/assets/65864547/d5c912a4-464b-4564-85de-3d957ad3894a)
![Interface_Python2](https://github.com/pasqualepepe887/BR/assets/65864547/a7c3d388-4e37-4724-9eaa-7ae690c90d8f)
