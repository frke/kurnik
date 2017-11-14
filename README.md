# kurnik
Umetno podaljševanje svetlobe v kurniku z uporabo Raspberry pi

Koda za prižiganje in ugašanje luči za umetno podaljševanje dneva v kurniku

Navodilo za raspberry pi:

Na Raspberry pi teče operacijski sistem Raspbian
instalirani paketi: gcc, wiringpi
instaliraj paket wiringpi z ukazom sudo apt-get install gcc wiringpi

Programsko kodo  kurnik-luc.c prevedi v izvršiljivo datoteko z ukazom
gcc -Wall -o kurnik-luc kurnik-luc.c -lwiringPi
poženeš datotek z ukazom
run sudo ./kurnik-luc

Program poganjaš s crontab pod userjem root
Crontab program se izvede vsako minuto v mesecih 11,12,1,2,3 in med urami 5:00-8:59 h in 16:00-20:59
 proženje vsako minuto v  dodaš v crontab root userja in izhod preusmeriš v log datoteko
* 5-8,16-20 * 11,12,1,2,3 * /home/pi/kurnik/kurnik-luc >> /tmp/kurnik-luc.log
če log datoteke ni, jo kreiraš z ukazom  touch /tmp/kurnik-luc.log



Opis delovanja: od 10.11.2017 do  31.3.2018 bom umetno podaljševal svetlobo v kurniku, vsak dan za eno minuto zjutraj in eno minuto zvečer.
Zaenkrat se izognem sledenju mraka, sončnega vzhoda, zahoda in samo podaljšujem čas svetlobe, glede na uro

Začetno stanje na dan 10.11.2017
luč je prižgana zjutraj med 6:45 in  8:00 (8:00 je fiksen čas, ko se luč ugasne)
luč je prižgana med 16:00 uro (fiksen čas) in 17:00 uro.
