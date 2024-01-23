### 1. Definizione delle Ipotesi

- **Ipotesi Nulla (H0)**: La vista non ha un impatto significativo sulle percentuali di vittoria nel gioco di sasso, carta e forbice.
- **Ipotesi Alternativa (H1)**: La vista ha un impatto significativo sulle percentuali di vittoria nel gioco di sasso, carta e forbice.

### 2. Selezione dei Partecipanti

- **Campionamento**: Assicuratevi che i partecipanti siano rappresentativi della popolazione che volete studiare.
  - campionamento casuale semplice (ogni membro della popolazione ha la stessa probabilità di essere scelto)
  - in questo caso i partecipanti sono stati scelti in modo casuale tra gli studenti dell'Università di Trento.
- **Esclusione**: Escludete i partecipanti che non soddisfano i criteri di inclusione (ad esempio, non hanno familiarità con il gioco di sasso, carta e forbice).

  - in questo caso i partecipanti sono stati esclusi se non avevano familiarità con il gioco.

- **Numero di Partecipanti**: Determinate la dimensione del campione necessaria per avere potenza statistica.

### 3. Progettazione Sperimentale

- **Condizioni Sperimentali**: Due condizioni - giocare con la vista (normale) e senza la vista (ad esempio, con una benda sugli occhi o in qualche altro modo).
- **Ordine delle Condizioni**: Alternate l'ordine in cui i partecipanti giocano nelle due condizioni per evitare effetti di ordine o di affaticamento.

### 4. Raccolta dei Dati

- **Misure**: Registrate la percentuale di vittorie, sconfitte e pareggi per ogni partecipante in entrambe le condizioni.
  - Dovrebbe essere fatto con un logging su CSV, in modo da poterlo analizzare con un programma esterno.
- **Controllo delle Variabili Confondenti**: Assicuratevi di controllare le variabili che potrebbero influenzare i risultati (come la familiarità con il gioco).
  - in questo caso la familiarità è controllata dalla rigidità del sistema in entrambe le condizioni.

### 5. Analisi dei Dati

- **Test Statistico**: Potreste utilizzare un test t per campioni appaiati per confrontare le medie delle percentuali di vittoria tra le due condizioni.

  - in questo caso è stato utilizzato il test di Wilcoxon per campioni appaiati, in quanto i dati non sono distribuiti normalmente.

- **Significatività Statistica**: Scegliete un livello di significatività (ad esempio, p < 0.05) per determinare se rifiutare l'ipotesi nulla.
- **Potenza Statistica**: Determinate la potenza statistica del vostro test (ad esempio, 1 - beta = 0.80) per determinare la probabilità di rifiutare l'ipotesi nulla quando è falsa.
- **Dimensione dell'Effetto**: Calcolate la dimensione dell'effetto (ad esempio, d = 0.80) per determinare la forza della relazione tra le variabili.

La dimensione del campione per un test t per campioni appaiati dipende da diversi fattori, tra cui l'effetto che si prevede di osservare, la variabilità dei dati, il livello di significatività (α) e la potenza desiderata dello studio (1 - β, dove β è l'errore di tipo II). Ecco una guida generale su come calcolarlo:

1. **Effetto Previsto (d)**: La differenza che ti aspetti di trovare tra le due condizioni. Questo può essere basato su studi precedenti o su considerazioni teoriche.

2. **Deviazione Standard (σ)**: La variabilità dei dati. Se hai dati preliminari o studi simili su cui basarti, puoi usare questi per stimare σ.

3. **Livello di Significatività (α)**: Comunemente impostato su 0.05, il che significa che sei disposto ad accettare una probabilità del 5% di rifiutare erroneamente l'ipotesi nulla quando è vera (errore di tipo I).

4. **Potenza (1 - β)**: La probabilità di rifiutare correttamente l'ipotesi nulla quando è falsa. Spesso è impostata su 0.80 o 0.90.

Con queste informazioni, puoi usare la formula per la dimensione del campione per un test t per campioni appaiati:

\[ n = \left( \frac{(1.96 + 0.84)^2 \times 2 \times \sigma^2}{d^2} \right) \]

dove:

- \( n \) è la dimensione del campione per ciascun gruppo.
- \( Z*{1-\frac{\alpha}{2}} \) è il valore Z corrispondente al livello di significatività (ad esempio, per α = 0.05, \( Z*{1-\frac{\alpha}{2}} \) è circa 1.96).
- \( Z*{1-\beta} \) è il valore Z corrispondente alla potenza dello studio (ad esempio, per β = 0.20, \( Z*{1-\beta} \) è circa 0.84).

Se non hai valori specifici per \( d \) e \( \sigma \), potresti dover fare delle stime ragionevoli basate su ricerche esistenti o su un piccolo studio pilota.

- Usando lo studio pilota, puoi stimare \( d \) come la differenza tra le medie dei due gruppi.
- Puoi stimare \( \sigma \) come la deviazione standard dei dati del tuo studio pilota.

Un'altra opzione è utilizzare software statistici o calcolatori online:

Certo, ci sono vari software e calcolatori online che possono aiutarti a determinare la dimensione del campione per un test t per campioni appaiati. Ecco alcuni esempi:

1. **G\*Power**: È uno dei software più popolari per calcoli di potenza statistica e dimensione del campione. È gratuito e fornisce un'ampia gamma di test, inclusi i test t per campioni appaiati. Puoi scaricarlo da [http://www.gpower.hhu.de/](http://www.gpower.hhu.de/).

2. **Statistical Solutions**: Offre un calcolatore di dimensione del campione online che può essere utilizzato per vari tipi di test, compresi i test t. Puoi trovarlo su [https://www.statsols.com/sample-size-calculator](https://www.statsols.com/sample-size-calculator).

3. **Calculator.net**: Ha un semplice calcolatore di dimensione del campione che può essere utile per stime rapide e approssimative. Visita [https://www.calculator.net/sample-size-calculator.html](https://www.calculator.net/sample-size-calculator.html) per maggiori dettagli.

4. **Sample Size Calculators for Designing Clinical Research**: Questo sito web offre una serie di calcolatori specifici per la ricerca clinica, ma possono essere utilizzati anche per altri tipi di ricerca. Il link è [http://www.sample-size.net/](http://www.sample-size.net/).

5. **IBM SPSS SamplePower**: Se hai accesso a SPSS, questo è un ottimo strumento integrato per calcolare la dimensione del campione e la potenza statistica per vari tipi di test, inclusi quelli per campioni appaiati. Per maggiori informazioni, visita [https://www.ibm.com/support/knowledgecenter/SSLVMB_23.0.0/spss/base/idh_sampower.html](https://www.ibm.com/support/knowledgecenter/SSLVMB_23.0.0/spss/base/idh_sampower.html).x

### 6. Considerazioni Etiche

- Assicuratevi che i partecipanti diano il consenso informato e che comprendano la natura dell'esperimento.
- Garantite la privacy e l'anonimato dei dati dei partecipanti.

### 7. Report dei Risultati

- Documentate tutte le fasi dell'esperimento, inclusi i metodi, i risultati, e le analisi.

- Provide details about

  - how you recruited participants
  - who are the participants
  - the participants’ demographics (age, gender, etc.)

- Then present the results and reflections on them

- When presenting your finding, only make claims that
  your data can support

- Graphical representations are very important for a
  presentation

- There are various techniques to visualize quantitative
  data.

### 8. Limitazioni e Considerazioni Future

- Riconoscete le limitazioni del vostro esperimento.
- Suggerite direzioni per future ricerche basate sui risultati ottenuti.
