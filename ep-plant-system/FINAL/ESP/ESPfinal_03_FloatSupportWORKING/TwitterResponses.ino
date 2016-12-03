/*
#define FTL 120
#define RTL 120
#define RTM 120


void dataResponse(int t, int h, int l, int m, int pl, int pm ) {
  //##### Temperature response ####
  if ((l - pl) > RTL) { //+++ sudden increase in  luminosity | RTL - rising triger luminosity it defines the change in luminosity needed to trigger this statement |defined as RTL > d(l)/dt
    Serial.println("\t Shine bright!");
  }
  else if ((pl - l) > FTL) { //--- sudden drop in  luminosity | FTL - falling triger luminosity it defines the drop in luminosity needed to trigger this statement |defined as FTL > d(l)/dt
    Serial.println("\t No Light!");
  }
  else if ((m - pm) > RTM) { //sudden positive change in moisture | RTM - rising triger moisture it defines the change in moisture needed to trigger this statement |defined as RTM > d(m)/dt
    Serial.println("\t Drowning!");
  }
  else { // static string responses that are based based on arbitrary data input
    int selector = (int)random(0, 5); //# of cases -1
    selector = 0;  // for temporary testing purposes comment out later
    switch (selector) {
      case 0: //temperature case
        if ((t < 25) && (t > 14)) { // normal temperature
          const static char *tempNormal[5][53]; //421 is the maximum value after url encoding
          selector = (int)random(0, 5);
          *tempNormal[0] = "Enjoying%20the%20perfect%20temperature!";
          *tempNormal[1] = "Chillaxing%20at%20the%20best%20temperature!";
          *tempNormal[2] = "Not%20to%20warm%20and%20not%20to%20cold%20-%20EPIC!";
          *tempNormal[3] = "Currently%20at%20thermodynamic%20equilibrium.";
          *tempNormal[4] = "Two%20words%3A%20%22Optimale%20Temperatur%22";
          tweet(server, tsPort, twKey, *tempNormal[selector]); //upper boundry at random is excluded!
        }
        else if (t > 24) { // to hot
          const static char *tempHot[3][78];
          selector = (int)random(0, 3);
          *tempHot[0] = "And+you+still+need+more+evidence+on+global+warming%3F+Im+sweating+over+here!";
          *tempHot[1] = "Status+update%3A+my+room+just+enabled+de_inferno+mode";
          *tempHot[2] = "Sweating%20like%20a%20mammal.%20Ewww";
          tweet(server, tsPort, twKey, *tempHot[selector]);
        }
        else { // to cold
          const static char *tempCold[2][31];
          selector = (int)random(0, 2);
          *tempCold[0] = "Im+cold%2C+help+me+out!";
          *tempCold[1] = "Feeling+chilly.(pun+intended)";
          tweet(server, tsPort, twKey, *tempCold[selector]);
        }
        break; //case0
      //######### HUMIDITY #########
      case 1: //Humidity case
        if ((h < 61) && (h > 49)) { // OK
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }
        else if (h > 60) { // very humidity!
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }

        else { // dry
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }
        break; //case1
      //######### LUMINOSITY #########
      case 2: // Luminosity case
        if ((l < 520) && (l > 200)) { // OK
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }
        else if (l > 520) { // to luminous
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }

        else { // to dark
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }
        break; //case2

      //######### MOISTURE #########
      case 3: // moisture case
        if ((m < 520) && (m > 200)) { // OK
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }
        else if (m > 520) { // to moist
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }

        else { // to dry
          static char *temp[3][70];
          selector = (int)random(0, 3);
          *temp[0] = "";
          *temp[1] = "";
          *temp[2] = "";
          tweet(server, tsPort, twKey, *temp[selector]);
        }
        break; //case3


      case 4: // random facts
        break; //case4

      case 5: // dayTimeEvents!
        break; //case5

    }//switch
  }//else
}//dataResponse

*/
