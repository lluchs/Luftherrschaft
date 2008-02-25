/*-- Druckluftfabrik --*/

#strict

#include WRKS
#include L120
#include DOOR

protected func Initialize(){
	SetEntrance(1); // aktiver Eingang
	return 1;
}

protected func Collection2(object pObject) {
	if(!pObject -> ~IsCompressedAirBottle()) // haben wir eine Druckluftflasche?
		return; // keine Druckluftflasche, fertig.
	return FillBottle(pObject); // Flasche füllen!
}

private func FillBottle(object pBottle) {
	// Check: muss eine Druckluftflasche sein und sich in der Fabrik, die genügend Energie braucht, befinden
	if(!EnergyCheck(50) || !pBottle -> ~IsCompressedAirBottle() || !Contained(pBottle) == this)
		return;
	// Wasserfass suchen, dass sich in der Fabrik befindet und voll ist
  var pFass = FindObject2(Find_ID(WBRL), Find_Container(this), Find_Func("BarrelIsFull"));
  if(pFass) { // Fass gefunden!
  	RemoveObject(pFass); // Entfernen...
  	CreateContents(EmptyBarrelID()); // ...und neues Fass erstellen
  	pBottle -> Fill(500); // die Flasche wird gefüllt.
  	Sound("pfft"); // das hört sich auch ganz toll an.
  	return 1;	// fertig.
  }
  else { // kein Fass?
  	Message("$TxtNoWater$", this); // Beschweren!
  	Sound("Error");
  	return; // fertig.
  }
}

private func FindBottle() {
	return FindObject2(Find_Container(this), Find_ID(LCAB), Find_Not(Find_Func("IsFull")));
}

/* Steuerung */

protected func ContainedDig() {
	[$TxtFillBottle$, Image=LCAB]
	var pFlasche = FindBottle();
	if(pFlasche)
		return FillBottle(pFlasche);
	Sound("Error");
	return;
}

protected func ContextFillBottle() {
	[$TxtFillBottle$, Image=LCAB]
	var pFlasche = FindBottle();
	if(pFlasche)
		return FillBottle(pFlasche);
	Sound("Error");
	return;
}

/* Produkteigenschaften (überladbar) */
public func ProductType() { return C4D_Object; }
public func ProductCondition() { return "IsCompressedAirBottle"; }
