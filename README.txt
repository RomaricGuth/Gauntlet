PROJET GAUNTLET
Projet réalisé dans le cadre de l'UE LIFAP4 : Conception et Développement d'Applications à l'Université. L'objectif était de réaliser un jeu en C++ en respectant les pratiques de développement Agile

NOTE OBTENUE : 17/20

Membres du groupe:

GUTH Romaric 11711434
RIMET Thomas 11609188
MARTINET Aurélien 11710954


Pour executer le jeu:
(uniquement sous linux)
Placez-vous dans répertoire du projet :
exemple : cd Bureau/gauntlet
tapez make pour compiler
puis tapez ./bin/test pour lancer l'executable du jeu.

Pour les tests de regression, procedez de la même manière en compilant avec make test 
Les executables se lancent avec ./bin/testReg et ./bin/testManches


Des documents complémentaires se trouvent dans le répertoire doc (diagrammes de gantt et des classes, présentation)


Description (non technique) du projet:

Le fonctionnement du système de Gauntlet date des années 80. 
Il consiste à évoluer dans un jeu global, en réalisant une succession de manches.
Ici, nous avons choisi de créer un labyrinthe dans lequel se déplace le personnage.
Au fil du parcours, des manches se déclenchent. Réussisez toutes les manches pour atteindre
l'arrivée !


Dans l'univers global, déplacez vous avec les flèches directionnelles.
Les manches se déclenchent selon la position du personnage sur le chemin de la victoire.
Au début de chaque manche, un affichage indique le lancement de la manche avec quelques indications, appuyez sur entrée
pour la lancer.


Gagnez toutes les manches et atteignez la ligne d'arrivée pour gagner ! 
Vous disposez de 3 vies. Si vous échouez une manche, vous perdez l'une d'elle, et la manche se relance. 
Si vous perdez toutes vos vies, la partie s'arrête et vous aurez perdu. 
Si vous ne réalisez pas l'objectif d'une manche, vous perdez une vie. 
Si vous ne respectez pas le temps imparti pour la manche, vous perdez une vie. 


Les différentes Manches :


Manche 1 :
    épreuve de calcul mental. 4 calculs générés aléatoirement se succèdent: une addition, une soustraction
    et une multiplication et l'addition de ces 3 premiers resultats. 
    Veillez donc à mémoriser vos 3 résultats pour le dernier calcul. Si vous vous trompez,
    vous perdez une vie. Si vous mettez trop de temps à répondre, vous perdrez également une vie.


Manche 2 :
    épreuve de rapidité. Des touches "lettre" du clavier sont choisis aléatoirement et affichées à l'écran .
    Vous devez appuyez sur la touche demandée. Vous devez en taper 15 correctement dans le temps imparti. 
    Si vous vous trompez, ou que vous mettez trop de temps pour tapez les 15 touches, vous perdrez la manche.

Manche 3 :
    épreuve de mémoire. Le terrain s'affiche durant 5 secondes avec une case rouge positionnée aléatoirement,
    qui est l'objectif à atteindre.Passé les 5 secondes, le terrain devient noir, mais les blocs sont toujours
    présents, vous empêchant de vous déplacer. Vous devez rejoindre l'objectif avant la fin du temps imparti.

Manche 4 :
    épreuve de mémoire. La carte est découpée en 4 étages, 3 étages comportant 3 portes et un étage avec une seule.
    Une seule porte amène à l'étage inférieur, vous devez atteindre la porte au RDC. Bien sur, les mauvaises portes
    vous téléportent également et peuvent vous ramener tout en haut !
    Deplacez vous avec les flèches lattérales et appuyez sur la flèche du haut pour prendre une porte.

Manche 5 :
    épreuve de survie. Vous êtes enfermé dans une croix au centre de la map. Vous devez évitez les balles
    tirées par les canons disposés sur les côtés. /!\ Attention, les commandes de déplacement sont inversées !

Manche 6 :
    Combat final. Vous arrivez sur une map avec 5 téleporteurs. Les téléporteurs communiquent entre eux.
    Leur redirection est générée aléatoirement au début de la manche. Un seul téléporteur redirige vers la 
    plateforme centrale, comportant le boss à battre. Lorsque vous reprenez le téléporteur central, 
    changent !

    Attention, votre personnage fera les actions suivantes dans la direction de votre dernier déplacement !

    Pour battre le boss, vous disposez de deux types d'attaques :

    un coup d'épée, puissant, tappant uniquement au corps à corps, avec la touche A.

    Deuxième type d'attaque, une fleur de puissance est positionnée aléatoirement sur la carte toutes les 10 secondes,
    reste affichée 5 secondes, puis disparaît. Lorque que vous marchez sur une fleur, mario rentre dans l'état
    feu pendant 6 secondes, vous permettant de tirer des boules de feu, qui ont une portée maximale de 6 cases.
    Pour tirer, appuyez sur la touche E.

   
    Bien sûr, Bowser ne se laissera pas faire :

    des balles sont tirées depuis les côtés selon la position du personnage, vous infligeants des dégâts si 
    elles vous touchent. Pour vous en protéger, vous pouvez soit les éviter, soit construire un mur. 
    Pour se faire, appuyer sur la touche Z.

    Si vous touchez le boss, il vous ecrasera et vous infligera de lourds dégats. Nous vous laissons quelques
    secondes d'invincibilité lorsque vous perdez de la vie, alors dégagez vous vite !

    Bowser effectue également des sauts qui le rendent intouchable, avant de retomber en lançant une terrible onde de choc.
    En phase 2, il pourra même quitter son île et vous sauter directement dessus ou que vous soyez.

    Evidemment cette fois vos simple murs seront innefficaces face à la puissance de Bowser, alors... 
    COURREZ ! Et pensez à utiliser les téléporteurs que nous avons si gentiment mis à votre disposition !

    Vous disposez d'un nombre de point de vie dans cette manche. Si ce nombre tombe à 0, vous perdrez une vie globale.
    Pour gagner, faites tomber à 0 les points de vie du boss !
