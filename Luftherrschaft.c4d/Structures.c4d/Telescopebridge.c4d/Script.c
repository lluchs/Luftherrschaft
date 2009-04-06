/*-- Teleskopbr�cke --*/

#strict 2 

local fUsed;

// Initializung

protected func Initialize()
{
  
  return 1;
}

// Steuerung

protected func ControlDigDouble()
{
  var iPhase = GetPhase(); // Phase speichern...
  if (!fUsed && GetAction() != "BridgeOut" && GetAction() != "Freez" && GetAction() != "Reverse") // Unbenutzt und hat nicht die Aktion "Reverse", "BridgeOut" oder "Freez"?
   SetAction("BridgeOut"); // Br�cke ausfahren
  if (fUsed && GetAction() != "Reverse" && GetAction() != "BridgeOut") // Benutzt und hat nicht die Aktion "Reverse" oder "BridgeOut"?
   {
   SetAction("Reverse"); // Br�cke einfahren
   if (iPhase != 32 && iPhase != 0) SetPhase(32-iPhase); // Phase setzen
   }
  return 1;
}

protected func ControlThrow()
{
  return 1;
}

// Der wichtigste Teil

protected func Timer()
{
  var iPhase = GetPhase(); // Phase speichern...
  if (GetAction() == "Reverse") CreateSolidMaskReverse();
  if (GetAction() != "BridgeOut") return 0; // Ist nicht die Aktion "BridgeOut" an? Abbrechen!
  if (GBackSemiSolid(-9+iPhase*2,9) || GetPhase() >= 32) // Material am n�sten Positionspunkt?
  {
    SetAction("Freez"); // ...Action einfrieren...
    SetPhase(iPhase); // ... auf die richtige Phase bringen...
    fUsed = true;
    return 0; // ...und abbrechen.
  }
  //CreateObject(_NEW,-9+iPhase*2,9);
  SetPhase(iPhase+1); // N�ste Phase.
  CreateSolidMask(); // SolidMask setzen
}

// SolidMask erstellen

protected func CreateSolidMask()
{
  var iPhase = GetPhase(); // Phase speichern...
  SetSolidMask(27+100*iPhase,14,14+iPhase*2,5,27,14); // SolidMask vergr��ern
}

protected func CreateSolidMaskReverse()
{
  var iPhase = GetPhase(); // Phase speichern...
  SetSolidMask(3227-100*iPhase,14,73-iPhase*2,5,27,14); // SolidMask verkleinern
}

protected func BrigdeOutDone(){ fUsed = true; } // Fertig ausgefahren

protected func ReverseDone() { fUsed = false; SetSolidMask(); } // Fertig eingefahren