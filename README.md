# Descrizione

Simulazione numerica di un sistema di _N_ particelle in un volume cubico di lato _L_ finito con condizioni di periodicità al bordo.
Le particelle sono sfere di raggio _r_ interagenti col potenziale di _sfera dura_, che vale _0_ per distanze maggiori di _r_ e infinito per distanze minori di _r_.

## Algoritmo

La simulazione viene effettuata riducendo il problema fisico agli urti tra
particelle, poiché tra un urto ed il successivo i corpi si muovono di
moto rettilineo uniforme.

Le velocità dello stato iniziale vengono estratte casualmente in modo che
che il sistema non abbia un moto ordinato (il sistema di riferimento centro
di massa risulta in quiete rispetto a quello "del laboratorio").
