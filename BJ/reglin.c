#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
  int ret;
  char nomEntrada[31];
  FILE *entrada;
  double averageX,averageY,sumx=0,sumy=0,sumx2=0,sumxix=0,sumxix2=0,sumyiy2=0,sumyiy=0,sumxy=0,sumxixyiy=0,disc=0,N=0,auxX,auxY,m,n,r;
  
  printf("Introdueix el nom del fitxer d'entrada\n");
  scanf("%s",nomEntrada);
  entrada=fopen(nomEntrada,"r");
  while((ret=fscanf(entrada,"%lf\t\t%lf\n",&auxX,&auxY))!=EOF) {
    sumx+=auxX;   
    sumy+=auxY;
    sumx2+=auxX*auxX;
    sumxy+=auxX*auxY;
    N++;
}
    averageX=sumx/N;
    averageY=sumy/N;
    sumxix=sumx-N*averageX;
    sumyiy=sumy-N*averageY;
    
  
    rewind(entrada);

    while((ret=fscanf(entrada,"%lf\t\t%lf\n",&auxX,&auxY))!=EOF) {
    sumxix2+=(auxX-averageX)*(auxX-averageX);
    sumyiy2+=(auxY-averageY)*(auxY-averageY);    
    sumxixyiy+=(auxY-averageY)*(auxX-averageX);
    }
    
    m=(N*sumxy-sumx*sumy)/(N*sumx2-sumx*sumx);
    printf("m=%lf\n",m);
    n=(sumx2*sumy-sumx*sumxy)/(N*sumx2-sumx*sumx);
    printf("n=%lf\n",n);
    r=(sumxixyiy)/(sqrt(sumxix2*sumyiy2));
    printf("r=%lf\n",r);

/*m=(N*sum(XiYi)-sum(xi)*sum(yi))/(disc)*/
  /*n=(sum(xi2)*sum(yi)-sum(xi)*sum(xi*yi))/disc*/
  /*disc=N*sum(xi2)-(sum(xi))2*/
  
  /*R=(sum(xi-x)*(yi-y))/(sqrt(sum(xi-x)2*sum(yi-y)2))*/

  return 0;
}