#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcions.h"
int n=0;
int main(void) {

  int N,cuenta,player=0,dealer=0,bet,auxsplit1,auxsplit2,ret,jugada,trampa2=0;
  int i,j,k=2,m,aux,p=0,midbet,bet1,bet2,trampa=0,asjug1=0,asjug2=0,sepuede=0;
  int ***mazo,noasesnosplit[17][10],asesnosplit[8][10],split[10][10];
  int accio;
  double bote;
  FILE *nans,*ans,*separar,*sortida;
  char nomnans[20]="noasesnosplit",nomans[20]="asesnosplit";


  srand(time(NULL));

/*Es llegeix la matriu de no asos i no split i es crea la matriu corresponent*/

  nans=fopen(nomnans,"r");

  if(nans==NULL){
    puts("No trobo l'arxiu\n");
    exit(1);
  }

  for(i=0;i<17;i++){
    for(j=0;j<10;j++) {
      ret=fscanf(nans,"%d",&jugada);
      noasesnosplit[i][j]=jugada;  
    }
  }

fclose(nans);

/*Es crea i llegeix la matriu de asos pero sense split*/

  ans=fopen(nomans,"r");

  for(i=0;i<8;i++){
    for(j=0;j<10;j++) {
    ret=fscanf(ans,"%d",&jugada);
    asesnosplit[i][j]=jugada;
    }
  }

  fclose(ans);

/*Es crea i llegeix la matriu amb split (si son asos se separa sempre)*/

  separar=fopen("splitter","r");

  for(i=0;i<10;i++){
    for(j=0;j<10;j++) {
      ret=fscanf(separar,"%d",&split[i][j]);
    }
  }
  fclose(separar);

/*Aquestes matrius es fan servir sempre, ara pillem les CI de la partida*/

printf("Con cuanto dinero empezamos?\n");
scanf("%lf",&bote);

printf("Cuantas partidas quieres hacer?\n");
scanf("%d",&N);

printf("Apuesta media:\n");
scanf("%d",&midbet);

puts("\n");

/*Obrim l'arxiu que farem servir per escriure els resultats*/

sortida=fopen("grafica","w");

/*Let the game begin*/

while(p<N) {

  if(p%50==0) {
    n=0;
    /*Es crea el mazo cada 25 partides, ha de ser dinàmic perque es modifica*/

    mazo=(int ***) malloc(6*sizeof(int **));
    for(i=0;i<6;i++) {
      mazo[i]=(int **) malloc(4*sizeof(int *));
    }
    for(i=0;i<6;i++) {
      for(j=0;j<4;j++) {
        mazo[i][j]=(int *) malloc(13*sizeof(int));
      }
    }

    /*Ara s'assignen les cartes*/

    for(m=0;m<6;m++) {
      for(i=0;i<4;i++) {
        for(j=1;j<13;j++) {
          mazo[m][i][j]=k;
          if(k==10) continue;
          k++;
        }
      k=2;
      }
    }

  for(m=0;m<6;m++) {
    for(i=0;i<4;i++) {
      mazo[m][i][0]=11;
    }
  }
  
 /*Ara es prepara el compte, que cada 25 partides s'inicialitza a 0*/

  cuenta=0;
  }
 
 
  bet=comoApostar(cuenta,midbet);
  /*printf("BOTE=%lf\n",bote);*/

 /*printf("Et recomano que apostis %d perque el compte es %d\n",bet,cuenta);*/

  /*Si hi ha previsio de quedarse en negatiu, abortem el programa*/

  /*if(bote-bet<0) {
    printf("Et quedaras sense diners en la jugada %d\n",p);
    exit(1);
  }*/

  /*Dealer pilla carta*/

  dealer=hit(mazo,&cuenta);

  /*Player pilla carta i comprova si es as*/

  player=hit(mazo,&cuenta);

  if(player==11){
    asjug1=1;
    /*printf("Jug1 te as\n");*/
  }

  auxsplit1=player;

  /*Player pilla 2a carta*/

  player+=hit(mazo,&cuenta);

  auxsplit2=player-auxsplit1;

  /*Es comprova si es pot separar*/

  if(auxsplit1==auxsplit2) sepuede=1;

  /*Es comprova si la 2a carta es un as*/

  if(auxsplit2==11) {
    asjug2=1;
   /* printf("Jug2 te as\n");*/
  }

  /*printf("dealer=%d\tplayer=%d+%d=%d\ncuenta=%d\n",dealer,auxsplit1,auxsplit2,player,cuenta);*/

  /*Es treu el cas BJ*/

  if(player==21) {

    /*printf("El jugador te blackjack!\n");*/
    
    dealer+=hit(mazo,&cuenta);

    if(dealer==21) {
      /*printf("El dealer te blackjack!\nEmpat\n");*/
      asjug1=0;
      asjug2=0;
      sepuede=0;
      if(n>260) {
      escriure(p,bote,sortida);
      }
      p++;
      continue;
    }

    else {
    bote+=1.5*bet;
    asjug1=0;
    asjug2=0;
    sepuede=0;
    if(n>260) {
    escriure(p,bote,sortida);
    }
    p++;
    continue;
    }
    
  }

  /*Si player no te BJ, es passa a fer una partida normal*/

  while((dealer<21&&player<21)||(player==22&&dealer<21&&asjug1==1&&asjug2==1)) {
    
    if(asjug1==1&&asjug2==1) {
      accio=4;
    }

    else {
      accio=quefem(dealer,player,asjug1,asjug2,noasesnosplit,asesnosplit,sepuede,split);
    }
 
    /*HIT*/
    if (accio==1) {
	   aux=player;
	   player+=hit(mazo,&cuenta);
	   sepuede=0;
	   if(((player-aux)==11)&&auxsplit2==0) asjug2=1;
	   if(((player-aux)==11)&&auxsplit2==1) asjug1=1;
	   if(player>21&&(asjug1==1||asjug2==1)) {
	        player-=10;
		if(asjug2==1) asjug2=0;
		if(asjug1==1) asjug1=0;
	   }
	   /*printf("dealer=%d\tplayer=%d\n",dealer,player);*/
    }
    
    /*STAND*/
    if (accio==2) {
	   /*printf("Et pares amb: dealer=%d\tplayer=%d\n",dealer,player);*/
	   break;
    }
    
    /*DOUBLE*/ 
    if (accio==3) {
	   /*printf("Dobles, la teva aposta passa a ser de %d\n",2*bet);*/
	   aux=player;
	   player+=hit(mazo,&cuenta);
	   if (player>21&&player-aux==11) {
	      player-=10;
	   }
	   /*printf("Et pares amb: dealer=%d\tplayer=%d\n",dealer,player);*/
	   bet+=bet;
	   break;
      }
      
    /*SPLIT*/
    if (accio==4) {
      
      /*Es comprova que es pot fer l'split*/
      
	if(sepuede==1) {
	  /*Trampa 4 es fa servir per valorar les dues jugades al final per separat*/
	  trampa=4;
	  /*printf("split1=%d\nsplit2=%d\n\nSeparem en aposta1 i aposta2\n",auxsplit1,auxsplit2);*/
	  
	  /*Aposta 1*/	  
	  
	  player=auxsplit1;
	  while(dealer<21 && player<21) {
	    
	    /*Primera accio es sempre HIT*/
	    
	    if(trampa2==0) {
	      accio=1;
	      /*Mirem si auxsplit1 es as o no*/
	      if(auxsplit1==11) {
		  asjug1=1;
	      }
	    else {
		  asjug1=0;
	      }
	    }
	    
	    /*Despres es valora*/
	    
	    else {
	      /*printf("asjug1=%d\nasjug2=%d\nsepuede=%d\n",asjug1,asjug2,sepuede);*/
	      accio=quefem(dealer,player,asjug1,asjug2,noasesnosplit,asesnosplit,0,split);
	    }
	    /*
	    if(accio!=1&&accio!=2&&accio!=3) {
	    puts("THE PILLAT1\n");
	    exit(1);
	    }
	    */
	    /*HIT*/
	    if (accio==1) {
	      
	      player+=hit(mazo,&cuenta);
	      trampa2=1;
	      if(player-auxsplit1==11) asjug2=1;
	      if(player>21&&player-auxsplit1==11) {
		      player-=10;
		      asjug1=0;
	      }
	      /*printf("dealer=%d\tsplit1=%d\n",dealer,player);*/
	      bet1=bet;
	      auxsplit1=player;
	   }
	   
	   /*STAND*/
	    if (accio==2) {
	      /*printf("Et pares amb: dealer=%d\tsplit1=%d\n",dealer,player);*/
	      auxsplit1=player;
	      bet1=bet;
	      break;
	    }
	   
	   /*DOUBLE*/
	    if (accio==3) {
	      /*printf("Dobles, la teva aposta passa a ser de %d\n",2*bet);*/
	      aux=player;
	      player+=hit(mazo,&cuenta);
	      if (player>21&&player-aux==11) {
		    player-=10;
	      }
	      /*printf("Et pares amb: dealer=%d\tsplit1=%d\n",dealer,player);*/
	      bet1=2*bet;
	      auxsplit1=player;
	      break;
	   }
	}
	
	  /*Aposta 2*/
	  player=auxsplit2;
	  asjug1=0;
	  asjug2=0;
	  trampa2=0;
	  
	  while(dealer<21&&player<21) {
	    
	    if(trampa2==0) {
	      accio=1;
	      if(auxsplit2==11) {
		asjug1=1;
	      }
	      else {
		asjug1=0;
	      }
	    }
	    
	    else {
	     /* printf("asjug1=%d\nasjug2=%d\nsepuede=%d\n",asjug1,asjug2,sepuede);*/
	      accio=quefem(dealer,player,asjug1,asjug2,noasesnosplit,asesnosplit,0,split);
	    }
    /*if(accio!=1&&accio!=2&&accio!=3) {
      puts("THE PILLAT2\n");
      exit(1);
    }*/
	    /*HIT*/
	    if (accio==1) {
	      player+=hit(mazo,&cuenta);
	      trampa2=1;
		if(player-auxsplit2==11) asjug2=1;
		if (player>21&&player-auxsplit2==11) {
		  player-=10;
		  asjug2=0;
		}
	      /*printf("dealer=%d\tsplit2=%d\n",dealer,player);*/
	      bet2=bet;
	      auxsplit2=player;
	    }
	    
	    /*STAND*/
	    if (accio==2) {
	      auxsplit2=player;
	      /*printf("Et pares amb: dealer=%d\tsplit2=%d\n",dealer,player);*/
	      bet2=bet;
	      break;
	    }
	    
	    /*DOUBLE*/
	    if (accio==3) {
	      /*printf("Dobles, la teva aposta passa a ser de %d\n",2*bet);*/
	      aux=player;
	      player+=hit(mazo,&cuenta);
	      if (player>21&&player-aux==11) {
		    player-=10;
	      }
	      /*printf("Et pares amb: dealer=%d\tsplit2=%d\n",dealer,player);*/
	      bet2=2*bet;
	      auxsplit2=player;
	      break;
	   }
	 }
	  break;
	}
	
      	else {
	  printf("Les dues cartes no son iguals\n");
	  continue;
	}
      }
	
  }
  
  /*Ara juga el dealer*/
  
  dealer+=hit(mazo,&cuenta);
  while(dealer<17) {
    aux=dealer;
    dealer+=hit(mazo,&cuenta);
    if (dealer>21&&dealer-aux==11) {
	dealer-=10;
    }
  }
  asjug1=0;
  asjug2=0;
  sepuede=0;
  trampa2=0;
  if(trampa==4) {
    
    trampa=0;
    /*Aposta 1*/
    
    /*printf("dealer=%d\tauxsplit1=%d\nauxsplit2=%d\n",dealer,auxsplit1,auxsplit2);  */

    if(n>260) bote+=checkGame(dealer,auxsplit1,bet1);
    
     /*Aposta2*/
    
     if (n>260) {
     bote+=checkGame(dealer,auxsplit2,bet2);
     escriure(p,bote,sortida);
     }
  
     p++;
    
  }
  
  else {
  
  /*printf("dealer=%d\tplayer=%d\n",dealer,player);  */
  
  if(n>260) {
  bote+=checkGame(dealer,player,bet);
  escriure(p,bote,sortida);
  }
  p++;
  }
  
  
}

for(i=0;i<6;i++) {
  for(j=0;j<4;j++) {
        free(mazo[i][j]);
  }
    free(mazo[i]);
}

free(mazo);
fclose(sortida);
    
return 0;
}


