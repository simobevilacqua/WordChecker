# WordChecker

Application developed for the course "Algorithms and Data Structures" of the Polytechnic of Milan.

## Goal
The main goal of the project is to create a system that checks the correspondence between two words of same length by using some optimized algorithms and data structures. The quality of the application is measured using some test cases making sure to complete the execution within predefined time and space limits.

## Description
The application takes in input the words length and the set of words (dictionary) that will be used during the execution.
The user can create new games using the command ```+nuova_partita```.
For each game you need to insert the reference word (word that will need to be guessed) and the maximum number of tries to get the right word. The user can submit the tries to guess the reference word and the system will reply with a report on the matching between the inserted word and the correct one. This information report whether a letter belongs to the reference word and it's in the correct spot, whether it belongs to it but it's in the wrong position or if it doesn't belong to it.
The user can insert other commands during the execution:
- ```+inserisci_inizio``` followed by ```+inserisci_fine```: to insert new words in the dictionary
- ```+stampa_filtrate```: to print the possible words in the dictionary that could be the reference word given the information acquired during previous failed tries.

## Final grade
30/30 cum Laude
