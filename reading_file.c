#include <stdio.h>

#define HEIGHT 600
#define WIDTH  4

int main(void)
{
  FILE *myfile;
  double myvariable;
  int i;
  int j;
  myfile=fopen("data.txt", "r");
  FILE * fptr_1;
  fptr_1 = fopen("canale1.txt", "w");
  FILE * fptr_2;
  fptr_2 = fopen("canale2.txt", "w");
  FILE * fptr_3;
  fptr_3 = fopen("canale3.txt", "w");
  for(i = 0; i < HEIGHT; i++)
  {
    for (j = 0 ; j < WIDTH; j++)
    {
      fscanf(myfile,"%lf",&myvariable);
      printf("%.15f ",myvariable);
      if(j == 0){
        fprintf(fptr_1, "%f ",myvariable);
        fprintf(fptr_2, "%f ",myvariable);
        fprintf(fptr_3, "%f ",myvariable);
      }
      else if(j==1){
        fprintf(fptr_1, "%f ",myvariable);
      }
      else if(j==2){
        fprintf(fptr_2, "%f ",myvariable);
      }
      else if(j==3){
        fprintf(fptr_3, "%f ",myvariable);
      }
    }
    fprintf(fptr_1, "\n");
        fprintf(fptr_2, "\n");
        fprintf(fptr_3, "\n");
  }

  fclose(myfile);
  fclose(fptr_1);
  fclose(fptr_2);
  fclose(fptr_3);
  return 0;
}