#strict 2

global func SetDir(int iDir) {
	if(iDir != GetDir() && GetEffect("VerticesDir", this)) {
		var iX, iCNAT;
		for(var i = 0; i < GetVertexNum(); i++) {
			iX = GetVertex(i);
			iX *= -1;
			SetVertex(i, 0, iX);
			iCNAT = GetDefCoreVal("VertexCNAT", "DefCore", GetID(), i);
			if(iDir != DIR_Left) {
				if(iCNAT & CNAT_Left) {
					iCNAT = iCNAT & ~ CNAT_Left;
					iCNAT = iCNAT | CNAT_Right;
				}
				else if(iCNAT & CNAT_Right) {
					iCNAT = iCNAT & ~ CNAT_Right;
					iCNAT = iCNAT | CNAT_Left;
				}
			}
			SetVertex(i, 2, iCNAT);
		}
	}
	return inherited(iDir, ...);
}

global func SetVerticesDir() {
	return AddEffect("VerticesDir", this, 1);
}
