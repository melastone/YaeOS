# YaeOS
*Yet Another Educational Operating System*

Progetto di Laboratorio del corso di Sistemi Operativi dell'Università di Bologna per l'anno accademico 2017/2018. 

Software di un Sistema Operativo organizzato su più livelli. 
Il software è studiato per essere eseguito sull'emulatore uARM basato sul processore ARM7TDMI.

### Fase 1
La fase 1 del progetto consiste nella realizzazione del Livello 2 del Sistema Operativo: il __Gestore delle code__.

Questo livello implementa le funzionalità relative ai Process Control Blocks, le strutture di dati dei processi. Nello specifico esso contiene due librerie, le quali gestiscono code ed alberi di PCB ed una Active Semaphore Hash Table che si occupa dei processi bloccati sui semafori. 


## PREREQUISITI

Per poter eseguire il software è necessario scaricare l'emulatore [uARM](https://github.com/mellotanica/uARM)

## INSTALLAZIONE

Per decomprimere il progetto, spostarsi nella directory di download ed eseguire i comando 

```
$ cd /x
$ tar -xvf YaeOS.tar.gz
```

dove x va sostituito con il percorso del file YaeOS.tar.gz .
Per ottenere il file eseguibile spostarsi nella cartella ottenuta dall'estrazione e digitare il comando

`$ make`

Per la pulizia dei file binari generati durante la compilazione eseguire 

`$ make clean`

## TEST P1

Per effettuare il test della Phase 1, una volta decompresso il file .tar.gz, eseguire il comando 

`$ make test`

seguito da 

`$ make clean`

per generare l'eseguibile chiamato __test__.
Dopodichè avviare l'emulatore uARM digitando nel terminale

`$ uarm`

Dalla schermata che si aprirà, cliccare sul menù delle impostazioni ed attraverso il pulsante __Browse..__ della sezione __Core File__ selezionare l'eseguibile generato precedentemente.
Accendere poi la macchina attraverso il pulsante di Power on, aprire uno dei terminali dal menù Terminals ed avviare il test cliccando sul tasto Play. 

## AUTORI

- [Daniel Veroli](https://github.com/denzelbass)
- [Lorenzo Turrini](https://github.com/lorenzoturrini)
- [Lorenzo Poluzzi](https://github.com/lorenzopoluzzis)
- [Melania Ghelli](https://github.com/melastone)

## LICENZA

## INFORMAZIONI E LINK UTILI

- [microARM: an ARM emulator](http://mellotanica.github.io/uARM/)
- [Specifiche per lo svolgimento del progetto](http://www.cs.unibo.it/~renzo/so/progetto.shtml) del professore Renzo Davoli
