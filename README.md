# Descrizione

Simulazione numerica di un sistema di _N_ particelle in un volume cubico di lato _L_ finito.
Le particelle sono sfere di raggio _r_ che interagiscono col potenziale di _sfera dura_, che vale _0_ per distanze maggiori di _r_ e infinito per distanze minori di _r_.

## Algoritmo

La simulazione viene effettuata riducendo il problema fisico agli urti tra
particelle, ***poiche'*** tra un urto ed il successivo i corpi si muovono di
moto rettilineo uniforme.

Le ___velocita'___ dello stato iniziale vengono estratte casualmente in modo che
che il sistema non abbia un moto ordinato (la il centro di massa risulta in
quiete rispetto a quello "del laboratorio").
