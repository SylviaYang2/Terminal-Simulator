#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int decimalToBinary(int decimalnum) {
    int binarynum = 0;
    int i = 1;
    while (decimalnum != 0) {
        binarynum += (decimalnum % 2) * i;
        printf("binarynum: %d\n", binarynum);
        decimalnum /= 2;
        printf("decimalnum: %d\n",decimalnum);
        i *= 10;
        printf("i: %d\n", i);
    }
    return binarynum;
}

void reverse_lines(char* input) {
  FILE* file = fopen(input, "r");
  char word[200];
  while (fscanf(file, "%[^\n]\n", word) != EOF) {
    for (int i = strlen(word) - 1; i >= 0; i--) {
      printf("%c", word[i]);
    }
    printf("\n");
  }
}

bool is_reverse(char* s1, char* s2) {
  if (strlen(s1) == 0 && strlen(s2) == 0) {
    return true;
  } else if (strlen(s1) == 0 || strlen(s2) == 0) {
      return false; // not same length
  } else {
      char s1first = tolower(s1[0]);
      char s2last = tolower(s2[strlen(s2)-1]);
      printf("s1first: %c\n", s1first);
      printf("s2last: %c\n", s2last);
      char* s1new = s1;
      printf("s1new: %s", s1new);
      s1new++;
      printf("s1new: %s", s1new);
      s2[strlen(s2)-1] = '\0';
        // s2--;
      return s1first == s2last && is_reverse(s1new, s2);
  }
}

// int analyze_paragraphs(char* input) {
//   FILE* file = fopen(input, "r");
//   int max = 0;
//   char line[200];
//   int count = 0;

//   while (fscanf(file, "%[^\n]\n", line) != EOF) {
//     while (strcmp("<p>", line) != 0) {
//       count++;
//       printf("%d\n", count);
//       fscanf(file, "%s", line);
//     }
//     printf("%d-line paragraph", count);
//     if (count > max) {
//       max = count;
//     }
//     count = 0;
//   }
//   return max;
// }

int analyze_paragraphs(char* input) {
  FILE* file = fopen(input, "r");
  int max = 0;
  char line[200];
  int count = 0;

  while (fscanf(file, "%[^\n]\n", line) != EOF) {
    if (strcmp("<p>", line) == 0) {
      printf("%d-line paragraph\n", count);
      if (count > max) {
        max = count;
        count = 0;
      }
    } else {
      printf("else");
      count++;
    }
  }
  return max;
}

char* acronym(char* s) {
  char* acr = (char*)malloc(20 * sizeof(char));
  bool letter = true;
  for (int i = 0; i < strlen(s); i++) {
    if (strcmp(" ", s[i]) == 0 || strcmp("-", s[i]) == 0) {
      // letter = 0;
    } else if (letter != 0) {
      // letter = 1;
      printf("true: %c\n", s[i]);
      // strcat(acr, toupper(s[i]));
    }
  }
  return acr;
}

int main() {
  printf("%s", acronym(" automatic teller machine "));

    // reverse_lines("text.txt");
    // printf("!!");
    // int num = analyze_paragraphs("text.txt");
    // printf("%d", num);

    // int decimalnum;
    // printf("Enter a Decimal Number: ");
    // scanf("%d", &decimalnum);
    // printf("Equivalent Binary Number is: %d", decimalToBinary(decimalnum));
    // return 0;
}