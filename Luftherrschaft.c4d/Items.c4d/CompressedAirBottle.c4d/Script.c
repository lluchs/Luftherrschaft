/*-- Druckluftbeh�lter --*/

#strict

static const LCAB_MaxFill = 500;

local iFillLevel;

protected func Initialize() {
  return 1;
}

/* Callbacks */
public func IsFull() {
	return iFillLevel == MaxFill();
}

public func IsCompressedAirBottle() {
	return 1;
}

// Maximale F�llung
public func MaxFill() { return LCAB_MaxFill; }

// F�llung erh�hen/verringern
public func DoFill(int iChange, bool fNoSound)
{
	if(!fNoSound)
		Sound("pfft");
  var iNewFill = BoundBy(iFillLevel + iChange, 0, MaxFill());
  if (iNewFill == iFillLevel) return;
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  // Tats�chliche �nderung des F�llstandes zur�ckgeben
  return iNewFill;
}

protected func Activate(pClonk) {
	return PlayerMessage(GetOwner(pClonk), "$TxtRestPression$", this, iDruck);
}
