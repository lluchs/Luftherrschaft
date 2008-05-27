/*-- Verbrannte Pumpe --*/

#strict

local Rope,Pull,Seilende,OldLength;
local basement;

/* Fundament */

protected func Destruction() {
  if(basement) RemoveObject(basement);
  return 1;
}

protected func Incineration() {
  // Seil entfernen
  if(Rope) RemoveObject(Rope);
  return 1;
}
