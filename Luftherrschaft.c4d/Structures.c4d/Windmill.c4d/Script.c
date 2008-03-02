/*-- Windrad --*/

#strict

#include L050

private func Initialize()
{
  SetAction("Flag");
  ObjectSetAction(CreateObject(LWNG,0,0,GetOwner()),"Turn",this);
  //Log("Init Windrad");
  SetEntrance(1);
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
