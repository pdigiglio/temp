# Descrizione

Sorgenti di eseguibili utilizzati per studiare numericamente dei sistemi fisici di varia natura.

## Particelle libere

Il file  ***free_part.cpp*** è simula numericamente un sistema di _N_ particelle in un volume cubico di lato _L_ finito con condizioni di periodicità al bordo.
Le particelle sono sfere di raggio _r<sub>0</sub>_ interagenti col potenziale di _sfera dura_, che vale _0_ per distanze maggiori di _r<sub>0</sub>_ e infinito per distanze minori di _r<sub>0</sub>_.

## Algoritmo

La simulazione viene effettuata riducendo il problema fisico agli urti tra
particelle, poiché tra un urto ed il successivo i corpi si muovono di
moto rettilineo uniforme.
Si può dichiarare dunque una matrice dei tempi di collisione _T_ in cui l'elemento _(i, j)_ rappresenti l'intervallo temporale tra il tempo attuale _t_ e il prossimo urto delle particelle _i_-esima e _j_-esima.
La matrice _T_ è simmetrica, pertanto, nel programma si è dichiarata, mediante allocazione dinamica della memoria, una matrice triangolare (inferiore).

### Evoluzione

L'evoluzione avviene iterando i seguenti passaggi:

* Si scorre la matrice dei tempi di collisione _T = { t<sub>ij</sub> }_ e cercando gli indici _i<sub>0</sub>_ e _j<sub>0</sub>_ per cui il tempo di collisione _t* := t<sub>i<sub>0</sub> j<sub>0</sub>_ risulti minimo;
* Si fa evolvere il sistema di un tempo _t*_;
* Si aggiornano le velocità delle particelle _i<sub>0</sub>_-esima e _j<sub>0</sub>_-esima (si scambiano le componenti delle loro velocità lungo l'asse che le congiunce);
* Si riaggiorna gli elementi della matrice _T_ che coinvolgono gli indici _i<sub>0</sub>_ e _j<sub>0</sub>_.

### Inizializzazione

Le velocità dello stato iniziale vengono estratte casualmente.
Si fa in modo che il sistema non abbia un moto ordinato (il sistema di riferimento centro
di massa risulta in quiete rispetto a quello "del laboratorio").
Per quanto riguarda le posizioni, si dispongono le particelle su un reticolo regolare
con configurazione BCC.

## Particelle interagenti

* ***lennard_jones.cpp***:
Sistema simile al precedente ma le particelle interagiscono col potenziale di _Lennard-Jones_:

![LJ](http://www.sciweavers.org/tex2img.php?eq=U%28r%29%20%3D%20k%5Cepsilon%20%5Cbigg%5B%0A%5Cbigg%28%5Cfrac%7B%5Csigma%7D%7Br%7D%5Cbigg%29%5E%7Bm%7D%0A%20-%20%5Cbigg%28%5Cfrac%7B%5Csigma%7D%7Br%7D%5Cbigg%29%5En%0A%5Cbigg%5D%0A%5Cqquad%0A%28k%2Cm%2Cn%29%20%3D%20%284%2C12%2C6%29.&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)



