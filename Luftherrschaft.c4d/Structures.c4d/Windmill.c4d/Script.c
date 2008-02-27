/*-- Windrad --*/

#strict

#include L103

private func Initialize()
{
  ObjectSetAction(CreateObject(LWNG,0,0,GetOwner()),"Turn",this);
  //Log("Init Windrad");
  return 1;
}

/* TimerCall */

private func CheckWindwing()
{
  // Komplett, aber kein Windrad da: reduzieren für Neuaufbau/Reparatur
  if(GetCon() == 100)
    if(!FindObject(LWNG,0,0,0,0,0,"Turn",this()) )
      DoCon(-1);
}
