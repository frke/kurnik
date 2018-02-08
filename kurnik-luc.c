// Koda za prižiganje in ugašanje luči za umetno podaljševanje dneva v kurniku

// navodilo za raspberry pi:
// instaliraj paket wiringpi z ukazom sudo apt-get install wiringpi
// programsko kodo  kurnik-luc.c prevedi v izvršiljivo datoteko z ukazom
// gcc -Wall -o kurnik-luc kurnik-luc.c -lwiringPi 
// poženeš datotek z ukazom
// run sudo ./kurnik-luc

// Crontab program se izvede vsako minuto v mesecih 11,12,1,2,3 in med urami 5:00-8:59 h in 16:00-20:59
// proženje vsako minuto v  dodaš v crontab root userja in izhod preusmeriš v log datoteko
// * 5-8,16-20 * 11,12,1,2,3 * /home/pi/kurnik/kurnik-luc >> /tmp/kurnik-luc.log
// če log datoteke ni, jo kreiraš z ukazom  touch /tmp/kurnik-luc.log



// Opis delovanja: od 10.11.2017 do  31.3.2018 bom umetno podaljševal svetlobo v kurniku, vsak dan za eno minuto zjutraj in eno minuto zvečer.
// Zaenkrat se izognem sledenju mraka, sončnega vzhoda, zahoda in samo podaljšujem čas svetlobe, glede na uro

// Začetno stanje na dan 10.11.2017
// popravljeno 20.11.2017 - produkcija + 10 minut
// luč je prižgana zjutraj med 6:45 in  8:00 (8:00 je fiksen čas, ko se luč ugasne)
// luč je prižgana med 16:00 uro (fiksen čas) in 17:00 uro.


#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include <stdlib.h>

// funkcija prizgi_luc prižge luč

int prizgi_luc (void) {
  wiringPiSetup () ;
  int luc_je_prizgana;
  luc_je_prizgana = digitalRead (0) ;

  if (luc_je_prizgana == 0 ){
    pinMode (0, OUTPUT) ;
    printf("Prižigam luč v kurniku \n");
    delay (1000) ;
    digitalWrite (0, HIGH) ;
  }
  else {
    printf("Luč je že prižgana, pustim nespremenjeno\n");
  }

  return 0 ;
}

// funkcija usasni_luc ugasne luč

int ugasni_luc (void){
  wiringPiSetup () ;
  int luc_je_prizgana;
  luc_je_prizgana = digitalRead (0) ;

  if (luc_je_prizgana > 0 ){
    pinMode (0, OUTPUT) ;
    printf("Ugašam luč v kurniku\n");
    delay (1000) ;
    digitalWrite (0,  LOW) ;
  }
  else {
    printf("Luč je že ugasnjena, pustim ugasnjeno\n");
  }

  return 0 ;
}


// Začetek glavnega programa

int main(int argc, char *argv[] ) {


// učenje parametrov
 if( argc == 2 ) {
      printf("The argument supplied is %s\n", argv[1]);
   }
   else if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
   }
   else {
      printf("One argument expected.\n");
   }
   char *arg1 = NULL;

   arg1 = argv[1];

   printf("parameter je %s\n", arg1 );

// konec učenje parametrov



   time_t curtime;
   struct tm *loc_time;

   // trenutni sistemski čas
   curtime = time (NULL);

   // pretvorba sistemske časa v lokalni čas
   loc_time = localtime (&curtime);

   // prikaz časa
//   printf("sistemski čas %s", asctime (&curtime));
   printf("lokalni čas %s", asctime (loc_time));

   int hour = loc_time->tm_hour; // trenutna ura
   int min = loc_time->tm_min;   // trenutna minuta
   int minut = ( hour * 60 ) + min; //koliko minut je poteklo od polnoči
   int danvletu = loc_time->tm_yday; //zaporedni dan v letu

   printf("Danes je %d dan v letu\n", danvletu);
   printf("poteklo je %d minut od polnoči\n", minut);

   int dan_i = danvletu - 323; //koliko dni je minilo od 20.11.2017, ko je bil 323 dan v letu
   if (dan_i < 0 ) {  // Novo leto 2018
	dan_i =  365 - 313 + danvletu;
dan_i=54;
// fiksiram na 3 januar
   }

   int zjutraj_prizgi = ( 6 * 60 + 45 ) - dan_i; // 6:45 je 405 minut od polnoči (6 * 60 + 45)
   int zjutraj_ugasni = 7 * 60 -2 ; // ob 6:58h ugasne luč - le do 8:00 teče minutni cron
   int zvecer_prizgi= 17 * 60 ; // ob 17h se prižge
   int zvecer_ugasni= (17 * 60 ) + dan_i; //ob 17:00 je 1020 minut od polnoči


   printf("zjutraj_prizgi %d minut od polnoci, ob %02d:%02d uri\n", zjutraj_prizgi, zjutraj_prizgi / 60, zjutraj_prizgi % 60);
   printf("zjutraj_ugasni %d minut od polnoci, ob %02d:%02d uri\n", zjutraj_ugasni, zjutraj_ugasni / 60, zjutraj_ugasni % 60);
   printf("zvecer_prizgi %d minut od polnoci, ob %02d:%02d uri\n", zvecer_prizgi, zvecer_prizgi / 60, zvecer_prizgi % 60);
   printf("zvecer_ugasni %d minut od polnoci, ob %02d:%02d uri\n", zvecer_ugasni, zvecer_ugasni / 60, zvecer_ugasni % 60);


//to je za test

/*
int number;
int i;
	for ( i = 1; i < 5 ; i++ ){
//	for (; ; ){
		prizgi_luc();
number =1000;

//	number = rand() % 250;
	printf("rand je %d\n", number);

	delay(number / 2);
	ugasni_luc();
	delay(number);


	}
*/


   /* jutranje podaljševanje dneva */
   if (minut > zjutraj_prizgi && minut <= zjutraj_ugasni){
	prizgi_luc();
   }
   else {
	/* večerno podaljševanje dneva */
	if (minut > zvecer_prizgi && minut <= zvecer_ugasni){
		prizgi_luc();
	}
	else {
		ugasni_luc();
	}
   }


   printf("zjutraj je luč gorela %d minut\n", zjutraj_ugasni - zjutraj_prizgi);
   printf("zvečer je luč gorela %d minut\n", zvecer_ugasni - zvecer_prizgi);
   printf("------------------------------------\n");

   return 0;
}
