#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX_SIZE_WORD 100


typedef struct Node {
   char *text;
   struct Node *right;
   struct Node *left;
} Node;

typedef struct Tree {
   Node *root;
   int count;
} Tree;

Node* create_node(const char *text) {
   Node* node = (Node*)malloc(sizeof(Node));

   node->text = strdup(text);
   node->left = node->right = NULL;

   return node;
}

Tree* create_tree() {
   Tree* tree = (Tree*)malloc(sizeof(Tree));

   tree->root = NULL;
   tree->count = 0;

   return tree;
}

int is_empty_tree(Tree* tree) {
   return tree->root == NULL;
}

int is_empty_node(Node* node) {
   return node == NULL;
}

void word_tolower(char* word) {
   for (int i = 0; word[i]; i++)
      word[i] = tolower(word[i]);
}

void remove_accents_from_word(char* word) {
   for (int i = 0; word[i] != '\0'; i++) {
      if (
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA0) ||
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA1) || 
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA2) || 
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA3)
      ){
         word[i] = 'a';
         memmove(&word[i + 1], &word[i + 2], strlen(&word[i + 2]) + 1);
      }
      else if (
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA8) ||
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA9) || 
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xAA)
      ) {
         word[i] = 'e';
         memmove(&word[i + 1], &word[i + 2], strlen(&word[i + 2]) + 1);
      }
      else if (
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xAE) ||
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xAD) || 
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xAC)
      ) {
         word[i] = 'i';
         memmove(&word[i + 1], &word[i + 2], strlen(&word[i + 2]) + 1);
      }
      else if (
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xB4) ||
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xB3) || 
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xB5)
      ) {
         word[i] = 'o';
         memmove(&word[i + 1], &word[i + 2], strlen(&word[i + 2]) + 1);
      }
      else if (
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xBA) ||
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xBB) || 
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xB9)
      ) {
         word[i] = 'u';
         memmove(&word[i + 1], &word[i + 2], strlen(&word[i + 2]) + 1);
      }
      else if (
         (word[i] == (char)0xC3 && word[i + 1] == (char)0xA7)
      ) {
         word[i] = 'c';
         memmove(&word[i + 1], &word[i + 2], strlen(&word[i + 2]) + 1);
      }
   }
}

int has_special_char(char* word) {
   for (int i = 0; word[i] != '\0'; i++) {
      if (!isalnum(word[i]))
         return 1;
   }

   return 0;
}

void add_node(Tree* tree, Node* node) {
   if (is_empty_tree(tree)) {
      tree->root = node;
      tree->count++;

      return;
   }

   Node* current = tree->root;
   while (current != NULL) {
      int cmp = strcmp(node->text, current->text);

      if (cmp < 0) {
         if (current->left == NULL) {
            current->left = node;
            tree->count++;

            return;
         }

         current = current->left;
      }
      else if (cmp > 0) {
         if (current->right == NULL) {
            current->right = node;
            tree->count++;

            return;
         }

         current = current->right;
      }
      else {
         free(node->text);
         free(node);

         return;
      }
   }
}

Node* search_word(Node* node, const char* word) {
   if (node != NULL) {
      int cmp = strcmp(word, node->text);
      
      if (cmp == 0)
         return node;
      else if (cmp < 0)
         return search_word(node->left, word);
      else
         return search_word(node->right, word);
   }

   return NULL;
}

void search_substring(Node* node, const char* substring) {
   if (node != NULL) {
      if (strstr(node->text, substring) != NULL)
         printf("%s\n", node->text);

      search_substring(node->left, substring);
      search_substring(node->right, substring);
   }
}

Node* remove_root_subtree(Node* root) {
   if (root == NULL)
      return NULL;

   Node* new_root = NULL;
   if (root->left == NULL)
      new_root = root->right;
   else if (root->right == NULL)
      new_root = root->left;
   else {
      Node* parent = root;
      Node* successor = root->right;

      while (successor->left != NULL) {
         parent = successor;
         successor = successor->left;
      }

      if (parent != root) {
         parent->left = successor->right;
         successor->right = root->right;
      }

      successor->left = root->left;
      new_root = successor;
   }

   free(root->text);
   free(root);

   return new_root;
}

void remove_element(Tree* tree, const char* word) {
   if (is_empty_tree(tree)) {
      printf("A árvore está vazia.\n");
      return;
   }

   Node* parent = NULL;
   Node* current = tree->root;

   while (current != NULL && strcmp(current->text, word) != 0) {
      parent = current;

      if (strcmp(word, current->text) < 0)
         current = current->left;
      else
         current = current->right;
   }

   if (current == NULL) {
      printf("Palavra não encontrada na árvore.\n");
      return;
   }

   if (parent == NULL)
      tree->root = remove_root_subtree(current);
   else if (parent->left == current)
      parent->left = remove_root_subtree(current);
   else
      parent->right = remove_root_subtree(current);

   printf("Palavra \"%s\" removida com sucesso.\n", word);
   tree->count--;
}

void save_node(FILE* file, Node* node) {
   if (!is_empty_node(node)) {
      save_node(file, node->left);
      fprintf(file, "%s\n", node->text);
      save_node(file, node->right);
   }
}

void save_tree(Tree* tree, const char* filename) {
   remove(filename);

   FILE *file = fopen(filename, "w");
   if (!file) {
      printf("Erro ao salvar o arquivo %s\n", filename);
      return;
   }

   if (!is_empty_tree(tree))
      save_node(file, tree->root);

   fclose(file);
}

void load_words_from_file(Tree* tree, const char* filename) {
   FILE *file = fopen(filename, "r");
   if (!file) {
      printf("Erro ao abrir o arquivo %s\n", filename);
      return;
   }

   char word[MAX_SIZE_WORD];
   while (fscanf(file, "%s", word) != EOF) {
      word_tolower(word);
      remove_accents_from_word(word);

      int valid = 1;
      for (int i = 0; word[i]; i++) {
         if (!isalnum(word[i])) {
            valid = 0;
            break;
         }
      }

      if (valid) {
         Node* node = create_node(word);
         add_node(tree, node);
      }
   }

   fclose(file);
}

int main() {
   char* filename = "input.txt";

   Tree* tree = create_tree();
   load_words_from_file(tree, filename);

   int choice;
   char word[MAX_SIZE_WORD];

   printf("Digite 1 para adicionar nova palavra\n");
   printf("Digite 2 para buscar uma palavra\n");
   printf("Digite 3 para remover uma palavra\n");
   printf("Digite 4 para buscar palavras por substring\n");
   printf("Digite 5 para calcular o tempo gasto na busca\n");
   printf("Digite 6 para calcular a quantidade de palavras armazenadas\n");
   printf("Digite 7 para sair\n");

   do {
      printf("Sua opção: ");
      scanf("%d", &choice);

      switch (choice) {
         case 1:
            printf("Digite a palavra a ser adicionada: ");
            scanf("%s", word);

            word_tolower(word);
            remove_accents_from_word(word);

            if (has_special_char(word)) {
               printf("Palavra inválida.\n");
               break;
            }

            Node* node = create_node(word);
            add_node(tree, node);

            break;

         case 2:
            printf("Digite a palavra a ser buscada: ");
            scanf("%s", word);

            word_tolower(word);
            remove_accents_from_word(word);

            Node* result = search_word(tree->root, word);
            if (result)
               printf("Palavra \"%s\" encontrada!\n", word);
            else
               printf("Palavra \"%s\" não encontrada!\n", word);

            break;

         case 3:
            printf("Digite a palavra a ser removida: ");
            scanf("%s", word);

            word_tolower(word);
            remove_accents_from_word(word);

            remove_element(tree, word);
            break;

         case 4:
            printf("Digite a substring a ser buscada: ");
            scanf("%s", word);

            search_substring(tree->root, word);
            break;

         case 5:
            printf("Digite a palavra a ser buscada para medir o tempo: ");
            scanf("%s", word);

            word_tolower(word);
            remove_accents_from_word(word);

            clock_t start = clock();
            search_word(tree->root, word);
            clock_t end = clock();

            double time = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Tempo de busca: %.6f segundos\n", time);

            break;

         case 6:
            printf("Quantidade de palavras armazenadas: %d\n", tree->count);
            break;

         case 7:
            printf("Saindo...\n");
            save_tree(tree, filename);

            break;

         default:
            printf("Opção inválida.\n");
      }
   } while (choice != 7);

   return 0;
}