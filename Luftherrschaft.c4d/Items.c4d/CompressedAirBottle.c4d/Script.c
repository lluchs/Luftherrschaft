/*-- Druckluftbeh�lter --*/

#strict

local iDruck, iMaxDruck;

protected func Initialize() {
  iDruck = 500;
  iMaxDruck = 500;
  return 1;
}

public func IsFull() {
	if(iDruck)
		return 1;
	return;
}

public func RemovePression(iPression) {
	// Sound
	Sound("pfft");
	// wird versucht, mehr Druck abzuziehen, als beinhaltet wird?
	if(iDruck < iPression) {
		var Druck = iDruck; // Druck speichern
		// leer :[
		iDruck = 0;
		// "abgezogener" Druck zur�ckgeben
		return Druck;
	}
	// Druck abziehen
	iDruck -= iPression;
	// abgezogener Druck zur�ckgeben
	return iPression;
}

protected func Activate(pClonk) {
	return PlayerMessage(GetOwner(pClonk), "$TxtRestPression$", this, iDruck);
}
