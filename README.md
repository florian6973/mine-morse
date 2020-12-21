# mine-morse

Projet de codage et décodage de MORSE dans le cadre de l'UE12.

## Compilation 

Utilisation des outils :

- Ubuntu 20.04 (WSL 2)
- g++ 9.3.0
- GNU Make 4.2.1

Utiliser `make` dans le répertoire principal. Le programme console est alors `bin/mine-morse_c`.

## Documentation

Le programme console affiche par défaut l'aide, qui indique comment l'utiliser. Le fichier de configuration est, si besoin, `data/config.mm`.

## Tests

Pour cela, il suffit de faire `make test`. Les fichiers nécessaires sont dans le dossier `test`.

## Compatibilité

Le programme décode également les fichiers audios générés par les sites suivants :

- https://www.meridianoutpost.com/resources/etools/calculators/calculator-morse-code.php
- https://www.radio-amater.rs/morsecodegenerator/
- https://morsecode.world/international/translator.html

Le fichier audio produit peut être décodé par le site suivant (avec la configuration par défaut) :

- https://morsecode.world/international/decoder/audio-decoder-adaptive.html
