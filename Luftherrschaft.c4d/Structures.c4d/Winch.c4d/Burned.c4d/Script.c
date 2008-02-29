/*-- Verbrannte Pumpe --*/

#strict

local Rope,Pull,Seilende,OldLength;

/* Fundament (Local 9) */

protected Destruction:
  if (Local(9)) RemoveObject(Local(9));
  return(1);

protected Incineration:
  // Seil entfernen
  if(Rope) RemoveObject(Rope);
  return(1);

