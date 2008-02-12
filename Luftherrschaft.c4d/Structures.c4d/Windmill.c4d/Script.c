/*-- Windrad --*/

#strict

#include BAS1

private func Initialize()
{
  ObjectSetAction(CreateObject(LWNG,0,0,GetOwner()),"Turn",this());
}
