# BR
BR - Un dispositivo Open-Source per controllare il Mondo

#LINK YOUTUBE

Il BR è un dispositivo Open-Source basato su ESP8266 01. Grazie alla connettività Wi-Fi 2.4Ghz il BR può controllare da remoto dispositivi anche diversi km lontani. IL device è dotato di un ADXL345 che permette di rilevare l'accelerazione del polso lungo i tre assi X-Y-Z. Inoltre grazie ad una Batteria da 3.7 V e 600 mAh il BR presentra un elevata autonomia adatta a qualasiasi utilizzo.

Fondamentale per il BR è la personalizzazione. Grazie ad un Interfaccia in Python, è possibile utilizzare il BR per controllarte il proprio PC (Volume, simulazione premuta tasti ecc). 

Il BR si compone di una PCB personalizzata che rende  semplice la connessione dei diversi elementi.

#FOTO PCB

Il codice in C caricato nell'ESP 01 permette di salvare le credenziali del proprio Wi-Fi sulla memoria EEMPROM. L'ESP una volta connesso crea un Web-Server a cui si può connettere qualiasi dispositivi e richiedere i dati rilevati dall'ADXL345.

#FOTO INTERFACCIA WEB-SERVER

In questo archivio troverai ilcodice relativo al BR  e ad un interfaccia realizzata in Python che permette di comprendere le potenzialità del dispositivo. (L'interfaccia è un esempio di personalizzazione dell'utilizzo del dispositivo.n L'unica barriera è l'immaginazione).

#FOTO INTERFACCIA
