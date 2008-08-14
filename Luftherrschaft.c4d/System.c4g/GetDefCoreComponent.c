#strict 2

global func GetDefCoreComponent(id idComponent, int iIndex, id idDef) {
	if(!idDef && !(idDef = GetID()))
		return;
	if(!idComponent) {
		var ID = GetDefCoreVal("Components", "DefCore", idDef, iIndex * 2);
		if(!ID)
			return;
		if(ID == idComponent)
			return GetDefCoreVal("Components", "DefCore", idDef, iIndex * 2 + 1);
	}
	for(var i = 0;; i++) {
		ID = GetDefCoreVal("Components", "DefCore", idDef, i * 2);
		if(!ID)
			break;
		if(ID == idComponent)
			return GetDefCoreVal("Components", "DefCore", idDef, i * 2 + 1);
	}
}
