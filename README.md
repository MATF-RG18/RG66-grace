# RG66-grace
GRace

Igrica je tipa "infinite runner", igrac se krece po sinusoidi u okruzenju nalik pustinji i izbegava ili ide na jednu od sledecih prepreka :

    Kamen : zaustavlja i tu se igra zavrsava
    Rupa : Kroz nju se propada i igra se takodje zavrsava
    Spirala : pri koliziji sa ovom preprekom igrac biva odbacen u vis i zatim se na stazu, ovo utice na povecanje skora
    Teleport : Baca igraca na neku od random koordinata, takodje kada udari u teleport postaje imun na kratko tako da moze da prolazi kroz prepreke
    Sarzer : Igrac takodje moze da skuplja sarzere u kojima ima po 5 metaka, kojima moze da unistava kamenje na stazi
    
Sve vreme se odbrojava skor, tako da je kljuc osvojiti sto vise poena pre nego sto se igra zavrsi, pored skora igrac uvek ima uvid u to koliko metaka mu je preostalo

Igra se igra iz prvog lica gde se stice utisak da je igrac na motoru i vozi kroz pustinju.

Kontorle :

    A, D za skretanje i Space za pucanje
