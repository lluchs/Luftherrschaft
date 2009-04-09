/*-- Luftschiff --*/

#strict 2

#include BLMP

/* Initialisierung */

protected func Initialize() {
  // SolidMask neu setzen (wegen Shipsmith)
  SetSolidMask(GetDefCoreVal("SolidMask", "DefCore", GetID(), 0),GetDefCoreVal("SolidMask", "DefCore", GetID(), 1),GetDefCoreVal("SolidMask", "DefCore", GetID(), 2),GetDefCoreVal("SolidMask", "DefCore", GetID(), 3),GetDefCoreVal("SolidMask", "DefCore", GetID(), 4),GetDefCoreVal("SolidMask", "DefCore", GetID(), 5));
  return _inherited(...);
}

protected func ControlDigDouble(object controller)	// doppelgraben
{
	[$TxtSplit$]
	if(GetID(Contents(0, controller)) == LMCY) {
		var iX, iY;
		for(var i; i < 360; i += RandomX(1, 10))
		{
			iX = Cos(i, RandomX(40, 50));
			iY = Sin(i, RandomX(40, 50));
   		CreateParticle("PSpark", iX, iY, -iX, -iY, 30, RGB(255,255,255));
   	}
   	var iRan = RandomX(100, 255);
   	Schedule(Format("CreateParticle(\"PSpark\", 0, 0, 0, 0, 1000, RGB(%d, %d, %d))", iRan, iRan, iRan), 10, 0, this);
   	Schedule("CastParticles(\"NoGravSpark\", RandomX(10, 20), RandomX(10, 20), 0, 0, 5, 10, RGB(255, 255, 100), RGB(255, 255, 200))", 20, 0, this);
		Schedule("Split2Components()", 20, 0, this);
		RemoveObject(Contents(0, controller));
	}
	else
		Sound("Error");
}

/* Forschung */

public func GetResearchBase() { return(BALN); }

/* Bau */

public func IsShipsmithProduct() { return true; }

// Bauverhalten
// iPromille gibt an, wieviel Promille (1/1000) der Baumaterialien vorhanden sind
// true, wenn die Grafik aktualisiert werden soll, false wenn gewartet wird
// wie die Zahl ausgerechnet wird: (1000 / Gesamtzahl_aller_Components)*verbaute_components
public func NextBuildStep(int iPromille) {
  if(iPromille < 380)
    return true;
  if(iPromille == 1000)
    return true;
  return false;
}
