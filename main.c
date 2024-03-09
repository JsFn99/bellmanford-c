#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure pour représenter une arête du graphe
struct Arete {
   int src, dest, poids;
};

// Structure pour représenter le graphe avec V le nombre de sommets et E le nombre d'arêtes
struct Graphe {
   int V, E;
   struct Arete *aretes;
};

// Fonction pour initialiser le graphe avec V sommets et E arêtes
struct Graphe *creerGraphe(int V, int E) {
   struct Graphe *graphe = (struct Graphe *)malloc(sizeof(struct Graphe));
   graphe->V = V;
   graphe->E = E;
   graphe->aretes = (struct Arete *)malloc(E * sizeof(struct Arete));
   return graphe;
}

// Fonction pour relâcher une arête et mettre à jour les distances si nécessaire tout en gardant la trace
void relax(struct Graphe *graphe, int u, int v, int *dist,int* pred) {
   if (dist[u] != INT_MAX && dist[u] + graphe->aretes[v].poids < dist[graphe->aretes[v].dest]) {
      dist[graphe->aretes[v].dest] = dist[u] + graphe->aretes[v].poids;
      pred[graphe->aretes[v].dest] = u;
   }
}

// Fonction pour exécuter l'algorithme de Bellman-Ford
void bellmanFord(struct Graphe* graphe, int src) {
   int V = graphe->V;
   int E = graphe->E;
   int* dist = (int*)malloc(V * sizeof(int));
   //tableau pour garder la trace du chemin parcouru
   int* predecesseur = (int*)malloc(V * sizeof(int));

   // Initialiser les distances a l'infini
   for (int i = 0; i < V; i++)
      dist[i] = INT_MAX;

   // La distance du sommet source a lui-meme est toujours 0
   dist[src] = 0;
   predecesseur[src]=0;

   // Relacher toutes les aretes V-1 fois
   for (int i = 1; i <= V - 1; i++) {
      for (int j = 0; j < E; j++) {
         relax(graphe, graphe->aretes[j].src, j, dist,predecesseur);
      }
   }

   // Controle de la presence d'une boucle negative
   for (int i = 0; i < E; i++) {
      int u = graphe->aretes[i].src;
      int v = graphe->aretes[i].dest;
      int poids = graphe->aretes[i].poids;
      if (dist[u] != INT_MAX && dist[u] + poids < dist[v]) {
         printf("il existe un cycle absorbant.\n");
         free(dist);
         return;
      }
   }
   // Afficher les distances minimales
   printf("Les distances minimales depuis le sommet source (%d) sont :\n", src);
   for (int i = 0; i < V; i++)
      printf("Vers le sommet %d : Distance = %d\n", i, dist[i]);
   // Afficher les chemins parcourus pour arriver a chaque sommet
   for (int i = 0; i < V; i++){
      int destination = i;
      printf("De S0 vers S%d {\t", i);
      while(destination > 0){
         int weight = dist[destination] - dist[predecesseur[destination]];
         printf("S%d, %d ,",destination, weight);
         destination = predecesseur[destination];
      }
      printf("S0 }\n");
   }   

   free(predecesseur);
   free(dist);
}

int main(int argc, char *argv[]) {
   if (argc < 2) {
      printf("Utilisation : %s <option>\n", argv[0]);
      printf("Options :\n");
      printf("  1. Données dans le code\n");
      printf("  2. L'utilisateur entre le graphe\n");
      printf("  3. Lire le graphe depuis un fichier texte\n");
      return 1;
   }

   int option = atoi(argv[1]);
   struct Graphe *graphe = NULL;
   int source = 0; // Source par défaut

   if (option == 1) {
      // Option 1 : Données dans le code
      int V = 5; // Nombre de sommets
      int E = 7; // Nombre d'arêtes
      graphe = creerGraphe(V, E);

      // Définir les arêtes du graphe avec les poids (graphe de l'exemple de base 1)
      graphe->aretes[0].src = 0; graphe->aretes[0].dest = 2; graphe->aretes[0].poids = 18;
      graphe->aretes[1].src = 0; graphe->aretes[1].dest = 4; graphe->aretes[1].poids = 3;
      graphe->aretes[2].src = 4; graphe->aretes[2].dest = 3; graphe->aretes[2].poids = 2;
      graphe->aretes[3].src = 3; graphe->aretes[3].dest = 1; graphe->aretes[3].poids = 1;
      graphe->aretes[4].src = 1; graphe->aretes[4].dest = 0; graphe->aretes[4].poids = 8;
      graphe->aretes[5].src = 1; graphe->aretes[5].dest = 2; graphe->aretes[5].poids = 4;
      graphe->aretes[6].src = 4; graphe->aretes[6].dest = 1; graphe->aretes[6].poids = 10;
   } // Option 2 : L'utilisateur entre le graphe
   else if (option == 2) {
      printf("Entrez le nombre de sommets : ");
      int V;
      scanf("%d", &V);

      printf("Entrez la matrice d'adjacence (%dx%d) :\n", V, V);
      graphe = creerGraphe(V, V * V);  // Nombre maximum d'arêtes dans un graphe complet

      for (int i = 0; i < V; i++) {
         for (int j = 0; j < V; j++) {
            scanf("%d", &graphe->aretes[i * V + j].poids);
            if (graphe->aretes[i * V + j].poids != 0) {
               graphe->aretes[i * V + j].src = i;
               graphe->aretes[i * V + j].dest = j;
            }
         }
      }

      printf("Entrez le sommet source : ");
      scanf("%d", &source);
   }

   // Option 3 : Lire le graphe depuis un fichier texte
   else if (option == 3) {
      if (argc < 3) {
         printf("Utilisation : %s 3 <nom_fichier>\n", argv[0]);
         return 1;
      }

      FILE *file = fopen(argv[2], "r");
      if (!file) {
         printf("Erreur lors de l'ouverture du fichier : %s\n", argv[2]);
         return 1;
      }

      // Lire le nombre de sommets
      int V;
      fscanf(file, "%d", &V);

      graphe = creerGraphe(V, V * V);  // Nombre maximum d'arêtes dans un graphe complet

      // Lire la matrice d'adjacence avec les poids
      for (int i = 0; i < V; i++) {
         for (int j = 0; j < V; j++) {
            fscanf(file, "%d", &graphe->aretes[i * V + j].poids);
            if (graphe->aretes[i * V + j].poids != 0) {
               graphe->aretes[i * V + j].src = i;
               graphe->aretes[i * V + j].dest = j;
            }
         }
      }

      fclose(file);
   } else {
      printf("Option invalide\n");
      return 1;
   }

   // Appliquer l'algorithme de Bellman-Ford
   if (graphe != NULL) {
      bellmanFord(graphe, source);

      // Libérer la mémoire allouée
      free(graphe->aretes);
      free(graphe);
   }

   return 0;
}
