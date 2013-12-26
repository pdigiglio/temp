# Descrizione


* ***free_part.cpp***:
Simulazione numerica di un sistema di _N_ particelle in un volume cubico di lato _L_ finito con condizioni di periodicità al bordo.
Le particelle sono sfere di raggio _r<sub>0</sub>_ interagenti col potenziale di _sfera dura_, che vale _0_ per distanze maggiori di _r<sub>0</sub>_ e infinito per distanze minori di _r<sub>0</sub>_.

* ***lennard_jones.cpp***:
Sistema simile al precedente ma le particelle interagiscono col potenziale di _Lennard-Jones_:

![LJ](http://www.sciweavers.org/tex2img.php?eq=U%28r%29%20%3D%20k%5Cepsilon%20%5Cbigg%5B%0A%5Cbigg%28%5Cfrac%7B%5Csigma%7D%7Br%7D%5Cbigg%29%5E%7Bm%7D%0A%20-%20%5Cbigg%28%5Cfrac%7B%5Csigma%7D%7Br%7D%5Cbigg%29%5En%0A%5Cbigg%5D%0A%5Cqquad%0A%28k%2Cm%2Cn%29%20%3D%20%284%2C12%2C6%29.&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0)


## Algoritmo

La simulazione viene effettuata riducendo il problema fisico agli urti tra
particelle, poiché tra un urto ed il successivo i corpi si muovono di
moto rettilineo uniforme.

Le velocità dello stato iniziale vengono estratte casualmente in modo che
che il sistema non abbia un moto ordinato (il sistema di riferimento centro
di massa risulta in quiete rispetto a quello "del laboratorio").
