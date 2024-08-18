# <div align="center"> Progetto di API </div> <div align="center"> Prova Finale, AA. 2022-2023 </div>

Questa è la prova finale per il corso di Algoritmi e Principi dell'Informatica @ Politecnico di Milano. Il progetto consiste nella realizzazione di un programma in C che, rispettando date specifiche, sia ottimizzato in termini di tempo di esecuzione e spazio allocato in memoria. Questo progetto ha ricevuto la valutazione massima di 30L.

### Specifica

L'obiettivo del progetto consiste nel realizzare in C un programma che pianifichi il percorso ottimale tra due stazioni di servizio in un'autostrada. Considerando informazioni più dettagliate riguardo la specifica del progetto (specifica completa), il programma può ricevere in input i seguenti comandi:
* _+aggiungi-stazione_ : Aggiunge una stazione, posta alla distanza indicata, dotata di numero-auto, dall’autonomia indicata.
* _+demolisci-stazione_ : Rimuove la stazione posta alla distanza indicata, se essa esiste.
* _+aggiungi-auto_ : Se la stazione esiste, aggiunge un’auto alla stessa. È possibile avere più auto con la stessa autonomia.
* _+rottama-auto_ : Rimuove un’automobile dalla stazione indicata, se la stazione esiste ed è dotata di almeno un’automobile con l’autonomia indicata.
* _+pianifica-percorso_ : Richiede di pianificare il percorso con i vincoli sopra indicati. Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazionida inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa nessunpercorso. L’azione di pianificazione non altera le stazioni o il loro parco veicoli. Le stazioni date sono sicuramente presenti.

### Implementazione

La struttura dati scelta per rappresentare l'autostrada è quella di un red-black tree, al fine di effettuare ricerca e inserimento delle stazioni in un tempo logaritmico ( _O(logn)_ ), dal momento che queste consistono nella maggior parte degli input che vengono sottoposti al programma. Considerando che viene dato come constraint nella specifica del progetto il fatto che ogni stazione può ospitare un massimo di 512 automobili, per semplificare la gestione dello stesso ogni parcheggio viene rappresentato come un array di dimensione massima di 512 elementi. Altra struttura dati utilizzata è uno stack, popolato all'esecuzione di un comando _+pianifica_percorso_, al fine di calcolare il percorso ottimale tra le sole stazioni possibili. 

### Vincoli

| Valutazione | Memoria | Tempo |       Esito        |
|-------------|--------:|------:|:------------------:|
| 18          | 128 MiB | 19 s  | :white_check_mark: |
| 21          | 118 MiB | 15 s  | :white_check_mark: |
| 24          | 108 MiB | 10 s  | :white_check_mark: |
| 27          |  98 MiB |  6 s  | :white_check_mark: |
| 30          |  88 MiB |  4 s  | :white_check_mark: |
| 30L         |  78 MiB |  1 s  | :white_check_mark: |
