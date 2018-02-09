// Koda za prižiganje in ugašanje luči za umetno podaljševanje dneva v kurniku

// navodilo za raspberry pi:
// instaliraj paket wiringpi z ukazom sudo apt-get install wiringpi ali dobi kodo na githubu
// programsko kodo  kurnik-luc.c prevedi v izvršiljivo datoteko z ukazom
// gcc -Wall -o kurnik-luc-s kurnik-luc-s.c -lwiringPi
// poženeš datotek z ukazom
// run sudo ./kurnik-luc-s

// Crontab program se izvede vsako minuto v mesecih 11,12,1,2,3 in med urami 5:00-8:59 h in 16:00-20:59
// proženje vsako minuto v  dodaš v crontab root userja in izhod preusmeriš v log datoteko
// * 5-8,16-20 * 11,12,1,2,3 * /home/pi/kurnik/kurnik-luc-s >> /tmp/kurnik-luc-s.log
// če log datoteke ni, jo kreiraš z ukazom  touch /tmp/kurnik-luc-s.log



// Opis delovanja: popraviti


#include <stdio.h>
#include <time.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
 
#ifndef min
#define min(a,b) ( (a) < (b) ? (a) : (b) )
#endif
 
void mid(const char *src, size_t start, size_t length, char *dst, size_t dstlen)
{       size_t len = min( dstlen - 1, length);
 
        strncpy(dst, src + start, len);
        // zero terminate because strncpy() didn't ? 
        if(len < length)
                dst[dstlen-1] = 0;
}

// funkcija prizgi_luc prižge luč

int prizgi_luc (void) {
  wiringPiSetup () ;
int luc_je_prizgana;
luc_je_prizgana = digitalRead (0) ;
if (luc_je_prizgana == 0 ){

  pinMode (0, OUTPUT) ;
  printf("Prižigam luč v kurniku !!!\n");
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
  printf("Ugašam luč v kurniku !!!\n");
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


// preberem iz datoteke soncni vzhod in zahod v array casi
    int ii = 0;
    char casi[365][11];
    FILE *fp = fopen("soncni-vzhod-zahod.txt", "r");

    if (fp == 0) {
        fprintf(stderr, "failed to open soncni-vzhod-zahod.txt\n");
        exit(1);
    }
    while (ii < 365 && fgets(casi[ii], sizeof(casi[0]), fp)) {
        casi[ii][strlen(casi[ii])-1] = '\0';
        ii = ii + 1;
    }
    fclose(fp);
    printf("%d\n", ii);
    printf("%s \n", casi[ii-1]);//zadnji element v arrayu


   time_t curtime;
   struct tm *loc_time;

   // trenutni sistemski čas
   curtime = time (NULL);

   // pretvorba sistemske časa v lokalni čas
   loc_time = localtime (&curtime);

   // prikaz časa
   printf("%s", asctime (loc_time));

   int hour = loc_time->tm_hour; // trenutna ura
   int min = loc_time->tm_min;   // trenutna minuta
   int minut = ( hour * 60 ) + min; //koliko minut je poteklo od polnoči
   int danvletu = loc_time->tm_yday ; //zaporedni dan v letu, 1.januar = 0

   printf("Danes je %d dan v letu\n", danvletu + 1 );
   printf("poteklo je %d minut od polnoči\n", minut);

   printf("soncni vzhod zahod %s\n", casi[danvletu]);
	char soncni_vzhod[5]; 
strncpy(soncni_vzhod, casi[danvletu],5);
 printf("sonce vzide ob %s \n", soncni_vzhod);
	char soncni_zahod[5]; 
mid(casi[danvletu],5,5,soncni_zahod,5);

//strncpy(soncni_zahod, casi[danvletu],7);
 printf("sonce zaide ob %s \n", soncni_zahod);



//	int soncni_vzhod = casi[danvletu];




   int dan_i = danvletu - 313; //koliko dni je minilo od 10.11.2017, ko je bil 313 dan v letu
   if (dan_i < 0 ) {  // Novo leto 2018
	dan_i =  365 - 313 + danvletu;
   printf("dan_i je %d dni\n", dan_i);
   }

	// fiksno ob 6:12 do 18.3.2018
	dan_i=33;
   int zjutraj_prizgi = ( 6 * 60 + 45 ) - dan_i; // 6:45 je 405 minut od polnoči (6 * 60 + 45)

   int zjutraj_ugasni = 8 * 60 ; // ob 8h ugasne luč 
   int zvecer_prizgi= 16 * 60 ; // ob 16h se prižge
	//fiksno ob 18:15
	dan_i=75;
   int zvecer_ugasni= (17 * 60 ) + dan_i; //ob 17:00 je 1020 minut od polnoči




   printf("zjutraj_prizgi %d minut od polnoci, ob %02d:%02d uri\n", zjutraj_prizgi, zjutraj_prizgi / 60, zjutraj_prizgi % 60);
   printf("zjutraj_ugasni %d minut od polnoci, ob %02d:%02d uri\n", zjutraj_ugasni, zjutraj_ugasni / 60, zjutraj_ugasni % 60);
   printf("zvecer_prizgi %d minut od polnoci, ob %02d:%02d uri\n", zvecer_prizgi, zvecer_prizgi / 60, zvecer_prizgi % 60);
   printf("zvecer_ugasni %d minut od polnoci, ob %02d:%02d uri\n", zvecer_ugasni, zvecer_ugasni / 60, zvecer_ugasni % 60);

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
