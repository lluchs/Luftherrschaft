/*-- Windrad --*/

#strict

#include BAS1

/* Initialisierung */

protected func Initialize()
{
  CreateWindwing();
}

/* Windrad erzeugen */

private func CreateWindwing()
{
  ObjectSetAction(CreateObject(WWNG,0,0,GetOwner()),"Turn",this());
}
