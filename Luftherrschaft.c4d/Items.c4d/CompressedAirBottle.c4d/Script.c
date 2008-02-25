/*-- Druckluftbehälter --*/

#strict

local iDruck, iMaxDruck;

protected func Initialize() {
  iDruck = 500;
  iMaxDruck = 500;
  return 1;
}

/* Callbacks */
public func IsFull() {
	if(iDruck)
		return 1;
	return;
}

public func IsCompressedAirBottle() {
	return 1;
}

public func RemovePression(iPression) {
	// Sound
	Sound("pfft");
	// wird versucht, mehr Druck abzuziehen, als beinhaltet wird?
	if(iDruck < iPression) {
		var Druck = iDruck; // Druck speichern
		// leer :[
		iDruck = 0;
		// "abgezogener" Druck zurückgeben
		return Druck;
	}
	// Druck abziehen
	iDruck -= iPression;
	// abgezogener Druck zurückgeben
	return iPression;
}

public func Fill(int iPression) {
	if((iPression + iDruck) > iMaxDruck) {
		var Druck = iDruck;
		iDruck = iMaxDruck;
		return iMaxDruck - Druck;
	}
	iDruck += iPression;
	return iPression;
}

protected func Activate(pClonk) {
	return PlayerMessage(GetOwner(pClonk), "$TxtRestPression$", this, iDruck);
}
