#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int hit(int ***a,int *cuenta){
  int m,i,j,aux,k=0;
  extern int n;
  
  while(k<1) {
  m=rand()%(6);  
  i=rand()%(4);
  j=rand()%(13);
  aux=a[m][i][j];
  if(aux==0) continue;
  *cuenta+=contar(aux);
  /*printf("Carta=%d\n",aux);Aquesta es la carta que surt*/
  a[m][i][j]=0;
  k++;
  n++;
  if(n==312) {
    printf("No quedan cartas\n");
    exit(1);
}
  }
  return aux;
}

int contar(int carta){
  int num;
  if(carta==10||carta==11) {
    num=-1;
  }
  if(carta==2||carta==3||carta==4||carta==5||carta==6) {
    num=1;
  }
  if(carta==7||carta==8||carta==9) {
    num=0;
  }
  return num;
}

int quefem(int dealer, int player,int asjug1, int asjug2,int nans[17][10], int ans[8][10],int sepuede,int split[10][10]) {
  int accio;
  if (sepuede==0) {
    if(asjug1==0&&asjug2==0) {
      if(player<5) accio=1;
      else {
    accio=nans[player-5][dealer-2];
  }
}
  if(asjug1==1||asjug2==1) {
    if(player<13) accio=1;
    else {
    accio=ans[player-13][dealer-2];
  }
}
  if(asjug1==1&&asjug2==1&&sepuede==1) accio=4;
}
if (sepuede==1) {
  accio=split[player/2-2][dealer-2];
}
  /*printf("accio=%d\n",accio);*/
  return accio;
}
  
void escriure(int p,double win, FILE *sortida) {
 fprintf(sortida,"%d\t\t%lf\n",p,win);
}

int comoApostar(int cuenta,int midbet){
  int bet;

  if(cuenta==0) {
    bet=midbet;
  }
  if(cuenta>0) {
    bet=(cuenta)*midbet;
  }
  if (cuenta<0) {
    bet=midbet/((-1)*(cuenta));
  }
 
return bet;
}

double checkGame(int dealer,int jugador,int bet) {
  
  double bote;
   
  if(dealer>21&&jugador<=21) {
	/*printf("Has guanyat!\n");*/
	bote=bet;
  }
  
  if(jugador>21&&dealer<=21) {
	/*printf("T'has passat\n");*/
	bote=-bet;
  }
  
  if(dealer>jugador&&dealer<22&&jugador<22) {
	/*printf("T'han guanyat\n");*/
	bote=-bet;
  }
  
  if(dealer<jugador&&jugador<=21) {
	/*printf("Has guanyat\n");*/
	bote=bet;

  }
  
  if(dealer==jugador&&jugador<=21) {
	/*printf("Empat\n");*/
	bote=0;
  }
  
  if(dealer>21&&jugador>21) {
	/*printf("T'has passat\n");*/
	bote=-bet;
  }
  return bote;
}