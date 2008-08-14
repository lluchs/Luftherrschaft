/*-- Druckluftbehälter --*/

#strict 2

#include SAVS

//static const LCAB_MaxFill = 400;
private func MaxFill() { return 400; }

protected func Initialize() {
	DoFill(200);
	return _inherited();
}

/* Callbacks */

public func IsCompressedAirBottle() { return 1; }

public func FillPicture() { return 0; }
private func FillSound() { return Sound("pfft"); }

/* Einen harten Aufschlag überlebt die Flasche nicht! So; Be careful! */
protected func Hit3() {
  var ExplodeSize;
  if (!GetAmount())
   return;
  else {
    CastObjects(LHSP, 4, 20);
		if (GetAmount() >= 40) {
	  	ExplodeSize = GetAmount() * GetAmount() / (40 - GetAmount()) + 15;
	  	Schedule(Format("Explode(%d)", ExplodeSize), 108, 1, 0);
		}
		if (GetAmount() <= 40) {
	  	ExplodeSize = GetAmount() / 40 + 20;
	  	Schedule(Format("Explode(%d)", ExplodeSize), 108, 1, 0);
		}
  }
}
	  
