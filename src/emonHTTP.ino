#include "Console.h"
#include "Process.h"
#include "Bridge.h"
#include "YunServer.h"
#include "YunClient.h"
#include "EmonLib.h"

// Site name
#define siteName "Home"

// Module name
#define moduleName "mainPower"

// HTTP server
YunServer server;

// Energy monitor board
EnergyMonitor emon1;

// Global vars
float realPower;
float apparentPower;
float powerFactor;
float Vrms;
float Irms;

// mesure execution time
unsigned long start;

void setup()
{
  // Bridge startup
   Bridge.begin();

   // Console startup
   Console.begin();

   // HTTP server startup
   server.begin();

   // Energy monitor setup
   emon1.current(1, 60.606);
   emon1.voltage(0, 250, 1.7);
}

void loop()
{
  // Listen for clients
  YunClient client = server.accept();

  // There is a new client ?
  if (client) {
    start = micros();
    // Read values from energy monitor board
    readEnergyMonitor();
    // Process request
    processHTTPRequest(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms
}

void readEnergyMonitor() {

  // Read values from energy monitor board
  emon1.calcVI(20,2000);

  realPower       = emon1.realPower;        //extract Real Power into variable
  apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
  Vrms            = emon1.Vrms;             //extract Vrms into Variable
  Irms            = emon1.Irms;             //extract Irms into Variable

  return;
}

// Return timestamp of Linino
unsigned long timeInEpoch() {
  Process time;                   // process to run on Linino
  char epochCharArray[12] = "";

  // Get UNIX timestamp
  time.begin("date");
  time.addParameter("+%s");
  time.run();
  time.readString().toCharArray(epochCharArray, 12);

  return atol(epochCharArray);
}

void processHTTPRequest(YunClient client) {

  // Send json response
  client.println(F("HTTP/1.1 200 OK"));
  //client.println("Status: 200");
  client.println(F("Content-type: text/json"));
  client.println(); //mandatory blank line

  // build json Response
  client.print("{ ");
    client.print("\"SiteName\": "); client.print("\""); client.print(siteName); client.print("\""); client.print(", ");
    client.print("\"ModuleName\": "); client.print("\""); client.print(moduleName); client.print("\""); client.print(", ");
    client.print("\"Irms\": "); client.print(String(Irms)); client.print(", ");
    client.print("\"Vrms\": "); client.print(String(Vrms)); client.print(", ");
    client.print("\"RealPower\": "); client.print(String(realPower)); client.print(", ");
    client.print("\"ApparentPower\": "); client.print(String(apparentPower)); client.print(", ");
    client.print("\"PowerFactor\": "); client.print(String(powerFactor)); client.print(", ");
    client.print("\"Time\": "); client.print(timeInEpoch()); client.print(", ");
    client.print("\"ExecTime\": "); client.print(String(micros()-start)); client.print(" ");
  client.print("}");

  if (Console) {
    Console.print("Got new HTTP connection : ");
    Console.print("SiteName: "); Console.print(siteName); Console.print(' ');
    Console.print("ModuleName: "); Console.print(moduleName); Console.print(' ');
    Console.print("Vrms: "); Console.print(Vrms); Console.print(' ');
    Console.print("Irms: "); Console.print(Irms); Console.print(' ');
    Console.print("RealPower: "); Console.print(realPower); Console.print(' ');
    Console.print("apparentPower: "); Console.print(apparentPower); Console.print(' ');
    Console.print("PowerFactor: "); Console.print(powerFactor); //Console.print(' ');
    Console.println();
  }
}
