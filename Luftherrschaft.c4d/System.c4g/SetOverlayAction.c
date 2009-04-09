/* Aktion mit Animation als Overlay setzen */

#strict 2

/* Methode 1: alle Einzelframes in der ActMap.txt */

global func OverlayActionInit(string szAction, id ID) {
	if(!ID && !(ID = GetID()))
		return;
	var iLength = GetActMapVal("Length", szAction, ID);
	if(!iLength)
		return;
	var iFacetX = GetActMapVal("Facet", szAction, ID, 0),
	    iFacetY = GetActMapVal("Facet", szAction, ID, 1),
	    iFacetWidth = GetActMapVal("Facet", szAction, ID, 2),
	    iFacetHeight = GetActMapVal("Facet", szAction, ID, 3),
	    iFacetPX = GetActMapVal("Facet", szAction, ID, 4),
	    iFacetPY = GetActMapVal("Facet", szAction, ID, 5);
	for(var i = 0; i < iLength; i++) {
		Log("[Action]");
		Log("Name=%s%d", szAction, i);
		Log("Length=1");
		Log("Facet=%d,%d,%d,%d,%d,%d", iFacetX + iFacetWidth * i, iFacetY, iFacetWidth, iFacetHeight, iFacetPX, iFacetPY);
		Log("");
	}
	return iLength;
}

global func SetOverlayAction(string szAction, int iOverlay, bool fNoCalls) {
	if(!iOverlay)
		iOverlay = GetUnusedOverlayID(1);
	AddEffect("IntOverlayAction", this, 100, GetActMapVal("Delay", szAction, GetID()), this, 0, szAction, iOverlay, fNoCalls);
	return iOverlay;
}

global func FxIntOverlayActionStart(object pTarget, int iEffectNumber, int iTemp, string szAction, int iOverlay, bool fNoCalls) {
	if(iTemp)
		return;
	var iLength = GetActMapVal("Length", szAction, GetID());
	EffectVar(0, pTarget, iEffectNumber) = szAction;
	EffectVar(1, pTarget, iEffectNumber) = iOverlay;
	EffectVar(2, pTarget, iEffectNumber) = iLength;
	// 3: i
	if(!fNoCalls) {
		var szStartCall = GetActMapVal("StartCall", szAction, GetID()),
			  szPhaseCall = GetActMapVal("PhaseCall", szAction, GetID()),
			  szEndCall = GetActMapVal("EndCall", szAction, GetID());
		if(szStartCall)
			Call(szStartCall);
		EffectVar(4, pTarget, iEffectNumber) = szPhaseCall;
		EffectVar(5, pTarget, iEffectNumber) = szEndCall;
	}
}

global func FxIntOverlayActionTimer(object pTarget, int iEffectNumber) {
	var szAction = EffectVar(0, pTarget, iEffectNumber),
	    iLength = EffectVar(2, pTarget, iEffectNumber),
	    i = EffectVar(3, pTarget, iEffectNumber),
	    iOverlay = EffectVar(1, pTarget, iEffectNumber),
	    szPhaseCall = EffectVar(4, pTarget, iEffectNumber);
	SetGraphics(0, this, GetID(), iOverlay, GFXOV_MODE_Action, Format("%s%d", szAction, i));
	if(szPhaseCall)
		Call(szPhaseCall);
	i++;
	if(iLength == i)
		return -1;
	EffectVar(3, pTarget, iEffectNumber) = i;
}

global func FxIntOverlayActionEnd(object pTarget, int iEffectNumber) {
	var iOverlay = EffectVar(1, pTarget, iEffectNumber),
	    szEndCall = EffectVar(5, pTarget, iEffectNumber);
	SetGraphics(0, this, 0, iOverlay);
	if(szEndCall)
		Call(szEndCall);
}
